//
//  RWGps.c
//  DrivingPositionRecordSystem
//
//  Created by qianfeng on 15/12/17.
//  Copyright (c) 2015年 qianfeng. All rights reserved.
//

#include "RWGps.h"
#include "RWFunction.h"
#include "RWFileOperation.h"

#if 1
FILE * fpGPS;
#else
int fdGPS;
#endif
//pthread_mutex_t lock;
int openGPS()
{
#if 0
    struct termios tms;
    //只读方式打开文件
    fdGPS = open(GPS_PATH,O_RDONLY|O_NDELAY|O_NOCTTY);
    if(fdGPS <= 0)
    {
        perror("ERROR");
        return 0;
    }
    //设置串口属性
    tcgetattr(fdGPS,&tms);
    tms.c_cflag |=CS8;
    tms.c_cflag &=~PARENB;
    tms.c_cflag &=~CSTOPB;
    cfsetispeed(&tms,B4800);
    tcsetattr(fdGPS,TCSANOW,&tms);
#else
    fpGPS = fopen(GPS_PATH,"r");
    if(!fpGPS)
    {
        perror("ERROR");
        return 0;
    }
#endif
    return 0;
}
//打开gps文件，读取里面的数据，存放在一个全局变量中
void * readGpsInfo(void * data)
{
    struct gpsinfo gps={0};
    char crmc[128]=" ";
    char cgga[128]=" ";
    char ct=0;
    //获取当前的系统时间
    time_t seconds;
    struct tm *ts;
    char isvalid;           //是否合法
    float tmp = 0;
    
    //从gps中获取的数据中，以GPRMC开头的数据是有效数据
    //如果从设备中读数据，是可以一直读下去的，用while(1)
    //如果从文件中读数据，会有读完的时候，用while(!feof(fpGPS))
#if 0
    while(1)
#else
    while(!feof(fpGPS))
#endif
    {
        while(strcmp(crmc,"GPRMC")!=0)
        {
            //read(fdGPS,&ct,1);
            fread(&ct, 1, 1, fpGPS);
            while(ct != '$')
            {
                //read(fdGPS,&ct,1);
                if (0 == fread(&ct, 1, 1, fpGPS))
                    return NULL;//文件读完
            }
            memset(crmc,'\0',sizeof(crmc));
            //read(fdGPS,crmc,5);
            fread(crmc, 5, 1, fpGPS);
        }
        memset(crmc,'\0',sizeof(crmc));
        //read(fdGPS,crmc,65);
        //fread(crmc, 70, 1, fpGPS);
        fgets(crmc, sizeof(crmc) - 1, fpGPS);//读取一行
//        printf("crmc=%s\n",crmc);
        //可以从GPRMC中获得该数据是否有效，获取经度纬度，
        //,094915.000,A,3414.9093,N,10900.0406,E,0.00,0.00,210512,,,A*60
        sscanf(crmc,",%f,%c,%f,%c,%f,%c",&tmp,&isvalid,&gps.latitude,&gps.latitude_flag,&gps.longitude,&gps.longitude_flag);
        gps.isvalid=isvalid=='A'?1:0;
        
        //可以从GPGGA中获取经度纬度，海拔高度
        while(strcmp(cgga,"GPGGA")!=0)
        {
            //read(fdGPS,&ct,1);
            fread(&ct, 1, 1, fpGPS);
            while(ct != '$')
            {
                //read(fdGPS,&ct,1);
                if (0 == fread(&ct, 1, 1, fpGPS))
                    return NULL;//文件读完
            }
            memset(cgga,'\0',sizeof(cgga));
            //read(fdGPS,cgga,5);
            fread(cgga, 5, 1, fpGPS);
        }
        //read(fdGPS,cgga,46);
        fgets(cgga, sizeof(cgga) - 1, fpGPS);
        //memset(cgga,'\0',sizeof(cgga));
        //read(fdGPS,cgga,10);
        //fread(cgga, 10, 1, fpGPS);
        sscanf(cgga + 46,"%f",&gps.hight);
        
        //时间,日期
        seconds = time(NULL);
        ts = localtime(&seconds);
        gps.datetime.year = ts->tm_year + 1900;
        gps.datetime.mon  = ts->tm_mon + 1;
        gps.datetime.day  = ts->tm_mday;
        gps.datetime.hour = ts->tm_hour;
        gps.datetime.min  = ts->tm_min;
        gps.datetime.sec  = ts->tm_sec;

        //将分析GPS得到的数据拷贝到一个全局变量里，准备保存到文件中。
//        pthread_mutex_lock(&lock);
        memset(&carPostionRecord, 0, sizeof(carPostionRecord));
        memcpy(&carPostionRecord, &gps, sizeof(gps));
//        pthread_mutex_unlock(&lock);
        //showPosition(&gps);
        memset(crmc,'\0',sizeof(crmc));
        memset(cgga,'\0',sizeof(cgga));
        break;//
    }
    
//    close(fdGPS);
    return 0;
}

void showPosition(GPS * position)
{
    printf("%04d/%02d/%02d  %02d:%02d:%02d  ", position->datetime.year, position->datetime.mon, position->datetime.day, position->datetime.hour, position->datetime.min, position->datetime.sec);
    if ('N' == position->latitude_flag
     || 'n' == position->latitude_flag) {
        printf("北纬:");
    }
    else
    {
        printf("南纬:");
    }
    printf("%10.4f ", position->latitude);
    if ('E' == position->longitude_flag
        || 'e' == position->longitude_flag) {
        printf("东经:");
    }
    else
    {
        printf("西经:");
    }
    printf("%10.4f\n", position->longitude);
    
}