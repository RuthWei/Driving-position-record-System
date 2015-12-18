//
//  RWGps.h
//  DrivingPositionRecordSystem
//
//  Created by qianfeng on 15/12/17.
//  Copyright (c) 2015年 qianfeng. All rights reserved.
//

#ifndef __DrivingPositionRecordSystem__RWGps__
#define __DrivingPositionRecordSystem__RWGps__

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include "RWList.h"

#define NODENUM 100//链表中有效结点为100个
//两个线程同时操作一个全局变量，需要用互斥锁把它锁定
//extern pthread_mutex_t lock;
#if 0
extern int fdGPS;
#else
extern FILE * fpGPS;
#endif
//打开GPS设备
int openGPS();
void * readGpsInfo(void * data);
void showPosition(GPS * position);
#endif /* defined(__DrivingPositionRecordSystem__RWGps__) */
