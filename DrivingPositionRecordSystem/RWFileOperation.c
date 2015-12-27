//
//  fileOperation.c
//  DrivingPositionRecordSystem
//
//  Created by qianfeng on 15/12/15.
//  Copyright (c) 2015年 qianfeng. All rights reserved.
//

#include <stdio.h>
#include "RWFunction.h"
#include "RWFileOperation.h"
//存放车辆行驶信息的文件使用频繁，所以，打开后，在线程结束时再关闭
FILE * fpCarPositionRecord;
long len = 0;
long curFileSize = 0;
//车辆信息登记：车牌号，私家车还是公共车，
int carInfoRecord(carInfo_s * carInfo)
{
    FILE * fp ;
    if (!carInfo) {
        return 0;
    }
    //只写方式，打开车辆信息记录文件
    //如果失败，退出
    fp = fopen(CARINFO_PATH, "wb");
    if (!fp) {
        return 0;
    }
    
    //输入车辆信息，并且保存在文件中
    printf("输入车牌号:");
    scanf("%s",carInfo->carLicense);
    printf("输入车辆类型:\n私家车(%d),出租车、公交车等公用车(%d)\n", privateCar,stateCar);
    scanf("%d", &(carInfo->carType));
    if (carInfo->carType == privateCar) {
        printf("输入司机姓名:");
        scanf("%s", carInfo->name);
    }
    //读取文件信息保存在carInfo里
    if (!fwrite(carInfo, sizeof(carInfo_s), 1, fp))
    {
        //写入错误，返回0
        return 0;
    }
    
    fclose(fp);
    
    return 1;
}
//打开文件，读取文件内容
int carInfoRead(carInfo_s * carInfo)
{
    FILE * fp ;
    if (!carInfo) {
        return 0;
    }
    //只读方式，打开车辆信息记录文件
    //如果成功，表示已经登记过了。如果失败，表示第一次使用该设备
    fp = fopen(CARINFO_PATH, "r");
    if (!fp) {
        return 0;
    }
    
    //读取文件信息保存在carInfo里
    if (!fread(carInfo, sizeof(carInfo_s), 1, fp))
    {
        //读取错误，返回0
        return 0;
    }
    
    fclose(fp);
    
    return 1;
}

/*
 函数名：carGpsRecordRead
 函数功能：打开文件，读取文件内容，把文件内容存放到链表里
 函数返回值：
 返回1,表示数据读完；返回－1,出错，返回其它值，都要继续进行读
 函数参数：
 NODE * head表示链表头
 int * state//当前读取的状态
 */
int carGpsRecordRead1(NODE * head, int * state)
{
    GPS position;
    NODE * p = NULL;
    
    if (!head) {
        return -1;
    }
    //读出文件开头保存的数据
    if (INIT_STATUS == * state){
        //获取文件的大小
        fseek(fpCarPositionRecord, 0, SEEK_END);
        curFileSize = ftell(fpCarPositionRecord);
        if (curFileSize == 0) {
            * state = NEWDATA_FINISHED;
            printf("亲，您的记录仪还未曾使用过!\n");
            return 1;
        }
        rewind(fpCarPositionRecord);
        fread(&len, sizeof(long), 1, fpCarPositionRecord);
    }
    p = head->next;
    count = 0;
    //读取文件信息保存在链表里
    while (!feof(fpCarPositionRecord)
           && p != NULL
           && p != head
           //&& len != ftell(fpCarPositionRecord)
           ){
        size_t ret = 0;
        ret = fread(&position, sizeof(GPS), 1, fpCarPositionRecord);
        if (ferror(fpCarPositionRecord)){
            //读取错误，返回-1
            *state = READ_ERROR;
            return -1;
        }
        /*
         else if (feof(fpCarPositionRecord))
         {
         //新数据读完了
         * state = NEWDATA_FINISHED;
         return 1;
         }
         else if (ret == 0){
         return 0;//??
         }
         */
        memcpy(&(p->position),&position,sizeof(position));
        p = p->next;
        count++;//记录本次读了几个结点的数据
    }
    
    //链表满了
    if (p == head) {
        //文件读完了
        if (feof(fpCarPositionRecord)) {
            *state = NEWDATA_FINISHED;
        }
        else{
            *state = NEWDATA_UNFINISHED;
        }
        //链表读满了
        return 1;
    }
    
    if (feof(fpCarPositionRecord))
    {
        //新数据读完了
        * state = NEWDATA_FINISHED;
        return 1;
    }
    
    if (!p) {
        *state = READ_ERROR;
        return -1;//文件读取出错了
    }
    return 0;

    
}
int carGpsRecordRead(NODE * head, int * state)
{
    GPS position;
    NODE * p = NULL;
    
    if (!head) {
        return -1;
    }
    
    if (!fpCarPositionRecord){
        createFileRecordGps();
    }
    
    //读出文件开头保存的数据
    if (INIT_STATUS == * state){
        //获取文件的大小
        fseek(fpCarPositionRecord, 0, SEEK_END);
        curFileSize = ftell(fpCarPositionRecord);
        if (curFileSize == 0) {
            * state = NEWDATA_FINISHED;
            printf("亲，您的记录仪还未曾使用过!\n");
            return 1;
        }
        rewind(fpCarPositionRecord);
        fread(&len, sizeof(long), 1, fpCarPositionRecord);
    }
//    printf("len = %ld, filesize=%ld\n",len, curFileSize);
    //如果当前len的大小和文件的大小是一样的，说明当前文件的所有数据都是最新数据，并且按时间的先后从文件开头一直到文件结尾，只需要从头读到尾就可以了
    if (len == curFileSize) {
        switch (*state) {
            case INIT_STATUS:
                //跳到新数据的起始位置
//                fseek(fpCarPositionRecord, sizeof(len), SEEK_SET);
                //开始读数据
                
                break;
            case NEWDATA_UNFINISHED:
                break;
            default:
                break;
        }
        goto READ_A;
    }
    else if (len < curFileSize){
        switch (* state) {
            case INIT_STATUS:
                //跳到新数据的起始位置
                fseek(fpCarPositionRecord, (len), SEEK_SET);
                //从旧数据开始读
                goto READ_B;
                break;
            case OLDDATA_FINISHED:
                fseek(fpCarPositionRecord, sizeof(len), SEEK_SET);
                goto READ_B;
            case OLDDATA_UNFINISHED:
                goto READ_B;
                break;
            default:
                break;
        }
    }
    else{
        return -1;
    }

READ_A:
    p = head->next;
    count = 0;
    //读取文件信息保存在链表里
    while (!feof(fpCarPositionRecord)
        && p != NULL
        && p != head
        //&& len != ftell(fpCarPositionRecord)
           ){
        size_t ret = 0;
        ret = fread(&position, sizeof(GPS), 1, fpCarPositionRecord);
        if (ferror(fpCarPositionRecord)){
            //读取错误，返回-1
            *state = READ_ERROR;
            return -1;
        }
        /*
        else if (feof(fpCarPositionRecord))
        {
            //新数据读完了
            * state = NEWDATA_FINISHED;
            return 1;
        }
        else if (ret == 0){
            return 0;//??
        }
        */
        memcpy(&(p->position),&position,sizeof(position));
        p = p->next;
        count++;//记录本次读了几个结点的数据
    }
    
    //链表满了
    if (p == head) {
        //文件读完了
        if (feof(fpCarPositionRecord)) {
            *state = NEWDATA_FINISHED;
        }
        else{
            *state = NEWDATA_UNFINISHED;
        }
        //链表读满了
        return 1;
    }
    
    if (feof(fpCarPositionRecord))
    {
        //新数据读完了
        * state = NEWDATA_FINISHED;
        return 1;
    }
    
    if (!p) {
        *state = READ_ERROR;
        return -1;//文件读取出错了
    }
    return 0;
READ_B:
    p = head->next;
    count = 0;
    //读取文件信息保存在链表里
    while (!feof(fpCarPositionRecord)
           && p != NULL
           && p != head
           //&& len != ftell(fpCarPositionRecord)
           ){
        size_t ret = 0;
        ret = fread(&position, sizeof(GPS), 1, fpCarPositionRecord);
        if (ferror(fpCarPositionRecord)){
            //读取错误，返回-1
            *state = READ_ERROR;
            return -1;
        }
        memcpy(&(p->position),&position,sizeof(position));
        p = p->next;
        count++;//记录本次读了几个结点的数据
    }
    
    //链表满了
    if (p == head) {
        //文件读完了
        if (feof(fpCarPositionRecord)) {
            *state = OLDDATA_FINISHED;
        }
        else{
            //文件没有读完
            if (*state == OLDDATA_UNFINISHED
            ||  *state == INIT_STATUS) {
                //正在读旧数据
                *state = OLDDATA_UNFINISHED;
            }
            else {
                //正在读新数据
                *state = NEWDATA_UNFINISHED;
            }
        }
        //链表读满了
        return 1;
    }
    
    if (feof(fpCarPositionRecord))
    {
        //新数据读完了
        * state = NEWDATA_FINISHED;
        return 1;
    }
    
    if (len <= ftell(fpCarPositionRecord)
    && *state == NEWDATA_UNFINISHED) {
        *state = NEWDATA_FINISHED;
        return 1;
    }
    if (!p) {
        *state = READ_ERROR;
        return -1;//文件读取出错了
    }
    return 0;

}
//打开文件，该文件保存的是车辆gps信息
int createFileRecordGps()
{
    long pos = 0;
    //读方式，打开车辆信息记录文件
    //如果成功，表示已经登记过了。如果失败，表示第一次使用该设备
    fpCarPositionRecord = fopen(DRIVERRECORD_PATH, "r+");
    if (fpCarPositionRecord) {
        return 1;
    }
    
    //读方式出错，需要写方式，打开车辆行驶记录文件
    //如果失败，退出
    fpCarPositionRecord = fopen(DRIVERRECORD_PATH, "w+");
    if (!fpCarPositionRecord) {
        return 0;
    }
    fwrite(&pos, sizeof(long), 1, fpCarPositionRecord);
    
    return 1;
}

//保存数据
void saveData(NODE * head)
{
    NODE * cur;
    long curWriteLen = 0;
    //如果文件还没有打开
    if(!fpCarPositionRecord)
    {
        //读方式打开
        fpCarPositionRecord = fopen(DRIVERRECORD_PATH,"r+");
        //读方式打开失败，文件还没有创建，写方式打开
        if (!fpCarPositionRecord )
        {
            fpCarPositionRecord = fopen(DRIVERRECORD_PATH,"w+");
        }
        fwrite(&curWriteLen,sizeof(long),1,fpCarPositionRecord );
    }
    //跳转到文件的开头
    rewind(fpCarPositionRecord );
    //文件的最前面存放前面有多少数据最新的，这些数据之后就是老数据
    fread(&curWriteLen,sizeof(curWriteLen),1,fpCarPositionRecord );
    //跳转到旧数据的位置,即将要写的数据要覆盖旧数据，保证文件中的数据都是最新的
    if (curWriteLen > 0){
        rewind(fpCarPositionRecord);
        fseek(fpCarPositionRecord ,curWriteLen,SEEK_SET);
    }
    else
    {
        //把0写到文件中，只为了一会在这个位置保存写入的大小
        fwrite(&curWriteLen, sizeof(curWriteLen), 1, fpCarPositionRecord);
    }
    cur=head;
    
    while(cur != NULL && count > 0)
    {
        //把链表中数据全部写到文件中
        fwrite(&(cur->position),sizeof(GPS),1,fpCarPositionRecord );
        cur=cur->next;
        count--;
    }
    
    curWriteLen = ftell(fpCarPositionRecord);
    printf("%ld\n",ftell(fpCarPositionRecord));
    if(curWriteLen > FILE_SIZE)
        curWriteLen = sizeof(curWriteLen);
    rewind(fpCarPositionRecord);
    fwrite(&curWriteLen,sizeof(long),1,fpCarPositionRecord );
    fflush(fpCarPositionRecord);
}