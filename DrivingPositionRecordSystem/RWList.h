//
//  RWRWList.h
//  DrivingPositionRecordSystem
//
//  Created by qianfeng on 15/12/11.
//  Copyright (c) 2015年 qianfeng. All rights reserved.
//

#ifndef __DrivingPositionRecordSystem__list__
#define __DrivingPositionRecordSystem__list__

#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<fcntl.h>
#include<termios.h>
#include<stdlib.h>
#include "RWMenu.h"

//每0.2秒记录一条信息，每秒记录5条，每5秒记录25条信息
#define TIMES5SECOND 25
#define TIMESPERSECOND 300
//定义一个位段结构体，准备存放时间
typedef struct stime
{
    unsigned int year:12;//0~4096
    unsigned int mon:4;//0~16
    unsigned int day:5;//0~32
    unsigned int hour:5;//0~32
    unsigned int min:6;//0~64
    unsigned int sec:6;//0~64
}TIME_S;
//GPS信息结构体
struct gpsinfo
{
#if 1
    TIME_S datetime;
    float longitude;	//经度
    float latitude;		//纬度
    char longitude_flag;//N表示北纬，S表示南纬
    char latitude_flag;	//E表示东经，W表示西经
    float hight;//海拔高度
    int isvalid;
#else
    int data;
#endif
    
};
typedef struct gpsinfo GPS;

//链表结点结构体,每个结点存储一分钟的信息
struct node
{
    GPS position;
    struct node * next;
};
typedef struct node NODE;

//车辆信息登记
typedef struct carInfo
{
    char name[20];
    char carLicense[10];//车牌号
    CARTYPE_E carType;//车辆类型
    
}carInfo_s;

extern carInfo_s carInfo;
extern NODE * head;
extern GPS carPostionRecord;
extern int count;
//创建链表,返回链表的头结点,num是链表中结点的个数
NODE * createList(int num);
//删除链表
void deleteList(NODE * head);
//添加结点
void addNode(NODE * head, NODE * node);
//获取当前位置信息，并且给结点进行赋值,count为这一分钟的第几次数据
void setNode(NODE * node);
//显示链表中的信息
void showList(NODE * head);
#endif /* defined(__DrivingPositionRecordSystem__list__) */
