//
//  menu.h
//  DrivingPositionRecordSystem
//
//  Created by qianfeng on 15/12/16.
//  Copyright (c) 2015年 qianfeng. All rights reserved.
//

#ifndef __DrivingPositionRecordSystem__menu__
#define __DrivingPositionRecordSystem__menu__

#include <stdio.h>

//公用的车，在记录时，需要输入司机姓名，私家车只有第一次记录时，需要记录；其它时间，不需要
typedef enum carType
{
    privateCar,//私家车
    stateCar,//公共车
    maxCar
}CARTYPE_E;

typedef enum operate
{
    displayRecord,
    continueRecord,
    max,
}OPERATE_E;

//欢迎界面
void welcomeMenu();
//使用界面
void operateMenu();

#endif /* defined(__DrivingPositionRecordSystem__menu__) */
