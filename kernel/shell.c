//POI-OS task shell
//by MT
#include "type.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "keyboard.h"
#include "proto.h"
#include "shell.h"
PRIVATE int compareAndDo(char* input,SHELLINFO* info);
PUBLIC void print_welcome()
{
	printf("Shell is running..\n---- Welcome to Poi OS ----\n");
	delay(5);
	printf(" _____   ____ _____ /\\/|\n");
	printf("|  __ \\ / __ \\_   _|/\\/\n");
	printf("| |__) | |  | || |\n");
	printf("|  ___/| |  | || |\n");
	printf("| |    | |__| || |\n");
	printf("|_|     \\____/_____|    \n\n");	
	printf("@ input help to get all commands @\n");
	
}

void divideOrder(char* input,char* goal)
{
	for(int i=0;i<STR_DEFAULT_LEN;i++)
	{
		if(input[i]!=' ' && input[i]!="\0")
			goal[i] = input[i];
		else
		{
			goal[i]='\0';
			return;
		}
	}
}

//处理有关进程的事务
PRIVATE void manageProcess(char* input)
{
	int nameLen = 0;
	switch(input[6])
	{
	case LIST_ALL:
		printf("\n-------------------------");
		for(int i=0;i<NR_TASKS;i++)
		{
			nameLen = strlen(task_table[i].name);
			printf("\ntask: %s ",task_table[i].name);
			for(int j=0;j<9-nameLen;j++)
				printf(" ");
			printf("| pid: %d",proc_table[i].pid);
		}	
		for(int i=0;i<NR_PROCS;i++)
		{
			nameLen = strlen(proc_table[i+NR_TASKS].name);
			printf("\nprocess: %s ",proc_table[i+NR_TASKS].name);
			for(int j=0;j<6-nameLen;j++)
				printf(" ");
			printf("| pid: %d",proc_table[i+NR_TASKS].pid);
		}	
		printf("\n-------------------------\n");
		break;
	default:
		printf("manage process failed");
	}
}

PRIVATE void processInputError(SHELLINFO* info)
{
	printf("\nUndefined command.");
	int distance = 0;
	int minDistance = 65536;
	int goalOrder = -1;
	for(int i=0;i<NR_ORDERS;i++)
	{
		int orderILen = strlen(info->allOrder[i]);
		delay(1);
		
		if(strlen(info->currentOrder) == orderILen)
		{
			
			for(int j=0;j<orderILen;j++)
			{
				if(info->currentOrder[j] != info->allOrder[i][j])
					distance++;
			}
			
			if(distance < minDistance)
			{
				goalOrder = i;
				minDistance = distance;
			}
			distance = 0;
		}
	}
	if(goalOrder != -1)
	{
		for(int i=0;i<strlen(info->allOrder[goalOrder]);i++)
			info->lastInput[i] = info->allOrder[goalOrder][i];
		printf(" Do you mean \"%s\" ? ",info->lastInput);
	}
}

PRIVATE int compareAndDo(char* input,SHELLINFO* info)
{
	while(1)
	{
		divideOrder(input,info->currentOrder);
		//info->ifError = 0;//先假设没有出错
		if(strcmp(input,GIRL) == 1)
		{
			printf(":zyy");
			break;
		}		
		if(strcmp(info->currentOrder,CLEAR) == 1)
				return TTY_DO_CLEAR;
		if(strcmp(info->currentOrder,HELP) == 1)
		{
			printf("\n---------  What You Could Do  ---------\n");
			delay(1);
			printf("help . . . . . . | list what you could do\n");
			delay(1);
			printf("clear . . . . . .| clear your screen\n");
			delay(1);
			printf("proc [-options]  | manage all process\n");

			break;
		}
		if(strcmp(info->currentOrder,PROC) == 1)
		{
			manageProcess(input);
			break;
		}
		if(strcmp(info->currentOrder,POI) == 1)
		{
			if(info->ifError == 1)
			{
				strcpy(input,info->lastInput);
				info->ifError = 0;
				continue;
			}
			printf("\npoi~ poi~ poi~");
			break;			
		}
		strcpy(info->lastInput,input);
		//未检查到的指令
		info->ifError = 1;
		processInputError(info);
		break;
	}
	
	return 0;
}

PRIVATE void init_info(SHELLINFO* info)
{
	info->ifError = 0;
	strcpy(info->allOrder[0],GIRL);
	printf("!!%d",strlen(info->allOrder[0]));
	strcpy(info->allOrder[1],HELP);
	strcpy(info->allOrder[2],CLEAR);
	strcpy(info->allOrder[3],PROC);
	strcpy(info->allOrder[4],POI);
}
PUBLIC void task_shell()
{
	SHELLINFO info;
	init_info(&info);

	print_welcome();

	delay(1);
	MESSAGE msg;
	reset_msg(&msg);
	while(1){
		send_recv(RECEIVE,ANY,&msg);
		printf("\nShabby Input : %s",msg.INSSM);
		divideOrder(msg.INSSM,info.currentOrder);
		printf(" | length: %d | order %s",strlen(msg.INSSM),info.currentOrder);
		delay(1);
			
		int outType = compareAndDo(msg.INSSM,&info);
		
		strcpy(msg.INSSM,"");
		reset_msg(&msg);
		msg.type = outType;
		send_recv(SEND,TASK_TTY,&msg);
	}
}
