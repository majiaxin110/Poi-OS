
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            global.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define GLOBAL_VARIABLES_HERE

#include "type.h"
#include "const.h"
#include "protect.h"
#include "tty.h"
#include "shell.h"
#include "console.h"
#include "proc.h"
#include "global.h"
#include "proto.h"


PUBLIC	struct proc	proc_table[NR_TASKS + NR_PROCS];

PUBLIC	struct task	task_table[NR_TASKS] = {
	{task_tty, STACK_SIZE_TTY, "TTY"},
	{task_sys, STACK_SIZE_SYS, "SYS"},
	{task_shell,STACK_SIZE_SHELL, "SHELL"},
	{Gpow, STACK_SIZE_GPOW, "GPOW"},
	{Gticks, STACK_SIZE_GTICKS, "GTICKS"},
	{task_tick, STACK_SIZE_TICK, "TT"}};

PUBLIC	struct task	user_proc_table[NR_PROCS] = {
	{TestA, STACK_SIZE_TESTA, "TestA"},
	{TestB, STACK_SIZE_TESTB, "TestB"},
	{TestC, STACK_SIZE_TESTC, "TestC"}};


PUBLIC	char		task_stack[STACK_SIZE_TOTAL];

PUBLIC	TTY		tty_table[NR_CONSOLES];
PUBLIC	CONSOLE		console_table[NR_CONSOLES];


PUBLIC	irq_handler	irq_table[NR_IRQ];

PUBLIC	system_call	sys_call_table[NR_SYS_CALL] = {sys_printx,
						       sys_sendrec,sys_getTime};

