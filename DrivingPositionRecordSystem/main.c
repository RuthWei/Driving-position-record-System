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
//#include <curses.h>//erase
////#include <conio.h>
//#include <CoreGraphics/CGBase.h>
#include "RWList.h"
#include "RWMenu.h"
#include "RWFunction.h"
#include "RWGps.h"
#include "RWFileOperation.h"
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

//    carInfo_s carInfo;
    int choice = 0;
    prepareWork();
    operateMenu();
    choice = getChoice();
    doSomething(choice);
    releaseWork();

    return 0;
}
