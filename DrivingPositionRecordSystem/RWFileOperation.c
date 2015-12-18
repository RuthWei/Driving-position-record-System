//
//  fileOperation.c
//  DrivingPositionRecordSystem
//
//  Created by qianfeng on 15/12/15.
//  Copyright (c) 2015年 qianfeng. All rights reserved.
//

#include <stdio.h>
#include "RWFunction.h"
#include "RWFileOperation.h"
//存放车辆行驶信息的文件使用频繁，所以，打开后，在线程结束时再关闭
FILE * fpCarPositionRecord;

//车辆信息登记：车牌号，私家车还是公共车，
int carInfoRecord(carInfo_s * carInfo)
{
    FILE * fp ;
    if (!carInfo) {
        return 0;
    }
    //只写方式，打开车辆信息记录文件
    //如果失败，退出
    fp = fopen(CARINFO_PATH, "wb");
    if (!fp) {
        return 0;
    }
    
    //输入车辆信息，并且保存在文件中
    printf("输入车牌号:");
    scanf("%s",carInfo->carLicense);
    printf("输入车辆类型:\n私家车(%d),出租车、公交车等公用车(%d)\n", privateCar,stateCar);
    scanf("%d", &(carInfo->carType));
    if (carInfo->carType == privateCar) {
        printf("输入司机姓名:");
        scanf("%s", carInfo->name);
    }
    //读取文件信息保存在carInfo里
    if (!fwrite(carInfo, sizeof(carInfo_s), 1, fp))
    {
        //写入错误，返回0
        return 0;
    }
    
    fclose(fp);
    
    return 1;
}
//打开文件，读取文件内容
int carInfoRead(carInfo_s * carInfo)
{
    FILE * fp ;
    if (!carInfo) {
        return 0;
    }
    //只读方式，打开车辆信息记录文件
    //如果成功，表示已经登记过了。如果失败，表示第一次使用该设备
    fp = fopen(CARINFO_PATH, "r");
    if (!fp) {
        return 0;
    }
    
    //读取文件信息保存在carInfo里
    if (!fread(carInfo, sizeof(carInfo_s), 1, fp))
    {
        //读取错误，返回0
        return 0;
    }
    
    fclose(fp);
    
    return 1;
}

/*
 函数名：carGpsRecordRead
 函数功能：打开文件，读取文件内容，把文件内容存放到链表里
 函数返回值：
 返回1,表示旧数据读完；返回－1,出错，返回其它值，都要继续进行读
 函数参数：
 NODE * head表示链表头
 long start表示读取的启始位置
 start=-1上次读取出错
 start=0第一次读取
 start=1旧数据读到一半/新数据读到一半
 start>1旧数据读完，直接读新数据
 */
long carGpsRecordRead(NODE * head, long start)
{
    GPS gpsInfo;
    long len = 0;
    NODE * p = NULL;
    
    if (!head) {
        return -1;
    }
    
    if (!fpCarPositionRecord){
        createFileRecordGps();
    }
    
    //读出文件开头保存的数据
    fread(&len, sizeof(long), 1, fpCarPositionRecord);
    if (-1 == start) {
        //上次读取出错
        fseek(fpCarPositionRecord, len, SEEK_SET);
    }
    else if (0 == start)
    {
        //第一次读取，先跳转到旧数据位置，先读旧数据
        fseek(fpCarPositionRecord, len, SEEK_SET);
    }
    else if (start > 1)
    {
        //旧数据读到一半/新数据读到一半
        fseek(fpCarPositionRecord, start, SEEK_SET);
    }
    else if (1 == start){
        //旧数据读完，直接读新数据
    }
    
    p = head->next;
    count = 0;
    //读取文件信息保存在链表里
    while (!feof(fpCarPositionRecord)
        && p != NULL
        && p != head){
        if (!fread(&gpsInfo, sizeof(GPS), 1, fpCarPositionRecord)){
            //读取错误，返回-1
            return -1;
        }
        memcpy(&(p->position),&gpsInfo,sizeof(gpsInfo));
        p = p->next;
        count++;//记录本次读了几个结点的数据
    }
    
    if (p == head) {
        //链表读满了
        return ftell(fpCarPositionRecord);
    }
    
    //文件读完了
    if (feof(fpCarPositionRecord)) {
        return 1;
    }
    
    if (!p) {
        return -1;//文件读取出错了
    }
    return 0;
}
//打开文件，该文件保存的是车辆gps信息
int createFileRecordGps()
{
    long pos = 0;
    //读方式，打开车辆信息记录文件
    //如果成功，表示已经登记过了。如果失败，表示第一次使用该设备
    fpCarPositionRecord = fopen(DRIVERRECORD_PATH, "r+");
    if (fpCarPositionRecord) {
        return 1;
    }
    
    //读方式出错，需要写方式，打开车辆行驶记录文件
    //如果失败，退出
    fpCarPositionRecord = fopen(DRIVERRECORD_PATH, "w+");
    if (!fpCarPositionRecord) {
        return 0;
    }
    fwrite(&pos, sizeof(long), 1, fpCarPositionRecord);
    
    return 1;
}

//保存数据
void saveData(NODE * head)
{
    NODE * cur;
    long len = 0;
    //如果文件还没有打开
    if(!fpCarPositionRecord)
    {
        //读方式打开
        fpCarPositionRecord = fopen(DRIVERRECORD_PATH,"r+");
        //读方式打开失败，文件还没有创建，写方式打开
        if (!fpCarPositionRecord )
        {
            fpCarPositionRecord = fopen(DRIVERRECORD_PATH,"w+");
        }
        fwrite(&len,sizeof(long),1,fpCarPositionRecord );
    }
    //跳转到文件的开头
    rewind(fpCarPositionRecord );
    //文件的最前面存放前面有多少数据最新的，这些数据之后就是老数据
    fread(&len,sizeof(int),1,fpCarPositionRecord );
    //跳转到旧数据的位置
    fseek(fpCarPositionRecord ,len,SEEK_CUR);
    cur=head;
    while(cur != NULL && count >= 0)
    {
        //把链表中数据全部写到文件中
        fwrite(cur,sizeof(NODE),1,fpCarPositionRecord );
        cur=cur->next;
        count--;
    }
    
    len = ftell(fpCarPositionRecord);
    rewind(fpCarPositionRecord);
    if(len > FILE_SIZE)
        len = 0;
    fwrite(&len,sizeof(long),1,fpCarPositionRecord );
    fflush(fpCarPositionRecord);
}