//
//  list.c
//  DrivingPositionRecordSystem
//
//  Created by qianfeng on 15/12/11.
//  Copyright (c) 2015年 qianfeng. All rights reserved.
//

#include "RWList.h"
#include "RWGps.h"
NODE * head;
//定义一个全局变量存放车辆当前位置信息
GPS carPostionRecord;

//定义一个计数器，每0.2秒记录一次，每5秒写一次文件
int count = 0;

//创建循环链表,返回链表的头结点
NODE * createList(int num)
{
    int i = 0;
    NODE * p;
    
    head = (NODE *)malloc(sizeof(NODE) * 1);
    if (!head) {
        return head;
    }
    
    p = head;
    while (i < num) {
        p->next = (NODE *)malloc(sizeof(NODE) * 1);
        if (!p->next) {
            return head;
        }
        p = p->next;
        i++;
    }
    
    p->next = head;
    return head;
}

//删除链表
void deleteList(NODE * head)
{
    //如果链表为空，则退出
    if (!head) {
        printf("亲，现在还没有链表呢！\n");
        return;
    }
    
    NODE * p = head->next;
    while (p && p != head) {
        head->next = p->next;
        free(p);
        p = p->next;
    }
    free(head);
    return;
}
//添加结点
void addNode(NODE * head, NODE * node)
{
    NODE * p;
    //如果链表为空，则退出
    if (!head) {
        printf("亲，现在还没有链表呢！\n");
        return;
    }
    
    if (!node) {
        node = (NODE *)malloc(sizeof(NODE) * 1);
        memset(node, 0, sizeof(NODE));
    }
//    //给新结点设置值
//    setNode(node);
    //把结点添加到链表的头部的下一个位置
    //实现链表中结点的时间由当前时间到离现在较远的时间
    //保存头结点的下一个结点
    p = head->next;
    //新结点的next指针指向头结点的下一个结点
    node->next = p;
    //头结点的next指针指向新结点
    head->next = node;
}

//显示链表中的信息
void showList(NODE * head)
{
    //int i = 0;
    //如果链表为空，则退出
    if (!head) {
        printf("亲，现在还没有链表呢！\n");
        return;
    }
    NODE * p = head->next;
    while(p && p != head && count > 0){
        showPosition(&p->position);
        p = p->next;
        count--;
    }
//    printf("NULL\n");
}

void setNode(NODE * newNode)
{
    //结点为null，退出
    if (!newNode ) {
        printf("亲，结点为null，不能给它设置值\n");
        return;
    }
#if 1
    //获取数据，并且给结点赋值
//    pthread_mutex_lock(&lock);
    memcpy(&(newNode->position),&carPostionRecord,sizeof(carPostionRecord));
//    pthread_mutex_unlock(&lock);
#else
    static int num = 0;
    node->data = num;
    num++;
#endif
}