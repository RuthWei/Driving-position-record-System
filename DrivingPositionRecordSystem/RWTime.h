//
//  RWTime.h
//  DrivingPositionRecordSystem
//
//  Created by qianfeng on 15/12/16.
//  Copyright (c) 2015年 qianfeng. All rights reserved.
//

#ifndef __DrivingPositionRecordSystem__RWTime__
#define __DrivingPositionRecordSystem__RWTime__

#include <stdio.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

void timer_deal(int sig);

//static void default_timer_open();
void default_timer_close();
int default_timer_open();
#endif /* defined(__DrivingPositionRecordSystem__RWTime__) */
