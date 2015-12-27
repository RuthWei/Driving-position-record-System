//
//  RWFileOperation2.h
//  DrivingPositionRecordSystem
//
//  Created by qianfeng on 15/12/16.
//  Copyright (c) 2015年 qianfeng. All rights reserved.
//

#ifndef __DrivingPositionRecordSystem__RWFileOperation2__
#define __DrivingPositionRecordSystem__RWFileOperation2__

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "RWList.h"

#define FILE_SIZE 8192
//读取文件的初始状态
#define INIT_STATUS 0x00
//新数据已经读完,也就是文件全部读完
#define NEWDATA_FINISHED (0x01)
//新数据读了一半
#define NEWDATA_UNFINISHED (0x02)
//旧数据全部读完
#define OLDDATA_FINISHED (0x04)
//旧数据读了一半
#define OLDDATA_UNFINISHED (0x08)
//读取文件失败
#define READ_ERROR 0x10

//存放车辆行驶信息的文件使用频繁，所以，打开后，在线程结束时再关闭
extern FILE * fpCarPositionRecord;
extern long len;
extern long curFileSize;

//车辆信息登记：车牌号，私家车还是公共车，
int carInfoRecord(carInfo_s * carInfo);
//打开文件，读取车辆信息
int carInfoRead(carInfo_s * carInfo);
//创建文件，存储车辆gps信息
int createFileRecordGps();
/*
 函数名：carGpsRecordRead
 函数功能：打开文件，读取文件内容，把文件内容存放到链表里
 函数返回值：
 返回1,表示旧数据读完；返回－1,出错，返回其它值，都要继续进行读
 函数参数：
 NODE * head表示链表头
 int * state//当前读取数据的状态
 */
int carGpsRecordRead(NODE * head, int * state);

//保存数据
void saveData(NODE * head);
#endif /* defined(__DrivingPositionRecordSystem__RWFileOperation2__) */
