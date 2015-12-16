//
//  main.c
//  DrivingPositionRecordSystem
//
//  Created by qianfeng on 15/12/11.
//  Copyright (c) 2015年 qianfeng. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>//erase
//#include <conio.h>
#include <CoreGraphics/CGBase.h>
#include "list.h"
void delay()
{
    for (int i = 0; i < 50000000; i++) {
        i++;
        i--;
    }
}
int main(int argc, const char * argv[]) {
    // insert code here...
//    far putpixel(200,200, 2);
//    line(0, 0, 200, 200);
//    refresh();
//    erase();
//    clear();
//    printf("Hello\b, World!\n");
//    for (int i = 1; i < 101; i++) {
//        printf("\r%3d%%", i);
//        //sleep(1);
//        delay();
//    }
//    //system("clear");
    //创建链表
    NODE * head = createList();
    //添加结点
    addNode(head);
    addNode(head);
    addNode(head);
    addNode(head);
    showList(head);
    deleteList(head);
    return 0;
}
