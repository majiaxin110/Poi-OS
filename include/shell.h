//针对shell.c的一系列声明

#ifndef _POI_SHELL_H
#define _POI_SHELL_H

typedef struct shell_storage
{
    char lastInput[STR_DEFAULT_LEN];
    char currentOrder[STR_DEFAULT_LEN];
    char allOrder[NR_ORDERS][STR_DEFAULT_LEN];

    int ifError;//上条指令是否有错误
}SHELLINFO;

#endif