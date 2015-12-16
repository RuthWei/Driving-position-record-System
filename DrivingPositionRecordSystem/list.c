//
//  list.c
//  DrivingPositionRecordSystem
//
//  Created by qianfeng on 15/12/11.
//  Copyright (c) 2015年 qianfeng. All rights reserved.
//

#include "list.h"
//创建链表,返回链表的头结点
NODE * createList()
{
    NODE * head;
    head = (NODE *)malloc(sizeof(NODE) * 1);
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
    while (p) {
        free(p);
        p = p->next;
    }
    free(head);
    return;
}
//添加结点
void addNode(NODE * head)
{
    NODE * p;
    //如果链表为空，则退出
    if (!head) {
        printf("亲，现在还没有链表呢！\n");
        return;
    }
    //新建一结点，并且申请空间
    NODE * newNode;
    newNode = (NODE *)malloc(sizeof(NODE) * 1);
    //如果申请空间失败，退出
    if (!newNode) {
        return;
    }
    //给结点赋值
    setNode(newNode);
    
    //把结点添加到链表的头部的下一个位置
    //实现链表中结点的时间由当前时间到离现在较远的时间
    //保存头结点的下一个结点
    p = head->next;
    //新结点的next指针指向头结点的下一个结点
    newNode->next = p;
    //头结点的next指针指向新结点
    head->next = newNode;
}

//显示链表中的信息
void showList(NODE * head)
{
    //如果链表为空，则退出
    if (!head) {
        printf("亲，现在还没有链表呢！\n");
        return;
    }
    NODE * p = head->next;
    while (p) {
        GPS gps  = p->position;
        printf("%d->", gps.data);
        p = p->next;
    }
    printf("NULL\n");
}


void setNode(NODE * node)
{
    //结点为null，退出
#if 0
    //获取数据，并且给结点赋值
#else
    static int num = 0;
    node->position.data = num;
    num++;
#endif
}