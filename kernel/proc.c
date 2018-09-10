
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                               proc.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "type.h"
#include "const.h"
#include "protect.h"
#include "tty.h"
#include "console.h"
#include "string.h"
#include "proc.h"
#include "global.h"
#include "proto.h"
#include "schedule_queue.h"

#define OUT_QUEUE -100

PRIVATE void block(struct proc* p);
PRIVATE void unblock(struct proc* p);
PRIVATE int  msg_send(struct proc* current, int dest, MESSAGE* m);
PRIVATE int  msg_receive(struct proc* current, int src, MESSAGE* m);
PRIVATE int  deadlock(int src, int dest);

PRIVATE int proc_status_before[NR_TASKS+NR_PROCS];
PRIVATE int proc_status_current[NR_TASKS+NR_PROCS];
PUBLIC int block_schedule = 0;

/*****************************************************************************
 *                                schedule
 *****************************************************************************/
/**
 * <Ring 0> Choose one proc to run.
 * 
 *****************************************************************************/

PUBLIC struct proc* get_last_proc(struct proc* head)
{
	struct proc* current = head;
	if(head == NULL){
		return NULL;
	}

	while(current->next_proc != NULL){
		current++;
		if(current->next_proc == NULL){
			return current;
		}
	}
	return current;
}

PUBLIC void add_to_queue(struct proc* current, struct Queue* queue){
	struct proc* p = get_last_proc(queue->linked_PCB);

	if(p == NULL){
		queue->linked_PCB = current;
	} else {
		p->next_proc = current;
	}

	// for(p = &FIRST_PROC; p <= &LAST_PROC; p++){
	// 	printf("    %s   XXXXXXXXXXXXX", p->name);
	// }

	current->already_run_for = 0;			
	current->next_proc = NULL;
}

PUBLIC void init_schedule_queue()
{
	Queue1.next = &Queue2;	//将3个队列连接起来
	Queue2.next = &Queue3;
	Queue3.next = NULL;

	Queue1.prio = 8;		//设定每个队列的时间片
	Queue2.prio = 16;
	Queue3.prio = 32;

	Queue1.linked_PCB = Queue2.linked_PCB = Queue3.linked_PCB = NULL;

	int i = 0;
	for(i = 0; i < NR_PROCS + NR_TASKS; i++){
		// proc_table[i].ticks = proc_table[i].priority;
		proc_table[i].next_proc = NULL;
		proc_table[i].already_run_for = 0;

		proc_status_current[i] = proc_table[i].p_flags;
		proc_status_before[i] = 0;
		if(proc_table[i].p_flags == 0){		//当前进程未被阻塞，将其加入Queue1
			add_to_queue(&proc_table[i], &Queue1);
		}
	}
}

PUBLIC void mqs_schedule()
{

	struct proc*	p;
	int		greatest_ticks = 0;
	int flags = 0;
	//分别处理三个队列
	//任务队列
	for(int i=0;i < NR_TASKS;i++)
	{
		if(proc_table[i].p_flags != 0 && task_queue[i] != OUT_QUEUE)
		//移出队列
		{
			for(int j=0;j<NR_TASKS + NR_PROCS;j++)
			{
				if(task_queue[j] == i)
				{
					task_queue[j] = task_queue[i];
					if(ifDebug == 1)
					{
						print_color_str(&(console_table[0]),"REMOVE:",YELLOW_CHAR);
						out_char(&(console_table[0]),'0' + i);
						print_color_str(&(console_table[0]),"FROM:",RED_CHAR);
						out_char(&(console_table[0]),'0' + j);
					}
					
					break;
				}
			}
			task_queue[i] = OUT_QUEUE;
			
		}
		if(proc_table[i].p_flags == 0 && task_queue[i] < 0)
		//加入队列
		{
			for(int j=0;j<NR_TASKS + NR_PROCS;j++)
			{
				if(task_queue[j] != OUT_QUEUE)
				{
					task_queue[i] = task_queue[j];
					flags = 1;
					task_queue[j] = i;
					break;
				}
			}
			if(!flags)
				task_queue[i] = 0;
			flags = 0;
			if(ifDebug == 1)
			{
				print_color_str(&(console_table[0]),"ADD:",BLUE_CHAR);
				out_char(&(console_table[0]),'0' + i);
			}
				
		}
	}
	//队列2,队列2目前固定只有一个进程
	if(proc_table[NR_TASKS].p_flags != 0)
	{
		proc_queueA[NR_TASKS] = OUT_QUEUE;
		if(fcfsHead == NR_TASKS)
			fcfsHead = proc_queueA[NR_TASKS];
	}	
	if(proc_table[NR_TASKS].p_flags == 0)
		proc_queueA[NR_TASKS] = 0;
	//队列3
	for (int i = NR_TASKS+1;i<NR_TASKS + NR_PROCS;i++) 
	{
		if(proc_table[i].p_flags != 0 && proc_queueB[i] != OUT_QUEUE)
		//移出队列
		{
			/*for(int j=0;j<NR_TASKS + NR_PROCS;j++)
			{
				if(proc_queueB[j] == i)
				{
					proc_queueB[j] = proc_queueB[i];
					if(ifDebug == 1)
					{
						print_color_str(&(console_table[0]),"REMOVE:",YELLOW_CHAR);
						out_char(&(console_table[0]),'0' + i);
						print_color_str(&(console_table[0]),"FROM:",RED_CHAR);
						out_char(&(console_table[0]),'0' + j);
					}
					break;
				}
			}
			proc_queueB[i] = OUT_QUEUE;*/
			if(fcfsHead == i)//在队头的情况
			{
				fcfsHead = proc_queueB[i];
				// print_color_str(&(console_table[0]),"!!| ",RED_CHAR);
				// out_char(&(console_table[0]),'0' + fcfsHead);
				proc_queueB[i] = OUT_QUEUE;
			}
			else
			{
				//在队尾或队中的情况
				for(int j=NR_TASKS;j<NR_TASKS + NR_PROCS;j++)
				{
					if(proc_queueB[j] == i)
					{					
						proc_queueB[j] = proc_queueB[i];
						if(fcfsTail == i)
							fcfsTail = j;
						proc_queueB[i] = OUT_QUEUE;
						break;
					}else if(j == NR_TASKS + NR_PROCS -1)
					{
						//最后一个进程被移除
						fcfsHead = fcfsTail = OUT_QUEUE;
						proc_queueB[i] = OUT_QUEUE;
					}
				}
				
			}
			
		}
		if(proc_table[i].p_flags == 0 && proc_queueB[i] == OUT_QUEUE)
		//加入队列
		{
			/*for(int j=NR_TASKS;j<NR_TASKS + NR_PROCS;j++)
			{
				if(proc_queueB[j] != OUT_QUEUE)
				{
					proc_queueB[i] = proc_queueB[j];
					proc_queueB[j] = i;
					flags = 1;
					break;
				}
			}
			//当前队列唯一进程
			if(!flags)
				proc_queueB[i] = 1;
			if(ifDebug == 1)
			{
				print_color_str(&(console_table[0]),"ADD:",BLUE_CHAR);
				out_char(&(console_table[0]),'0' + i);
			}*/
			if(fcfsTail != OUT_QUEUE)
			{
				proc_queueB[fcfsTail] = i;
				fcfsTail = i;
			}
			else
			{
				fcfsHead = fcfsTail = OUT_QUEUE;
			}
		}
	}

	// for(int i=0;i<NR_TASKS + NR_PROCS;i++)
	// {
	// 	if((proc_queueB[i] != OUT_QUEUE && proc_table[i].p_flags != 0) || proc_queueB[i] >= NR_TASKS + NR_PROCS)
	// 	{
	// 		print_color_str(&(console_table[0]),"WARNING:",YELLOW_CHAR);
	// 		out_char(&(console_table[0]),'0' + i);
	// 	}
	// }
	// while (!greatest_ticks) {
	// 	for (p = &FIRST_PROC; p <= &LAST_PROC; p++) {
	// 		if (p->p_flags == 0) {
	// 			if (p->ticks > greatest_ticks) {
	// 				greatest_ticks = p->ticks;
	// 				p_proc_ready = p;
	// 			}
	// 		}
	// 	}

	// 	if (!greatest_ticks)
	// 		for (p = &FIRST_PROC; p <= &LAST_PROC; p++)
	// 			if (p->p_flags == 0)
	// 				p->ticks = p->priority;
	// }
	// int k;
	// while(!greatest_ticks)
	// {
	// 	for(int i=0;i<NR_TASKS + NR_PROCS;i++)
	// 	{
	// 		if(proc_queue[i] != OUT_QUEUE)
	// 		{
	// 			k = i;
	// 			break;
	// 		}
	// 	}
	// 	for(int i=k;i<9 && proc_queue[i] != OUT_QUEUE;i = proc_queue[i])
	// 	{
	// 		if(proc_table[i].ticks > greatest_ticks)
	// 		{
	// 			greatest_ticks = proc_table[i].ticks;
	// 			p_proc_ready = &(proc_table[i]);
	// 		}
	// 		if(proc_table[i].p_flags != 0)
	// 			print_color_str(&(console_table[0]),"WARNING:",YELLOW_CHAR);
	// 	}

	// 	if(!greatest_ticks)
	// 	{
	// 		for(int i=0;i<NR_TASKS + NR_PROCS;i++)
	// 		{
	// 			if(proc_queue[i] != OUT_QUEUE)
	// 			{
	// 				k = i;
	// 				break;
	// 			}
	// 		}
	// 		for(int i=k;i<9 && proc_queue[i] != OUT_QUEUE;i = proc_queue[i])
	// 			proc_table[i].ticks = proc_table[i].priority;
	// 	}
	// }
	int i;
	while (!greatest_ticks) {
		for (i=0,p = &FIRST_PROC; p <= &LAST_PROC && i<NR_TASKS + NR_PROCS; p++,i++) {
			if((i<NR_TASKS && task_queue[i] == OUT_QUEUE) || (i == NR_TASKS && proc_queueA[i] == OUT_QUEUE) 
				|| (i>NR_TASKS && proc_queueB[i] == OUT_QUEUE) )
				continue;
			if (p->p_flags == 0) {
				if (p->ticks > greatest_ticks && i<NR_TASKS) {
					greatest_ticks = p->ticks;
					p_proc_ready = p;
				}
				if(i<NR_TASKS && greatest_ticks > 0)
					break;
				// print_color_str(&(console_table[0]),"ffSuccess",GREEN_CHAR);
				//进入非任务队列时间
				//非任务队列采用FCFS
				//A队列分给应有时间
				if(i >= NR_TASKS && i<NR_TASKS + NR_APROCQUEUE && changeProAFlag == 0) 
				{
					greatest_ticks = p->ticks;
					p_proc_ready = p;
					changeProAFlag =1;
				}
				//B队列分给应有时间
				else if(fcfsHead == i && p->ticks != 0 && changeProBFlag == 0)
				{
					greatest_ticks = p->ticks;
					p_proc_ready = p;
					changeProBFlag = 1;
				}
			}
			else
			{
				print_color_str(&(console_table[0]),"WARNING:",YELLOW_CHAR);
				out_char(&(console_table[0]),'0' + i);
			}
		}
		if (!greatest_ticks && i>=NR_TASKS)
		{
			for (p = &FIRST_PROC; p <= &LAST_PROC; p++)
				if (p->p_flags == 0)
					p->ticks = p->priority;
			changeProAFlag = changeProBFlag = 0;
		}
			
	}



}

PUBLIC void mrc_schedule()
  {
  struct proc* current = Queue1.linked_PCB;
	struct proc* p_temp = p_proc_ready;
	int done = False;
	int i = 0;
	int j = 0;

	int is_head = True;
	struct proc* p;

	for(i = 0; i < NR_PROCS + NR_TASKS; i++){
		proc_status_current[i] = proc_table[i].p_flags;								//填充当前进程状态表

		if((proc_status_before[i] != 0) && (proc_status_current[i] == 0)){			//有进程刚刚就绪……	
			add_to_queue(&proc_table[i], &Queue1);
			proc_status_before[i] = 0;
			proc_table[i].ticks = proc_table[i].priority;								//将其加入Q1
		} else if ((proc_status_before[i] == 0) && (proc_status_current[i] != 0)){		//有进程刚刚被阻塞
			struct proc* p_left = NULL;
			struct proc* p_right = proc_table[i].next_proc;
			
			proc_status_before[i] = 1;

			int faded = True;
			if(Queue1.linked_PCB == &proc_table[i]){
				Queue1.linked_PCB = p_right;
				faded = False;

			}else if(Queue2.linked_PCB == &proc_table[i]){
				Queue2.linked_PCB = p_right;
				faded = False;

			}else if(Queue3.linked_PCB == &proc_table[i]){
				Queue3.linked_PCB = p_right;
				faded = False;

			}else{			
				for(j = 0; j < NR_PROCS + NR_TASKS; j++){								//找到该进程之前的进程
					if(proc_table[j].next_proc == &proc_table[i]){
						p_left = &proc_table[j];
						faded = False;
						break;
					}
				}
				p_left->next_proc = p_right;			//将其从队列中移除
			}		

			// assert(faded == False);
			proc_table[i].already_run_for = 0;
			proc_table[i].next_proc = NULL;
			proc_table[i].ticks = proc_table[i].priority;
		}
	}

	for(p = Queue1.linked_PCB; p != NULL; p = p->next_proc){
		assert(p->p_flags == 0);
	}
	for(p = Queue2.linked_PCB; p != NULL; p = p->next_proc){
		// assert(p->p_flags == 0);
	}
	for(p = Queue3.linked_PCB; p != NULL; p = p->next_proc){
		assert(p->p_flags == 0);
	}

	current = Queue1.linked_PCB;
	if(current != NULL){
		if(current->already_run_for >= Queue1.prio){	//如果Q1的第一个进程时间片已到……
			if(current->next_proc != NULL){				//且有下一个进程的话
				Queue1.linked_PCB = current->next_proc;
				p_proc_ready = current->next_proc;
				// assert(p_proc_ready->p_flags == 0);

				add_to_queue(current, &Queue3);			//将该进程加入Q2
				// assert(current->p_flags == 0);
				done = True;
			} else {
				add_to_queue(current, &Queue3);			//将该进程加入Q2
				// assert(current->p_flags == 0);
				done = False;
			}
		} else {
			if(current->ticks <= 0){					//该进程已经运行完成
				if(current->next_proc != NULL){			//该进程在队列中还有后续进程
					Queue1.linked_PCB = current->next_proc;
					current->already_run_for = 0;					//重设该进程的时间片
					current->next_proc = NULL;

					p_proc_ready = Queue1.linked_PCB;
					// assert(p_proc_ready->p_flags == 0);
					done = True;
				} else {
					current = NULL;						
					done = False;
				}
				
			} else {
				p_proc_ready = current;
				// assert(p_proc_ready->p_flags == 0);
				done = True;
			}
		}
	}

	if((done == False) && (Queue2.linked_PCB != NULL)){
		current = Queue2.linked_PCB;

		if(current->already_run_for >= Queue2.prio){	//如果Q2的第一个进程时间片已到……
			if(current->next_proc != NULL){				//且有下一个进程的话
				Queue2.linked_PCB = current->next_proc;
				p_proc_ready = current->next_proc;
				// assert(p_proc_ready->p_flags == 0);

				add_to_queue(current, &Queue3);			//将该进程加入Q3
				// assert(current->p_flags == 0);
				done = True;
			} else {
				add_to_queue(current, &Queue3);			//将该进程加入Q3
				// assert(current->p_flags == 0);
			}
		} else {
			if(current->ticks <= 0){					//该进程已经运行完成
				if(current->next_proc != NULL){			//该进程在队列中还有后续进程
					Queue2.linked_PCB = current->next_proc;
					current->already_run_for = 0;			//重设该进程的时间片
					current->next_proc = NULL;

					p_proc_ready = Queue2.linked_PCB;
					// assert(p_proc_ready->p_flags == 0);
					done = True;
				} else {
					current = NULL;
					done = False;
				}
				
			} else {
				p_proc_ready = current;
				// assert(p_proc_ready->p_flags == 0);
				done = True;
			}
		}
	}

	if((done == False) && (Queue3.linked_PCB != NULL)){
		current = Queue3.linked_PCB;
		// assert(current->p_flags == 0);
		if(current->p_flags != 0){
			current = current->next_proc;
		}

		if(current != NULL)
		{
			if(current->ticks > 0){			//当前进程尚未运行完
				p_proc_ready = current;
				// assert(p_proc_ready->p_flags == 0);
				done = True;
			} else {
				if(current->next_proc != NULL){		//如果已经运行完
					Queue3.linked_PCB = current->next_proc;
					current->already_run_for = 0;
					current->next_proc = NULL;
					init_schedule_queue();
				}
				// assert(p_proc_ready->p_flags == 0);
				done = True;
			}
		} else {
			init_schedule_queue();
			// assert(p_proc_ready->p_flags == 0);
			done = True;
		}
	}

	if(block_schedule == 0){
		if(p_temp->ticks > 0){
			p_proc_ready = p_temp;
			return;
		}
	}

	int	greatest_ticks = 0;
	
	while (!greatest_ticks) {
		for (p = &FIRST_PROC; p <= &LAST_PROC; p++) {
			if (p->p_flags == 0) {
				if (p->ticks > greatest_ticks) {
					greatest_ticks = p->ticks;
					p_proc_ready = p;
				}
			}
		}

		if (!greatest_ticks)
			for (p = &FIRST_PROC; p <= &LAST_PROC; p++)
				if (p->p_flags == 0)
					p->ticks = p->priority;
	}
  	int all_done = True;

	for(i = 0; i < NR_PROCS + NR_TASKS; i++){
		proc_status_before[i] = proc_status_current[i];

		if((proc_table[i].ticks > 0) && (proc_table[i].p_flags == 0)){
			all_done = False;
			// out_char(&(console_table[0]), ']');
		}
	}

	if(all_done == True){
		for(i = 0; i < NR_PROCS + NR_TASKS; i++){
			if(proc_table[i].p_flags == 0){
				// proc_table[i].ticks = proc_table[i].priority;
				proc_table[i].already_run_for = 0;
			}
		}
	}
  }

PUBLIC void rr_schedule()
{
	struct proc*	p;
	int		greatest_ticks = 0;

	while (!greatest_ticks) {
		for (p = &FIRST_PROC; p <= &LAST_PROC; p++) {
			if (p->p_flags == 0) {
				if (p->ticks > greatest_ticks) {
					greatest_ticks = p->ticks;
					p_proc_ready = p;
				}
			}
		}

		if (!greatest_ticks)
			for (p = &FIRST_PROC; p <= &LAST_PROC; p++)
				if (p->p_flags == 0)
					p->ticks = p->priority;
	}
}
/*****************************************************************************
 *                                sys_sendrec
 *****************************************************************************/
/**
 * <Ring 0> The core routine of system call `sendrec()'.
 * 
 * @param function SEND or RECEIVE
 * @param src_dest To/From whom the message is transferred.
 * @param m        Ptr to the MESSAGE body.
 * @param p        The caller proc.
 * 
 * @return Zero if success.
 *****************************************************************************/
PUBLIC int sys_sendrec(int function, int src_dest, MESSAGE* m, struct proc* p)
{
	assert(k_reenter == 0);	/* make sure we are not in ring0 */
	assert((src_dest >= 0 && src_dest < NR_TASKS + NR_PROCS) ||
	       src_dest == ANY ||
	       src_dest == INTERRUPT);

	int ret = 0;
	int caller = proc2pid(p);
	MESSAGE* mla = (MESSAGE*)va2la(caller, m);
	mla->source = caller;

	assert(mla->source != src_dest);

	/**
	 * Actually we have the third message type: BOTH. However, it is not
	 * allowed to be passed to the kernel directly. Kernel doesn't know
	 * it at all. It is transformed into a SEND followed by a RECEIVE
	 * by `send_recv()'.
	 */
	if (function == SEND) {
		ret = msg_send(p, src_dest, m);
		if (ret != 0)
			return ret;
	}
	else if (function == RECEIVE) {
		ret = msg_receive(p, src_dest, m);
		if (ret != 0)
			return ret;
	}
	else {
		panic("{sys_sendrec} invalid function: "
		      "%d (SEND:%d, RECEIVE:%d).", function, SEND, RECEIVE);
	}

	return 0;
}

/*****************************************************************************
 *                                send_recv
 *****************************************************************************/
/**
 * <Ring 1~3> IPC syscall.
 *
 * It is an encapsulation of `sendrec',
 * invoking `sendrec' directly should be avoided
 *
 * @param function  SEND, RECEIVE or BOTH
 * @param src_dest  The caller's proc_nr
 * @param msg       Pointer to the MESSAGE struct
 * 
 * @return always 0.
 *****************************************************************************/
PUBLIC int send_recv(int function, int src_dest, MESSAGE* msg)//@param function  SEND, RECEIVE or BOTH @param src_dest  The caller's proc_nr @param msg Pointer to the MESSAGE struct
{
	int ret = 0;

	if (function == RECEIVE)
		memset(msg, 0, sizeof(MESSAGE));

	switch (function) {
	case BOTH:
		ret = sendrec(SEND, src_dest, msg);
		if (ret == 0)
			ret = sendrec(RECEIVE, src_dest, msg);
		break;
	case SEND:
	case RECEIVE:
		ret = sendrec(function, src_dest, msg);
		break;
	default:
		assert((function == BOTH) ||
		       (function == SEND) || (function == RECEIVE));
		break;
	}

	return ret;
}

/*****************************************************************************
 *				  ldt_seg_linear
 *****************************************************************************/
/**
 * <Ring 0~1> Calculate the linear address of a certain segment of a given
 * proc.
 * 
 * @param p   Whose (the proc ptr).
 * @param idx Which (one proc has more than one segments).
 * 
 * @return  The required linear address.
 *****************************************************************************/
PUBLIC int ldt_seg_linear(struct proc* p, int idx)
{
	struct descriptor * d = &p->ldts[idx];

	return d->base_high << 24 | d->base_mid << 16 | d->base_low;
}

/*****************************************************************************
 *				  va2la
 *****************************************************************************/
/**
 * <Ring 0~1> Virtual addr --> Linear addr.
 * 
 * @param pid  PID of the proc whose address is to be calculated.
 * @param va   Virtual address.
 * 
 * @return The linear address for the given virtual address.
 *****************************************************************************/
PUBLIC void* va2la(int pid, void* va)
{
	struct proc* p = &proc_table[pid];

	u32 seg_base = ldt_seg_linear(p, INDEX_LDT_RW);
	u32 la = seg_base + (u32)va;

	if (pid < NR_TASKS + NR_PROCS) {
		assert(la == (u32)va);
	}

	return (void*)la;
}

/*****************************************************************************
 *                                reset_msg
 *****************************************************************************/
/**
 * <Ring 0~3> Clear up a MESSAGE by setting each byte to 0.
 * 
 * @param p  The message to be cleared.
 *****************************************************************************/
PUBLIC void reset_msg(MESSAGE* p)
{
	memset(p, 0, sizeof(MESSAGE));
}

/*****************************************************************************
 *                                block
 *****************************************************************************/
/**
 * <Ring 0> This routine is called after `p_flags' has been set (!= 0), it
 * calls `schedule()' to choose another proc as the `proc_ready'.
 *
 * @attention This routine does not change `p_flags'. Make sure the `p_flags'
 * of the proc to be blocked has been set properly.
 * 
 * @param p The proc to be blocked.
 *****************************************************************************/
PRIVATE void block(struct proc* p)
{
	assert(p->p_flags);
	block_schedule = 1;
	p_schedule[whichSchedule]();
	block_schedule = 0;
}

/*****************************************************************************
 *                                unblock
 *****************************************************************************/
/**
 * <Ring 0> This is a dummy routine. It does nothing actually. When it is
 * called, the `p_flags' should have been cleared (== 0).
 * 
 * @param p The unblocked proc.
 *****************************************************************************/
PRIVATE void unblock(struct proc* p)
{
	assert(p->p_flags == 0);
}

/*****************************************************************************
 *                                deadlock
 *****************************************************************************/
/**
 * <Ring 0> Check whether it is safe to send a message from src to dest.
 * The routine will detect if the messaging graph contains a cycle. For
 * instance, if we have procs trying to send messages like this:
 * A -> B -> C -> A, then a deadlock occurs, because all of them will
 * wait forever. If no cycles detected, it is considered as safe.
 * 
 * @param src   Who wants to send message.
 * @param dest  To whom the message is sent.
 * 
 * @return Zero if success.
 *****************************************************************************/
PRIVATE int deadlock(int src, int dest)
{
	struct proc* p = proc_table + dest;
	while (1) {
		if (p->p_flags & SENDING) {
			if (p->p_sendto == src) {
				/* print the chain */
				p = proc_table + dest;
				printl("=_=%s", p->name);
				do {
					assert(p->p_msg);
					p = proc_table + p->p_sendto;
					printl("->%s", p->name);
				} while (p != proc_table + src);
				printl("=_=");

				return 1;
			}
			p = proc_table + p->p_sendto;
		}
		else {
			break;
		}
	}
	return 0;
}

/*****************************************************************************
 *                                msg_send
 *****************************************************************************/
/**
 * <Ring 0> Send a message to the dest proc. If dest is blocked waiting for
 * the message, copy the message to it and unblock dest. Otherwise the caller
 * will be blocked and appended to the dest's sending queue.
 * 
 * @param current  The caller, the sender.
 * @param dest     To whom the message is sent.
 * @param m        The message.
 * 
 * @return Zero if success.
 *****************************************************************************/
PRIVATE int msg_send(struct proc* current, int dest, MESSAGE* m)
{
	struct proc* sender = current;
	struct proc* p_dest = proc_table + dest; /* proc dest */

	assert(proc2pid(sender) != dest);

	/* check for deadlock here */
	if (deadlock(proc2pid(sender), dest)) {
		panic(">>DEADLOCK<< %s->%s", sender->name, p_dest->name);
	}

	if ((p_dest->p_flags & RECEIVING) && /* dest is waiting for the msg */
	    (p_dest->p_recvfrom == proc2pid(sender) ||
	     p_dest->p_recvfrom == ANY)) {
		assert(p_dest->p_msg);
		assert(m);

		phys_copy(va2la(dest, p_dest->p_msg),
			  va2la(proc2pid(sender), m),
			  sizeof(MESSAGE));
		p_dest->p_msg = 0;
		p_dest->p_flags &= ~RECEIVING; /* dest has received the msg */
		p_dest->p_recvfrom = NO_TASK;
		unblock(p_dest);

		assert(p_dest->p_flags == 0);
		assert(p_dest->p_msg == 0);
		assert(p_dest->p_recvfrom == NO_TASK);
		assert(p_dest->p_sendto == NO_TASK);
		assert(sender->p_flags == 0);
		assert(sender->p_msg == 0);
		assert(sender->p_recvfrom == NO_TASK);
		assert(sender->p_sendto == NO_TASK);
	}
	else { /* dest is not waiting for the msg */
		sender->p_flags |= SENDING;
		assert(sender->p_flags == SENDING);
		sender->p_sendto = dest;
		sender->p_msg = m;

		/* append to the sending queue */
		struct proc * p;
		if (p_dest->q_sending) {
			p = p_dest->q_sending;
			while (p->next_sending)
				p = p->next_sending;
			p->next_sending = sender;
		}
		else {
			p_dest->q_sending = sender;
		}
		sender->next_sending = 0;

		block(sender);

		assert(sender->p_flags == SENDING);
		assert(sender->p_msg != 0);
		assert(sender->p_recvfrom == NO_TASK);
		assert(sender->p_sendto == dest);
	}

	return 0;
}


/*****************************************************************************
 *                                msg_receive
 *****************************************************************************/
/**
 * <Ring 0> Try to get a message from the src proc. If src is ed sending
 * the message, copy the message from it and unblock src. Otherwise the caller
 * will be blocked.
 * 
 * @param current The caller, the proc who wanna receive.
 * @param src     From whom the message will be received.
 * @param m       The message ptr to accept the message.
 * 
 * @return  Zero if success.
 *****************************************************************************/
PRIVATE int msg_receive(struct proc* current, int src, MESSAGE* m)
{
	struct proc* p_who_wanna_recv = current; /**
						  * This name is a little bit
						  * wierd, but it makes me
						  * think clearly, so I keep
						  * it.
						  */
	struct proc* p_from = 0; /* from which the message will be fetched */
	struct proc* prev = 0;
	int copyok = 0;

	assert(proc2pid(p_who_wanna_recv) != src);

	if ((p_who_wanna_recv->has_int_msg) &&
	    ((src == ANY) || (src == INTERRUPT))) {
		/* There is an interrupt needs p_who_wanna_recv's handling and
		 * p_who_wanna_recv is ready to handle it.
		 */

		MESSAGE msg;
		reset_msg(&msg);
		msg.source = INTERRUPT;
		msg.type = HARD_INT;

		assert(m);

		phys_copy(va2la(proc2pid(p_who_wanna_recv), m), &msg,
			  sizeof(MESSAGE));

		p_who_wanna_recv->has_int_msg = 0;

		assert(p_who_wanna_recv->p_flags == 0);
		assert(p_who_wanna_recv->p_msg == 0);
		assert(p_who_wanna_recv->p_sendto == NO_TASK);
		assert(p_who_wanna_recv->has_int_msg == 0);

		return 0;
	}

	/* Arrives here if no interrupt for p_who_wanna_recv. */
	if (src == ANY) {
		/* p_who_wanna_recv is ready to receive messages from
		 * ANY proc, we'll check the sending queue and pick the
		 * first proc in it.
		 */
		if (p_who_wanna_recv->q_sending) {
			p_from = p_who_wanna_recv->q_sending;
			copyok = 1;

			assert(p_who_wanna_recv->p_flags == 0);
			assert(p_who_wanna_recv->p_msg == 0);
			assert(p_who_wanna_recv->p_recvfrom == NO_TASK);
			assert(p_who_wanna_recv->p_sendto == NO_TASK);
			assert(p_who_wanna_recv->q_sending != 0);
			assert(p_from->p_flags == SENDING);
			assert(p_from->p_msg != 0);
			assert(p_from->p_recvfrom == NO_TASK);
			assert(p_from->p_sendto == proc2pid(p_who_wanna_recv));
		}
	}
	else if (src >= 0 && src < NR_TASKS + NR_PROCS) {
		/* p_who_wanna_recv wants to receive a message from
		 * a certain proc: src.
		 */
		p_from = &proc_table[src];

		if ((p_from->p_flags & SENDING) &&
		    (p_from->p_sendto == proc2pid(p_who_wanna_recv))) {
			/* Perfect, src is sending a message to
			 * p_who_wanna_recv.
			 */
			copyok = 1;

			struct proc* p = p_who_wanna_recv->q_sending;

			assert(p); /* p_from must have been appended to the
				    * queue, so the queue must not be NULL
				    */

			while (p) {
				assert(p_from->p_flags & SENDING);

				if (proc2pid(p) == src) /* if p is the one */
					break;

				prev = p;
				p = p->next_sending;
			}

			assert(p_who_wanna_recv->p_flags == 0);
			assert(p_who_wanna_recv->p_msg == 0);
			assert(p_who_wanna_recv->p_recvfrom == NO_TASK);
			assert(p_who_wanna_recv->p_sendto == NO_TASK);
			assert(p_who_wanna_recv->q_sending != 0);
			assert(p_from->p_flags == SENDING);
			assert(p_from->p_msg != 0);
			assert(p_from->p_recvfrom == NO_TASK);
			assert(p_from->p_sendto == proc2pid(p_who_wanna_recv));
		}
	}

	if (copyok) {
		/* It's determined from which proc the message will
		 * be copied. Note that this proc must have been
		 * waiting for this moment in the queue, so we should
		 * remove it from the queue.
		 */
		if (p_from == p_who_wanna_recv->q_sending) { /* the 1st one */
			assert(prev == 0);
			p_who_wanna_recv->q_sending = p_from->next_sending;
			p_from->next_sending = 0;
		}
		else {
			assert(prev);
			prev->next_sending = p_from->next_sending;
			p_from->next_sending = 0;
		}

		assert(m);
		assert(p_from->p_msg);

		/* copy the message */
		phys_copy(va2la(proc2pid(p_who_wanna_recv), m),
			  va2la(proc2pid(p_from), p_from->p_msg),
			  sizeof(MESSAGE));

		p_from->p_msg = 0;
		p_from->p_sendto = NO_TASK;
		p_from->p_flags &= ~SENDING;

		unblock(p_from);
	}
	else {  /* nobody's sending any msg */
		/* Set p_flags so that p_who_wanna_recv will not
		 * be scheduled until it is unblocked.
		 */
		p_who_wanna_recv->p_flags |= RECEIVING;

		p_who_wanna_recv->p_msg = m;
		p_who_wanna_recv->p_recvfrom = src;
		block(p_who_wanna_recv);

		assert(p_who_wanna_recv->p_flags == RECEIVING);
		assert(p_who_wanna_recv->p_msg != 0);
		assert(p_who_wanna_recv->p_recvfrom != NO_TASK);
		assert(p_who_wanna_recv->p_sendto == NO_TASK);
		assert(p_who_wanna_recv->has_int_msg == 0);
	}

	return 0;
}

/*****************************************************************************
 *                                dump_proc
 *****************************************************************************/
PUBLIC void dump_proc(struct proc* p)
{
	char info[STR_DEFAULT_LEN];
	int i;
	int text_color = MAKE_COLOR(GREEN, RED);

	int dump_len = sizeof(struct proc);

	out_byte(CRTC_ADDR_REG, START_ADDR_H);
	out_byte(CRTC_DATA_REG, 0);
	out_byte(CRTC_ADDR_REG, START_ADDR_L);
	out_byte(CRTC_DATA_REG, 0);

	sprintf(info, "byte dump of proc_table[%d]:\n", p - proc_table); disp_color_str(info, text_color);
	for (i = 0; i < dump_len; i++) {
		sprintf(info, "%x.", ((unsigned char *)p)[i]);
		disp_color_str(info, text_color);
	}

	/* printl("^^"); */

	disp_color_str("\n\n", text_color);
	sprintf(info, "ANY: 0x%x.\n", ANY); disp_color_str(info, text_color);
	sprintf(info, "NO_TASK: 0x%x.\n", NO_TASK); disp_color_str(info, text_color);
	disp_color_str("\n", text_color);

	sprintf(info, "ldt_sel: 0x%x.  ", p->ldt_sel); disp_color_str(info, text_color);
	sprintf(info, "ticks: 0x%x.  ", p->ticks); disp_color_str(info, text_color);
	sprintf(info, "priority: 0x%x.  ", p->priority); disp_color_str(info, text_color);
	sprintf(info, "pid: 0x%x.  ", p->pid); disp_color_str(info, text_color);
	sprintf(info, "name: %s.  ", p->name); disp_color_str(info, text_color);
	disp_color_str("\n", text_color);
	sprintf(info, "p_flags: 0x%x.  ", p->p_flags); disp_color_str(info, text_color);
	sprintf(info, "p_recvfrom: 0x%x.  ", p->p_recvfrom); disp_color_str(info, text_color);
	sprintf(info, "p_sendto: 0x%x.  ", p->p_sendto); disp_color_str(info, text_color);
	sprintf(info, "nr_tty: 0x%x.  ", p->nr_tty); disp_color_str(info, text_color);
	disp_color_str("\n", text_color);
	sprintf(info, "has_int_msg: 0x%x.  ", p->has_int_msg); disp_color_str(info, text_color);
}


/*****************************************************************************
 *                                dump_msg
 *****************************************************************************/
PUBLIC void dump_msg(const char * title, MESSAGE* m)
{
	int packed = 0;
	printl("{%s}<0x%x>{%ssrc:%s(%d),%stype:%d,%s(0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x)%s}%s",  //, (0x%x, 0x%x, 0x%x)}",
	       title,
	       (int)m,
	       packed ? "" : "\n        ",
	       proc_table[m->source].name,
	       m->source,
	       packed ? " " : "\n        ",
	       m->type,
	       packed ? " " : "\n        ",
	       m->u.m3.m3i1,
	       m->u.m3.m3i2,
	       m->u.m3.m3i3,
	       m->u.m3.m3i4,
	       (int)m->u.m3.m3p1,
	       (int)m->u.m3.m3p2,
	       packed ? "" : "\n",
	       packed ? "" : "\n"/* , */
		);
}

//以下为Poi OS 原创函数

//比较两字符串是否相等
PUBLIC int strcmp(char* str1,char* str2)
{
	int length1 = strlen(str1);
	int length2 = strlen(str2);
	if(length1 != length2)
		return 0;	
	for(int i=0;i<strlen(str1);i++)
	{
		if(str1[i] != str2[i])
			return 0;	
	}
	return 1;
}


PUBLIC int sys_getTime()
{
	// do
	// {
	// 	value = 0x0a;
	// 	value = value | 0x80;
	// 	out_byte(0x70,value);
	// 	value = in_byte(0x71);
	// }while(value & 0x80 != 0);
	// value = 2;
	// value = value | 0x80;
	// out_byte(0x70,value);
	// value = in_byte(0x71);
	// out_byte(0x70,0);
	// value<<28;
	// value>>28;
	disable_int();
	do
	{	timeData->year =   CMOS_READ(0x09) + CMOS_READ(0x32) * 0x100;
		timeData->month =  CMOS_READ(0x08);
		timeData->day =    CMOS_READ(0x07);	
		timeData->hour =   CMOS_READ(0x04);	
		timeData->minute = CMOS_READ(0x02);
	}while(timeData->minute != CMOS_READ(0x02));
	
	out_byte(0x70,0x00);
	enable_int();
	return 35;
}