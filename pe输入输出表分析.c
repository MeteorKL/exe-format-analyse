//
//  main.c
//  test
//
//  Created by Meteor on 16/6/1.
//  Copyright © 2016年 meteor. All rights reserved.
//

#include "stdio.h"

#define PE 0x200

#define byte char
#define word short int
#define dword int
int virturalAddressToFileAddress(int address)
{
    if (address >= 0x59000)
        address = address - 0x59000 + 0x51200;
    else if (address >= 0x11000)
        address = address - 0x11000 + 0xb000;
    return address;
}

void getApiNoAndName(FILE *fp, dword inputTableAPIPoint)
{
    word inputTableAPINo;
    char inputTableAPIName[50];
    int offset;
    
    offset = inputTableAPIPoint;
    fseek(fp, offset, SEEK_SET);
    fread(&inputTableAPINo, sizeof(char), 2, fp);
    printf("+0x%08x %04x: API序号 %04x \n", offset, inputTableAPINo, inputTableAPINo);
    offset += 2;
    
    int i=0;
    while (1) {
        fread(&inputTableAPIName[i], sizeof(char), 1, fp);
        if (inputTableAPIName[i]==0) {
            break;
        }
        i++;
    }
    printf("+0x%08x : API名字 %s \n", offset, inputTableAPIName);
    offset += 4;
}

void getApiNoAndNameList(FILE *fp, dword inputTableAPIPointList)
{
    dword inputTableAPIPoint;
    int offset;
    
    offset = inputTableAPIPointList;
    printf("-------------------------------------------------------------------------\n");
    while (1) {
        fseek(fp, offset, SEEK_SET);
        fread(&inputTableAPIPoint, sizeof(char), 4, fp);
        if (inputTableAPIPoint==0)
            break;
        printf("+0x%08x %08x: API名字指针 %08x ", offset, inputTableAPIPoint, inputTableAPIPoint);
        inputTableAPIPoint = virturalAddressToFileAddress(inputTableAPIPoint);
        printf("文件地址 %08x \n", inputTableAPIPoint);
        getApiNoAndName(fp, inputTableAPIPoint);
        offset += 4;
    }
    printf("-------------------------------------------------------------------------\n");
}

void getDllName(FILE *fp, dword inputTableDllNamePoint)
{
    char inputTableDllName[30];
    int offset;

    offset = inputTableDllNamePoint;
    fseek(fp, offset, SEEK_SET);
    
    int i=0;
    while (1) {
        fread(&inputTableDllName[i], sizeof(char), 1, fp);
        if (inputTableDllName[i]==0) {
            break;
        }
        i++;
    }
    printf("+0x%08x : dll名字 %s \n", offset, inputTableDllName);
    offset += 4;
}

void GetInputTableList(FILE *fp, dword inputTable)
{
    dword inputTableAPINamePointList, inputTableDllNamePoint, inputTableAPIAddressList;
    int offset;
    
    offset = inputTable;
    while (1) {
        fseek(fp, offset, SEEK_SET);
        fread(&inputTableAPINamePointList, sizeof(char), 4, fp);
        if (inputTableAPINamePointList==0) {
            break;
        }
        printf("+0x%08x %08x: 指向该dll所引用的API名字指针表 %08x ", offset, inputTableAPINamePointList, inputTableAPINamePointList);
        inputTableAPINamePointList = virturalAddressToFileAddress(inputTableAPINamePointList);
        printf("文件地址 %08x \n", inputTableAPINamePointList);
        offset += 0xC;
        fseek(fp, 8, SEEK_CUR);
        fread(&inputTableDllNamePoint, sizeof(char), 4, fp);
        printf("+0x%08x %08x: dll名字指针 %08x ", offset, inputTableDllNamePoint, inputTableDllNamePoint);
        inputTableDllNamePoint = virturalAddressToFileAddress(inputTableDllNamePoint);
        printf("文件地址 %08x \n", inputTableDllNamePoint);
        getDllName(fp, inputTableDllNamePoint);
        fread(&inputTableAPIAddressList, sizeof(char), 4, fp);
        printf("+0x%08x %08x: 指向该dll所引用的API地址指针表 %08x ", offset, inputTableAPIAddressList, inputTableAPIAddressList);
        inputTableAPIAddressList = virturalAddressToFileAddress(inputTableAPIAddressList);
        printf("文件地址 %08x (载入内存时会由操作系统修改)\n", inputTableAPIAddressList);
        
        getApiNoAndNameList(fp, inputTableAPINamePointList);
        offset += 0x8;
    }
}

void getApiName(FILE *fp, dword outputTableAPIPoint, int n)
{
    char outputTableAPIName[50];
    int offset;
    
    offset = outputTableAPIPoint;
    fseek(fp, offset, SEEK_SET);
    int i=0;
    while (1) {
        fread(&outputTableAPIName[i], sizeof(char), 1, fp);
        if (outputTableAPIName[i]==0) {
            break;
        }
        i++;
    }
    printf("+0x%08x : API名字 %s \n", offset, outputTableAPIName);
    offset += 4;
}

void getApiNameList(FILE *fp, dword outputTableAPIPointList, int n)
{
    dword outputTableAPIPoint;
    int offset;
    
    offset = outputTableAPIPointList;
    printf("-------------------------------------------------------------------------\n");
    int i=0;
    while (1) {
        fseek(fp, offset, SEEK_SET);
        fread(&outputTableAPIPoint, sizeof(char), 4, fp);
        if (i==n)
            break;
        printf("+0x%08x %08x: API名字指针 %08x ", offset, outputTableAPIPoint, outputTableAPIPoint);
        outputTableAPIPoint = virturalAddressToFileAddress(outputTableAPIPoint);
        printf("文件地址 %08x \n", outputTableAPIPoint);
        getApiName(fp, outputTableAPIPoint, n);
        offset += 4;
        i++;
    }
    printf("-------------------------------------------------------------------------\n");
}

void getApiNoList(FILE *fp, dword outputTableAPIPointList, int n)
{
    dword outputTableAPIPoint;
    int offset;
    
    offset = outputTableAPIPointList;
    printf("-------------------------------------------------------------------------\n");
    int i=0;
    fseek(fp, offset, SEEK_SET);
    while (1) {
        fread(&outputTableAPIPoint, sizeof(char), 2, fp);
        if (i==n)
            break;
        printf("+0x%08x %04x: API序号 %x \n", offset, outputTableAPIPoint, outputTableAPIPoint);
        offset += 2;
        i++;
    }
    printf("-------------------------------------------------------------------------\n");
}

//IMAGE_EXPORT_DIRECTORY STRUCT
//Characteristics           DWORD      ?;未使用,总是为0
//TimeDateStamp             DWORD      ?;文件产生的时刻
//MajorVersion              WORD       ?;未使用,总是为0
//MinorVersion              WORD       ?;未使用,总是为0
//nName                     DWORD      ?;指向文件名的RVA
//nBase                     DWORD      ?;导出函数的起始序号
//NumberOfFunctions         DWORD      ?;导出函数的总数
//NumberOfNames             DWORD      ?;以名称导出的函数总数
//AddressOfFunctions        DWORD      ?;指向导出函数地址表的RVA
//AddressOfNames            DWORD      ?;指向函数名地址表的RVA
//AddressOfNameOrdinals     DWORD      ?;指向函数名序号表RVA
void GetOutputTableList(FILE *fp, dword outputTable)
{
    dword dw, DllNamePoint, startNumber, number, nameNumber;
    int offset;
    
    offset = outputTable + 0x0c;
    fseek(fp, offset, SEEK_SET);
    fread(&DllNamePoint, sizeof(char), 4, fp);
    printf("+0x%08x(+%02x) %08x: 指向文件名的RVA %08x ", offset, offset-outputTable, DllNamePoint, DllNamePoint);
    DllNamePoint = virturalAddressToFileAddress(DllNamePoint);
    printf("文件地址 %08x \n", DllNamePoint);
    getDllName(fp, DllNamePoint);
    offset += 4;
    
    fseek(fp, offset, SEEK_SET);
    fread(&startNumber, sizeof(char), 4, fp);
    printf("+0x%08x(+%02x) %08x: 导出函数的起始序号 %x \n", offset, offset-outputTable, startNumber, startNumber);
    offset += 4;
    
    fread(&number, sizeof(char), 4, fp);
    printf("+0x%08x(+%02x) %08x: 导出函数的总数 %x \n", offset, offset-outputTable, number, number);
    offset += 4;
    
    fread(&nameNumber, sizeof(char), 4, fp);
    printf("+0x%08x(+%02x) %08x: 以名称导出的函数总数 %x \n", offset, offset-outputTable, nameNumber, nameNumber);
    offset += 4;
    
    fread(&dw, sizeof(char), 4, fp);
    printf("+0x%08x(+%02x) %08x: 指向导出函数地址表的RVA %08x ", offset, offset-outputTable, dw, dw);
    dw = virturalAddressToFileAddress(dw);
    printf("文件地址 %08x \n", dw);
    offset += 4;
    
    fread(&dw, sizeof(char), 4, fp);
    printf("+0x%08x(+%02x) %08x: 指向函数名地址表的RVA %08x ", offset, offset-outputTable, dw, dw);
    dw = virturalAddressToFileAddress(dw);
    printf("文件地址 %08x \n", dw);
    getApiNameList(fp, dw, nameNumber);
    offset += 4;
    
    fseek(fp, offset, SEEK_SET);
    fread(&dw, sizeof(char), 4, fp);
    printf("+0x%08x(+%02x) %08x: 指向函数名序号表RVA %08x ", offset, offset-outputTable, dw, dw);
    dw = virturalAddressToFileAddress(dw);
    printf("文件地址 %08x \n", dw);
    getApiNoList(fp, dw, number);
    offset += 4;
    
//    fread(&inputTableAPIAddressList, sizeof(char), 4, fp);
//    printf("+0x%08x %08x: 指向该dll所引用的API地址指针表 %08x ", offset, inputTableAPIAddressList, inputTableAPIAddressList);
//    inputTableAPIAddressList = virturalAddressToFileAddress(inputTableAPIAddressList);
//    printf("文件地址 %08x (载入内存时会由操作系统修改)\n", inputTableAPIAddressList);
//    
//    getApiNoAndNameList(fp, inputTableAPINamePointList);
//    offset += 0x8;
}

int main(void)
{
    const char *filename1="/Users/meteor/Downloads/DllCallStatic.exe";
    const char *filename2="/Users/meteor/Downloads/DllSample.dll";
    FILE *fp;
    dword dwTemp, inputTable, inputTableLen, outputTable, outputTableLen;
    fp = fopen(filename1, "rb");
    
    int offset = PE + 0x74;
    fseek(fp, offset, SEEK_SET);
    fread(&dwTemp, sizeof(char), 4, fp);
    printf("+0x%08x %08x: 标志 %08x \n", offset, dwTemp, dwTemp);
    offset += 4;
    
    fread(&outputTable, sizeof(char), 4, fp);
    printf("+0x%08x %08x: 输出表虚拟内存地址（RVA） %08x ", offset, outputTable, outputTable);
    outputTable = virturalAddressToFileAddress(outputTable);
    printf("文件地址 %08x \n", outputTable);
    offset += 4;
    
    fread(&outputTableLen, sizeof(char), 4, fp);
    printf("+0x%08x %08x: 输出表长度 %x \n", offset, outputTableLen, outputTableLen);
    offset += 4;
    
    fread(&inputTable, sizeof(char), 4, fp);
    printf("+0x%08x %08x: 输入表虚拟内存地址（RVA） %08x ", offset, inputTable, inputTable);
    inputTable = virturalAddressToFileAddress(inputTable);
    printf("文件地址 %08x \n", inputTable);
    offset += 4;
    
    fread(&inputTableLen, sizeof(char), 4, fp);
    printf("+0x%08x %08x: 输入表长度 %x \n", offset, inputTableLen, inputTableLen);
    offset += 4;
    
    GetInputTableList(fp, inputTable);
    //GetOutputTableList(fp, outputTable);
}
