//
//  function.c
//  DrivingPositionRecordSystem
//
//  Created by qianfeng on 15/12/16.
//  Copyright (c) 2015年 qianfeng. All rights reserved.
//

#include "RWFunction.h"
#include "RWList.h"
#include "RWFileOperation.h"
#include "RWGps.h"
//
carInfo_s carInfo;
/*
 如果是公用车，在继续记录之前需要输入当前车辆的司机。
 如果是私家车，不需要再次输入司机姓名
 需要一个全局变量存放车辆类型和司机姓名
 */
char driverName[20];
CARTYPE_E carType;


//输入选择
int getChoice()
{
    int ret = 0;
    while (!scanf("%d", &ret)
        || ret >= max
        || ret < 0)
    {
        while (getchar() != '\n') {
            ;
        }
    }
    return ret;
}

void prepareWork()
{
    //打开文件读取车辆信息
    if (!carInfoRead(&carInfo))
    {
        printf("第一次使用该系统，请录入车辆基本信息\n");
        //打开文件失败，需要重新进行记录
        if (!carInfoRecord(&carInfo))
        {
            printf("保存车辆信息出错\n");
            exit(1);
        }
    }
    //打开车辆状态文件，读取文件内容
    //创建链表
    head = createList(NODENUM);
    //打开文件，文件中保存的GPS信息
    createFileRecordGps();
    //打开GPS
    if (!fpGPS) {
        openGPS();
    }
}
//根据选择进行相应的操作
void doSomething(int choice)
{
    int ret = 0;
    int state = INIT_STATUS;
    NODE * p = head->next;
    NODE * lastTimeNode = p;
    count = 0;
    
    switch (choice) {
        case displayRecord:
            carInfoShow(&carInfo);
            ret = carGpsRecordRead(head, &state);
            while (ret != -1
            && NEWDATA_FINISHED != state) {
                showList(head);
                ret = carGpsRecordRead(head, &state);
            }
            showList(head);
            break;
        case continueRecord:
            //记录停车前的100个数据，每0.2秒记录一次
#if 0
            signal(SIGALRM, timer_deal);
            pthread_t thread_id;
            
            pthread_mutex_init(&lock,NULL);
            创建一个线程不停的读取GPS的信息
            pthread_create(&thread_id, NULL, readGpsInfo, NULL);
            default_timer_open();
#endif
            
            while (1){
                //读取GPS信息
                readGpsInfo(NULL);
                //保存在链表中
                setNode(p);
                count++;
                //每隔0.2秒记录一次
                usleep(200000);
                //每隔5秒，往文件中记录一次
                if (count >= TIMES5SECOND) {
                    saveData(lastTimeNode);
                    lastTimeNode = p;
#if 0
                    //如果测试，在1分钟后，跳出循环，结束工作
                    ret++;
                    if (ret >= 1) {
                        break;
                    }
#endif
                    //break;//
                }
                p = p->next;
                if (feof(fpGPS)) {
                    saveData(lastTimeNode);
                    printf("gps file end\n");
                    return ;
                }
            }
        default:
            break;
    }
//    default_timer_close();
//    pthread_mutex_destroy(&lock);
    //close(fdGPS);
    
END:
    printf("==========    end    =======\n");
}

void carInfoShow(carInfo_s * carInfo)
{
    printf("carLicense:%s",carInfo->carLicense);
    
    if (privateCar == carInfo->carType)
    {
        printf(" carType:privateCar");
    }
    else{
        printf(" carType:stateCar");
    }
    
    if (strcmp(carInfo->name, "")) {
        printf(" driverName:%s", carInfo->name);
    }
    printf("\n");
}

//结束工作
void releaseWork()
{
    fclose(fpGPS);
    fclose(fpCarPositionRecord);
    fclose(fpGPS);
    deleteList(head);
}
