#ifndef SCHEDULE_QUEUE_H
#define SCHEDULE_QUEUE_H

typedef struct Queue	/*多级就绪队列信息*/  
{  
    struct 	proc * linked_PCB;		/*就绪队列中的进程队列指针*/  
    int 	prio;					/*本就绪队列所分配的时间片*/  
    struct 	Queue *next;			/*指向下一个就绪队列的链表指针*/  
}schedule_queue;  

schedule_queue Queue1;
schedule_queue Queue2;
schedule_queue Queue3;

#endif