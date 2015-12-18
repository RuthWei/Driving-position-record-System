//
//  function.h
//  DrivingPositionRecordSystem
//
//  Created by qianfeng on 15/12/16.
//  Copyright (c) 2015年 qianfeng. All rights reserved.
//

#ifndef __DrivingPositionRecordSystem__function__
#define __DrivingPositionRecordSystem__function__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "RWList.h"

#define TEST 0

#ifdef TEST
#define GPS_PATH "./GPS_data.txt"
#define CARINFO_PATH "./carInfo.txt"
#define DRIVERRECORD_PATH "./driverRecord.txt"
#else
#define GPS_PATH "/dev/ttyUSB0"
#define CARINFO_PATH "/bin/carinfo.txt"
#define DRIVERRECORD_PATH "/bin/driverRecord.txt"
#endif

/*
 如果是公用车，在继续记录之前需要输入当前车辆的司机。
 如果是私家车，不需要再次输入司机姓名
 需要一个全局变量存放车辆类型和司机姓名
 */
extern char driverName[20];
extern CARTYPE_E carType;



//显示车辆信息
void carInfoShow(carInfo_s * carInfo);
//输入选择
int getChoice();
//根据选择进行相应的操作
void doSomething(int choice);
//准备工作
void prepareWork();
//结束工作
void releaseWork();
#endif /* defined(__DrivingPositionRecordSystem__function__) */
