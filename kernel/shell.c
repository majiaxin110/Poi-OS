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
#include "game2048.h"

#define ANIMATION_DELAY_TIME 1500

PRIVATE int compareAndDo(char* input,SHELLINFO* info);

PRIVATE int atoi(char* str)
{
	int result = 0;
	for(int i=0;i<strlen(str);i++)
	{
		if(str[i] >= '0' && str[i] <= '9')
			result = result*10 + str[i] - '0';
		else
			break;
	}
	return result;
}

PUBLIC void print_welcome()
{
	printf("Shell is running..\n");

// open OS animation
	int i = 8;
	while(i){
		printf("QBQBBBBBiSB: .::.  iBBBBBBBBBBBBBBBBQBBB\n");
		printf("BBBBBBBBd  rBBBBBBB. QBQBBBBBBBQBBBBBBBB\n");
		printf("BBBBBBBBQ dBBBBBBBBBu .rXBBBBBQBBBBBBBBB\n");
		printf("BBBQBBBBv B .BBBBd QB LJ  rBQddPPPd:dBBB\n");
		printf("Bv ....:  BBBBu.EBEBB Bu:.  .:..... PBBB\n");
		printf("BBggMgRQB  BBBD7BBBB :BBBBBi BBBBBBBBBBB\n");
		printf("BBBBBBBBBBv .PBBDUi 7BBBQBBB 2BBBBBBBBBB\n");
		printf("BBBBBQBBBBBBE  iLK7 BBBBBBBB: BBBBBBBBBB\n");
		printf("QBBBQBBBBBBBi.1 BB uBBBBBBBB: BBBBBBBBBB\n");
		printf("BBBBBBBBBBBB BBv  qQBBBBBBBB iBBBBBBBBBB\n");
		printf("QBBBBBBBBBB7 BQB BBBBBBBBBQQ BQBBBBBBBBB\n");
		printf("BQBBBBBBBDP SBBB KBBBBBBBBB rQBQBBBBBBBB\n");
		printf("QBBBBBBBB5  BBQBd 7SPgBBBU .BBBBQBBBBBQB\n");
		printf("BBBBBBBBBBBQBBBBBBPvi:r  .SBBBBBBBBQBQBB\n");
		printf("QBQBBBBBQBBBBBBBQBBBBBB uBBBBBBBBBBBBBBB\n");
		printf("BBBBBBBBBBBBBBBBBBBBBBB XBBBBBBBBQBBBQBB\n");
		printf("QBBBBBBBBBBBBBBBQBBBBBB KBQBBBQBBBBBBBBB\n");
		printf("BBBQBQBBBBBBBBBBBQBBBBB PQBBBBBBBBBBBBBQ\n");
		printf("QBBBBBBBQBBBBBBBBBQBBBB XBBBBBBBBBBBBBBB\n");
		printf("BBBBBBBQBQBBBBBBBQBQBBB 5BBBBBBBBBBBBBBB\n");
		printf("QBBBBBBBBBBBBBBBBBBBBBB YBBBBBBBQBBBBBQB\n");
		milli_delay(ANIMATION_DELAY_TIME);	
		clear_screen(&(console_table[0]));
		printf("BBBBBBBBBBBB2      vBBBBBBBQBBBBBBBBBBBB\n");
		printf("QBBBBBBBR :  5BBBBP  QBBBBQBBBBBBBQBQBQB\n");
		printf("BBBBBBBBBB YBBBQBBBB7 2BBBBBBBBBBBBBBQBB\n");
		printf("QBBBBBBBB: B JBBBBvrB  .:RBBBBBBBBBBBBBB\n");
		printf("BBqKRgRMB iQ sBBBBi Br.B   qUv77rrr: RBB\n");
		printf("QBi ...... QBBg  BBBB vRs2: 7sv77v777BBB\n");
		printf("BBBQBQBBBBK :ZBBBBBX :BBBBBi.QBBBBBBBQBB\n");
		printf("QBBBBBBBQBBBJ. .... rBBBBBBB PBBBBBBBBBB\n");
		printf("BBBBBBBBBBBBPBBBBBB gBBBBBBB .BBBBBBBBBB\n");
		printf("QBBBQBBBBBQq  :BBB .BBBBBBBB .QBBBBBBBQB\n");
		printf("BBBQBBBQBQB 7B. r iBBBBQBBBB 2BBBBBBBBBB\n");
		printf("QBBBQBBBBBB BBB iBBBBBBBBBBs BBBBBBBBBBB\n");
		printf("BBBBBBBBBB: BBB .BBBBBBBBBD DBBBBBBQBQBQ\n");
		printf("QBBBBBBBBB 2BBBB  IBBBBBBu IBBBBBBBBQBBB\n");
		printf("BQBBBBBQB  BBBBBBJ.  .ir. gBBQBBBBBBBBBB\n");
		printf("QBBBBBBBBBBBBBBBBBBBQbP 7BQBBBBBBBBBBBQB\n");
		printf("BBBBBBBBBQBBBBBBBBBBBQB QQBBBBBBBBBBBBBB\n");
		printf("QBBBQBBBBBQBBBBBBBQBBBB DBBBBBBBBBBBBBBB\n");
		printf("BBBBBBBBBBBBBBBQBQBBBBB dBBQBQBBBBBQBBBB\n");
		printf("QBBBQBQBBBQBBBBBBBBBBBB SBBBBBBBBBBBBBBB\n");
		printf("BBBBBBBBBBBBBQBBBBBBBBB 5BBBBBBQBBBBBQBB\n");
		milli_delay(ANIMATION_DELAY_TIME);	
		clear_screen(&(console_table[0]));
		printf("QBBBBBBBQBBBBBBY.. .iRBBQBQBBBBBBBQBBBQB\n");
		printf("BBBBBBBBBBg:B. :UPE5i .BBBBQBBBBBBBBBBBB\n");
		printf("QBBBBBQBQBBr .BBBBBBBB  BBBBBBBBBBBBBBBB\n");
		printf("BQBBBBBBBBBr BEBBQBQBBB RBBBBBBQBBBBBBBB\n");
		printf("QBBBBBBBBBB KB gBBQB 7Q. BQBBBBBBBQBBBBB\n");
		printf("BQBBBBBBBBBi.BXBgjdBXBB 7 KQBBBBBBBBBBBB\n");
		printf("QBB5.ir7rvJL JBB5.7BBB  J  .i:...... 7BB\n");
		printf("BBBD7vvr7r7Y1  vQBBg1 :BgMB iDdKqqggMBBB\n");
		printf("QBBBBBBBQBBBBB1: ... dBBQBBR BBBQBBBBBBB\n");
		printf("BBBBBBBBBQBQBBBBBBBr BBBBQBB XBBBBBBBBBB\n");
		printf("QBBBBBBBQBBBBBBBQBq BBBBQBBB:7BBBBBBBBBB\n");
		printf("BBBBBQBBBBBQBBBBBY PBBBBBQBB.7BBBBBBBQBQ\n");
		printf("QBBBQBQBBB2:SBBB  RBBBBBQBBB QBBQBBBBBBB\n");
		printf("BBBBBQBBBB  :   jBBBBBBQBBBs.QBBBBBBBBBQ\n");
		printf("QBBBBBBBQB BBB: BBBBBBBBBBB QBQBBBBBBBBB\n");
		printf("BQBBBBBBBB BBBB7 rBBBBBBBB LBBBQBBBBBBBB\n");
		printf("QBBBQBBBQg BBBQBQ.  .rJgS iBQBBBBBQBBBBB\n");
		printf("BBBQBBBBB: BBBBBBBBEu77  5BBBBBQBBBQBBBB\n");
		printf("QBBBBBBBu 7BBBBBBBQBBBB SBBBBBBBBBBBBBBB\n");
		printf("BBBBBBBQBBBBBBBBBBBBBBB JBBBBBBBBBBBBBBB\n");
		printf("QBBBBBBBBBBBQBBBBBBBBBB uBBBBBQBBBQBBBBB\n");
		printf("BBBBBBBBBQBBBBBBBBBBBBB XBBBBBBBBQBBBBBB\n");
		printf("QBBBQBBBBBBBBBBBBBBBBBB gBBBBBBBQBQBBBBB\n");
		milli_delay(ANIMATION_DELAY_TIME);	
		clear_screen(&(console_table[0]));
		printf("BBBBBQBBBBBBBBBBBQBB5i:.iIBBBBBBBBBBBQBB\n");
		printf("QBQBQBBBQBBBBBBBB.   rj1i  5BBBBBBBBBBBB\n");
		printf("BBBBBBBBBBBBBQBBB  BBBBBBBZ 2QBBBQBBBBBB\n");
		printf("QBBBBBBBBBBBQBBB  BBBBBBBBB5 BBBBBBBQBBB\n");
		printf("BBBBBBBBBQBBBQBB QB gQBQB7 B gBBBBBBBBBB\n");
		printf("QBQBQBQBQBBBBBBB BBQBB..BBQB BQBBBBBBBBB\n");
		printf("BBBBBBBBBBBQBQBBr BQBBSXBBB rBBBBBBBBBBB\n");
		printf("QBQBQBQBBBBBBBBBBX iSMBBU  qBBBBBBBBBBBB\n");
		printf("BBBBBBBB      ...i.  .   : u7i:::::... s\n");
		printf("QBBBBBBBQRMgZEbbdMB iBBBBB: SuvLvvvsJI2B\n");
		printf("BBBBBQBBBBBBBBBBBBB BBBBBBB BBBBBBBBBBBB\n");
		printf("QBBBQBBBBBBBBBBBBB :BBBBBBB rBBBBBBBQBBB\n");
		printf("BBBBBBBBBBBBBBBBBi BBQBBBQB .BBBBQBBBBBB\n");
		printf("QBBBQBBBBBBBBBBBb QBBBBBQBB .BQBBBBBQBBB\n");
		printf("BBBBBBBBBBBQBBBB 1BQBBBQBBB 7BBBBBBBBBBB\n");
		printf("QBBBBBBBBBBBBBB rBQBBBBBBBB RBBBBBBBBBBB\n");
		printf("BQBBBBBQBBBBBq 1BQBQBQBBBBJ BBBBBBBBBBBB\n");
		printf("QBBBQBBBQBBBBI gBBBBBBQBBB 7QBQBBBBBBBBB\n");
		printf("BBBBBBBBBBBBBI   :gBBBBBBK BBBBQBBBBBBBB\n");
		printf("QBBBBBQBBBQB5 BBP.   .iX1 vBQBBBQBBBBBBB\n");
		printf("BBBQBBBBBBBg dBBBBBgj:.  iBBBBBBBBBBBQBB\n");
		printf("QBQBBBBBBBB LBBBQBBBBBB vBBBBBBBBBBBBBBB\n");
		printf("BBBBBBBBBQ. BBBBBBBBBBB iQBBBBBBBQBBBQBB\n");
		printf("QBBBBBBBBB BBBQBQBBBQBB rBBBBBQBBBBBQBBB\n");

		milli_delay(ANIMATION_DELAY_TIME*3);	
		clear_screen(&(console_table[0]));
		printf("BBBBBBBBBBBBBBBQBBQLi::iIBBBBBBQBBBQBBBB\n");
		printf("QBBBBBBBBBBBBBBr.:.vKDbs. vBBBBBBBBBBBBB\n");
		printf("BBBBBBBBBBBBBBBg UBBBBBBBQ iBBBQBBBBBQBB\n");
		printf("QBBBBBQBBBBBBBQ IBEBBBBBZBB BBBBBBQBQBQB\n");
		printf("BBBBBBBBBBBBBBQ BX BBBBB QB 5BBQBBBBBBBB\n");
		printf("QBBBBBBBBBBBBBB DBZBLrdBEBB QBBBBBQBBBBB\n");
		printf("BBBQBQBBBBBQBBB1 BBBKvRBBB:1BQBBBBBBBBBB\n");
		printf("QBBBBBB..i:::iiv  .UDgq27  MIjvL7v77rr B\n");
		printf("BQBBBBBES5X5KSKqQMQi 7r7dQ. 1vLvYYJLJLYB\n");
		printf("QBBBBBBBBBBBQBBBBBB 1BBBBBQ BBBBBBBBQBBB\n");
		printf("BBBBBBBBBBBBBBBBBBs BBBQBBB PBBQBBBBBBBB\n");
		printf("QBBBBBBBQBBBBBBBBB gQBBBQBQ.iBBBBBBBBBBB\n");
		printf("BBBBBBBBBBBQBBBBB vBBBBQBBB.:BBBBBBBBBBB\n");
		printf("QBBBBBQBBBBBBBBg rBBBBBBBBB.:BBBQBBBBBBB\n");
		printf("BBBBBBBBBQBBBQL qBBBBBBBBBB UBBBBBBBBBBQ\n");
		printf("QBBBBBBBBBBBBI gBBBBBBBBBBR BBBBBBBBBBBB\n");
		printf("BBBBBBBQBE7...r :DBBBBBBBQ .BBBBBBBBBBBB\n");
		printf("QBBBBBBBg vPBBBBv  iYPgBBL BBBBBBBQBBBBB\n");
		printf("BBBBBQBBP BQBBBBBBQJr::   QBBBBQBBBQBBBB\n");
		printf("QBBBBBBB5 BBBBBBBBQBBBB.rBBBBBBBBBBBBBBB\n");
		printf("BBBBBBBBu BBBBBQBBBBBBB 7QBBBQBQBBBBBBBB\n");
		printf("QBBBBBBB. BBBBBBBBBBQBQ 7BBBBBQBBBQBBBBB\n");
		printf("BBBBBBBD uBBBBBBBBBBBBB 5BBBBBBBBBBBBBBB\n");
		printf("BBBBBBBBBBBBBBBBBBQBBBB DBBBBBQBBBBBQBQB\n");
		milli_delay(ANIMATION_DELAY_TIME);	
		clear_screen(&(console_table[0]));
		printf("QBBBBBBBQBBBBBBY.. .iRBBQBQBBBBBBBQBBBQB\n");
		printf("BBBBBBBBBBg:B. :UPE5i .BBBBQBBBBBBBBBBBB\n");
		printf("QBBBBBQBQBBr .BBBBBBBB  BBBBBBBBBBBBBBBB\n");
		printf("BQBBBBBBBBBr BEBBQBQBBB RBBBBBBQBBBBBBBB\n");
		printf("QBBBBBBBBBB KB gBBQB 7Q. BQBBBBBBBQBBBBB\n");
		printf("BQBBBBBBBBBi.BXBgjdBXBB 7 KQBBBBBBBBBBBB\n");
		printf("QBB5.ir7rvJL JBB5.7BBB  J  .i:...... 7BB\n");
		printf("BBBD7vvr7r7Y1  vQBBg1 :BgMB iDdKqqggMBBB\n");
		printf("QBBBBBBBQBBBBB1: ... dBBQBBR BBBQBBBBBBB\n");
		printf("BBBBBBBBBQBQBBBBBBBr BBBBQBB XBBBBBBBBBB\n");
		printf("QBBBBBBBQBBBBBBBQBq BBBBQBBB:7BBBBBBBBBB\n");
		printf("BBBBBQBBBBBQBBBBBY PBBBBBQBB.7BBBBBBBQBQ\n");
		printf("QBBBQBQBBB2:SBBB  RBBBBBQBBB QBBQBBBBBBB\n");
		printf("BBBBBQBBBB  :   jBBBBBBQBBBs.QBBBBBBBBBQ\n");
		printf("QBBBBBBBQB BBB: BBBBBBBBBBB QBQBBBBBBBBB\n");
		printf("BQBBBBBBBB BBBB7 rBBBBBBBB LBBBQBBBBBBBB\n");
		printf("QBBBQBBBQg BBBQBQ.  .rJgS iBQBBBBBQBBBBB\n");
		printf("BBBQBBBBB: BBBBBBBBEu77  5BBBBBQBBBQBBBB\n");
		printf("QBBBBBBBu 7BBBBBBBQBBBB SBBBBBBBBBBBBBBB\n");
		printf("BBBBBBBQBBBBBBBBBBBBBBB JBBBBBBBBBBBBBBB\n");
		printf("QBBBBBBBBBBBQBBBBBBBBBB uBBBBBQBBBQBBBBB\n");
		printf("BBBBBBBBBQBBBBBBBBBBBBB XBBBBBBBBQBBBBBB\n");
		printf("QBBBQBBBBBBBBBBBBBBBBBB gBBBBBBBQBQBBBBB\n");
		milli_delay(ANIMATION_DELAY_TIME);	
		clear_screen(&(console_table[0]));
		printf("BBBBBBBBBBBB2      vBBBBBBBQBBBBBBBBBBBB\n");
		printf("QBBBBBBBR :  5BBBBP  QBBBBQBBBBBBBQBQBQB\n");
		printf("BBBBBBBBBB YBBBQBBBB7 2BBBBBBBBBBBBBBQBB\n");
		printf("QBBBBBBBB: B JBBBBvrB  .:RBBBBBBBBBBBBBB\n");
		printf("BBqKRgRMB iQ sBBBBi Br.B   qUv77rrr: RBB\n");
		printf("QBi ...... QBBg  BBBB vRs2: 7sv77v777BBB\n");
		printf("BBBQBQBBBBK :ZBBBBBX :BBBBBi.QBBBBBBBQBB\n");
		printf("QBBBBBBBQBBBJ. .... rBBBBBBB PBBBBBBBBBB\n");
		printf("BBBBBBBBBBBBPBBBBBB gBBBBBBB .BBBBBBBBBB\n");
		printf("QBBBQBBBBBQq  :BBB .BBBBBBBB .QBBBBBBBQB\n");
		printf("BBBQBBBQBQB 7B. r iBBBBQBBBB 2BBBBBBBBBB\n");
		printf("QBBBQBBBBBB BBB iBBBBBBBBBBs BBBBBBBBBBB\n");
		printf("BBBBBBBBBB: BBB .BBBBBBBBBD DBBBBBBQBQBQ\n");
		printf("QBBBBBBBBB 2BBBB  IBBBBBBu IBBBBBBBBQBBB\n");
		printf("BQBBBBBQB  BBBBBBJ.  .ir. gBBQBBBBBBBBBB\n");
		printf("QBBBBBBBBBBBBBBBBBBBQbP 7BQBBBBBBBBBBBQB\n");
		printf("BBBBBBBBBQBBBBBBBBBBBQB QQBBBBBBBBBBBBBB\n");
		printf("QBBBQBBBBBQBBBBBBBQBBBB DBBBBBBBBBBBBBBB\n");
		printf("BBBBBBBBBBBBBBBQBQBBBBB dBBQBQBBBBBQBBBB\n");
		printf("QBBBQBQBBBQBBBBBBBBBBBB SBBBBBBBBBBBBBBB\n");
		milli_delay(ANIMATION_DELAY_TIME);	
		clear_screen(&(console_table[0]));
		--i;
	}
	printf("--------");
	print_color_str(&(console_table[0])," Welcome to Poi OS ",YELLOW_CHAR);
	printf("--------\n");
	delay(5);
	printf(" _____   ____ _____ /\\/|\n");
	printf("|  __ \\ / __ \\_   _|/\\/\n");
	printf("| |__) | |  | || |\n");
	printf("|  ___/| |  | || |\n");
	printf("| |    | |__| || |\n");
	printf("|_|     \\____/_____|    \n\n");	
	printf("@ input");
	print_color_str(&(console_table[0])," help ",YELLOW_CHAR);
	printf("to get all commands @\n");
	
}

PRIVATE void divideOrder(char* input,char* goal)
{
	for(int i=0;i<STR_DEFAULT_LEN;i++)
	{
		if((input[i]) != ' ' && (input[i]) != "\0")
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
	int pid = 0;
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
			printf("| pid: %d ",proc_table[i].pid);
			printf("| prio: %d",proc_table[i].priority);
		}	
		for(int i=0;i<NR_PROCS;i++)
		{
			if(proc_table[i+NR_TASKS].p_flags != 0)
				continue;
			nameLen = strlen(proc_table[i+NR_TASKS].name);
			printf("\nprocess: %s ",proc_table[i+NR_TASKS].name);
			for(int j=0;j<6-nameLen;j++)
				printf(" ");
			printf("| pid: %d |",proc_table[i+NR_TASKS].pid);
			printf("| prio: %d | status: %d",proc_table[i+NR_TASKS].priority,proc_table[i+NR_TASKS].p_flags);
		}	
		printf("\n-------------------------\n");
		break;
	case BLOCK:
		pid = atoi(input + 8);
		printf("\ninput pid : %d",pid);
		if(pid < NR_TASKS)
		{
			printf("\nSystem Task cannot be blocked by users.");
			break;
		}
		proc_table[pid].p_flags = -1;
		proc_table[pid].ticks = 0;
		printf("\nprocess %d has been killed.",pid);
		break;
	case ACTIVATE:
		pid = atoi(input + 8);
		printf("\ninput pid : %d",pid);
		if(pid < NR_TASKS)
		{
			printf("\nSystem Task cannot be activated by users.");
			break;
		}
		proc_table[pid].p_flags = 0;
		printf("\nprocess %d has been activated.",pid);
		break;
	default:
		printf("\nmanage process failed. parameters error.");
	}
}

PRIVATE void processInputError(SHELLINFO* info)
{
	printf("\n");
	print_color_str(info->tty->p_console,"Undefined Command.",YRED_CHAR);
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
	printf("\n");
}

PRIVATE int compareAndDo(char* input,SHELLINFO* info)
{
	MESSAGE msg;
	char mtTest[STR_DEFAULT_LEN];
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
			print_color_str(info->tty->p_console,"help",GREEN_CHAR);
			printf(" . . . . . . | list what you could do\n");
			print_color_str(info->tty->p_console,"clear",GREEN_CHAR);
			printf(" . . . . . .| clear your screen\n");
			print_color_str(info->tty->p_console,"proc",GREEN_CHAR);
			print_color_str(info->tty->p_console," [-options]",YELLOW_CHAR);
			printf("  | manage all process\n");
			print_color_str(info->tty->p_console,"poi",GREEN_CHAR);
			printf(" . . . . . . .| correct last input error\n");
			print_color_str(info->tty->p_console,"game",GREEN_CHAR);
			print_color_str(info->tty->p_console," [-options]",YELLOW_CHAR);
			printf("  | to play games\n");
			print_color_str(info->tty->p_console,"time",GREEN_CHAR);
			printf(" . . . . . . | to get current rtc time\n");
			print_color_str(info->tty->p_console,"debug",GREEN_CHAR);
			printf(" . . . . . .| to switch debug mode\n");
			print_color_str(info->tty->p_console,"sche",GREEN_CHAR);
			print_color_str(info->tty->p_console," [-options]",YELLOW_CHAR);
			printf("  | to switch schedule mode\n");
			print_color_str(info->tty->p_console,"shutdown",GREEN_CHAR);
			printf(" . . . . | to exit\n");
			break;
		}
		if(strcmp(info->currentOrder,PROC) == 1)
		{
			manageProcess(input);
			break;
		}
		if(strcmp(info->currentOrder,GAME) == 1)
		{
			if(input[6] == '2')
			{
				printf("\nPress Ctrl+F3 to play 2048.");
				while(1)
				{
					msg.type = 0;
					send_recv(SEND,TASK_TTY,&msg);
					reset_msg(&msg);
					send_recv(RECEIVE,ANY,&msg);
					
					divideOrder(msg.INSSM,mtTest);
					if(strcmp(mtTest,"w\0") == 1)
						msg.RETVAL = GUP;
					if(strcmp(mtTest,"s\0") == 1)
						msg.RETVAL = GDOWN;
					if(strcmp(mtTest,"a\0") == 1)
						msg.RETVAL = GLEFT;
					if(strcmp(mtTest,"d\0") == 1)
						msg.RETVAL = GRIGHT;
					if(strcmp(mtTest,"l\0") == 1)
					{
						msg.RETVAL = ENDGAME;
						send_recv(SEND,GPOW,&msg);
						return TTY_DO_INDEX;
					}
					send_recv(SEND,GPOW,&msg);
				}
				break;
			}
			else
			{
				printf("\nPress Ctrl+F2 to play ticks.");
			while(1)
			{
				msg.type = 0;
				send_recv(SEND,TASK_TTY,&msg);
				reset_msg(&msg);
				send_recv(RECEIVE,ANY,&msg);
				
				divideOrder(msg.INSSM,mtTest);
				if(strcmp(mtTest,"a\0") == 1)
					msg.RETVAL = GUP;
				if(strcmp(mtTest,"r\0") == 1)
					msg.RETVAL = GDOWN;
				if(strcmp(mtTest,"l\0") == 1)
				{
					msg.RETVAL = ENDGAME;
					send_recv(SEND,GTICKS,&msg);
					return TTY_DO_INDEX;
				}
				send_recv(SEND,GTICKS,&msg);
			}
			break;
			}
		}
		if(strcmp(info->currentOrder,SHUTDOWN) == 1)
		{
			printf("\nPOI OS is shutting down...");
			delay(100);
			clear_screen(info->tty->p_console);
			clear_screen(&(console_table[1]));
			clear_screen(&(console_table[2]));
			clear_init_screen(info->tty->p_console);
			//关中断
			disable_int();
			for(int i = 0;i < NR_IRQ;i++)
				disable_irq(i);
			break;
		}
		if(strcmp(info->currentOrder,TIME_ORDER) == 1)
		{
			int value = rtcTime();
			printf("\nfunc out :%d",value);
			printf("\ntime infomation in RTC: %x.%x.%x %x:%x\n",timeData->year,timeData->month,timeData->day,timeData->hour,timeData->minute);
			break;
		}
		if(strcmp(info->currentOrder,IFDEBUG) == 1)
		{
			ifDebug = (ifDebug == 1) ? 0:1;
			printf("now debug switch is %d",ifDebug);
			break;
		}
		if(strcmp(info->currentOrder,SCHEDULE) == 1)
		{
			whichSchedule = input[8] - '0';
			printf("\n Schedule Algorithm has switched.\n");
			break;
		}
		//未匹配到的指令
		if(strcmp(info->currentOrder,POI) == 1)
		{
			if(info->ifError == 1)
			{
				strcpy(input,info->lastInput);
				info->ifError = 0;
				continue;
			}
			printf("\n");
			print_color_str(info->tty->p_console,"poi~ ",RED_CHAR);
			print_color_str(info->tty->p_console,"poi~ ",BLUE_CHAR);
			print_color_str(info->tty->p_console,"poi~ ",GREEN_CHAR);
			printf("\n");
			break;			
		}
		strcpy(info->lastInput,input);
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
	strcpy(info->allOrder[1],HELP);
	strcpy(info->allOrder[2],CLEAR);
	strcpy(info->allOrder[3],PROC);
	strcpy(info->allOrder[4],POI);
	strcpy(info->allOrder[5],GAME);
	strcpy(info->allOrder[6],SHUTDOWN);
	strcpy(info->allOrder[7],TIME_ORDER);
	strcpy(info->allOrder[8],SCHEDULE);
}

PUBLIC void task_shell()
{
	SHELLINFO info;
	init_info(&info);

	print_welcome();

	delay(1);
	MESSAGE msg;
	reset_msg(&msg);
	int seed = 65536;
	unsigned int kill = 0;
	while(1){
		send_recv(RECEIVE,ANY,&msg);
		info.tty = (TTY*)(msg.INTTY);
		//printf("\nShabby Input : %s",msg.INSSM);
		divideOrder(msg.INSSM,info.currentOrder);
		// seed = (1103515245*seed + 12345) % 4294967296;
		// kill = seed % 3 + 2; 
		// printf(" | length: %d | order %s | rand:%d",strlen(msg.INSSM),info.currentOrder,kill);
		delay(1);
			
		int outType = compareAndDo(msg.INSSM,&info);
		
		strcpy(msg.INSSM,"");
		reset_msg(&msg);
		msg.type = outType;
		
		send_recv(SEND,TASK_TTY,&msg);
	}
}
