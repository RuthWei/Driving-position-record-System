//
//  list.h
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

#define TEST 0

#ifdef TEST
#define GPS_PATH "./gpsData.txt"
#else
#define GPS_PATH "/dev/ttyUSB0"
#endif
//GPS信息结构体
struct gpsinfo
{
#if 0
    time_t datetime;
    float longitude;	//经度
    float latitude;		//纬度
    char longitude_flag;//N表示北纬，S表示南纬
    char latitude_flag;	//E表示东经，W表示西经
    float hight;
    int isvalid;
#else
    int data;
#endif
    
};
typedef struct gpsinfo GPS;

//链表结点结构体
struct node
{
    GPS position;
    struct node * next;
};
typedef struct node NODE;

//创建链表,返回链表的头结点
NODE * createList();
//删除链表
void deleteList(NODE * head);
//添加结点
void addNode(NODE * head);
//打开文件，读取文件内容，把文件内容存放到链表里
int fileOperation();
//获取当前位置信息，并且给结点进行赋值
void setNode(NODE * node);
//显示链表中的信息
void showList(NODE * head);
#endif /* defined(__DrivingPositionRecordSystem__list__) */
