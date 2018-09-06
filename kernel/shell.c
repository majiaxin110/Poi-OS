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

PUBLIC void print_welcome()
{
	printf("Shell is running..\n---- Welcome to Poi OS ----\n");
	delay(5);
	printf(" _____   ____ _____ /\\/|\n");
	delay(1);
	printf("|  __ \\ / __ \\_   _|/\\/\n");
	delay(1);
	printf("| |__) | |  | || |\n");
	delay(1);
	printf("|  ___/| |  | || |\n");
	delay(1);
	printf("| |    | |__| || |\n");
	delay(1);
	printf("|_|     \\____/_____|    \n\n");
	delay(1);	
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

PUBLIC void task_shell()
{
	SHELLINFO info;
	print_welcome();
	delay(1);
	MESSAGE msg;
	reset_msg(&msg);
	while(1){
		send_recv(RECEIVE,ANY,&msg);
		printf("\nShabby Input : %s",msg.INSSM);
		if(strcmp(msg.INSSM,"mostlovelygirl\0") == 1)
			printf(":zyy");
		divideOrder(msg.INSSM,info.currentOrder);
		printf(" | length: %d | order %s",strlen(msg.INSSM),info.currentOrder);
		delay(1);
		strcpy(msg.INSSM,"");
		reset_msg(&msg);
		msg.type = 2;
		if(strcmp(info.currentOrder,"clear\0") == 1)
			msg.type = TTY_DO_CLEAR;
		if(strcmp(info.currentOrder,"help\0") == 1)
		{
			printf("\n------ What You Could Do ------\n");
			delay(1);
			printf("help   | list what you could do\n");
			delay(1);
			printf("clear  | clear your screen");
		}
		send_recv(SEND,TASK_TTY,&msg);
	}
}
