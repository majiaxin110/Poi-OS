//POI-OS 2048 game 
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
#include <string.h>

PUBLIC int print = 1;
PUBLIC int preticks = -1;      //global interger for ticks game

PUBLIC int doRand(GDATA* data)//产生0-4随机数
{
    data->seed = (1103515245*data->seed + 12345) % 4294967296;
    data->randNum = data->seed % 3 + 2;
    return data->randNum;
}
PRIVATE void initGame(GDATA* data)
{
    data->seed = get_ticks() % 100;
    for(int i=0;i < GROWS;i++)
    {
        for(int j=0;j < GCOLS;j++)
            data->cells[i][j] = 0;
    }
    int x = GROWS;
    data->cells[doRand(data)][doRand(data)] = 2;
    data->cells[doRand(data)][doRand(data)] += 2;
    data->p_con = &(console_table[2]);
}

PRIVATE void clearAndPrint(GDATA* data)
{
    clear_screen(data->p_con);
    printf("--------------------------\n");
    for(int i=0;i<GROWS;i++)
    {
        for(int j=0;j<GCOLS;j++)
        {
            printf("| ");
            if(data->cells[i][j] == 4)
                print_color_str(data->p_con,"4 ",YELLOW_CHAR);
            else if(data->cells[i][j] == 8)
                print_color_str(data->p_con,"8 ",BLUE_CHAR);
            else
                printf("%d ",data->cells[i][j]);
        }
        printf("|\n--------------------------\n");
    }
}
PRIVATE int tryMove(int direc,GDATA* data)
{
    switch(direc)
    {
    case GUP:
        for(int i=0;i<GROWS-1;i++)
            for(int j=0;j<GCOLS;j++)
                if(data->cells[i][j] == 0)
                    for(int k = i+1;k<GROWS;k++)
                        //直接使用下边各行中第一个非0数字来填补
                        if(data->cells[k][j]!=0){
                            data->cells[i][j]=data->cells[k][j];
                            data->cells[k][j]=0;
                            break;
                        }
        
        //将相同的数字相加
        for(int i=1;i<GROWS;i++)
            for(int j=0;j<GCOLS;j++)
                if(data->cells[i][j] == data->cells[i-1][j] || data->cells[i-1][j]==0){
                    data->cells[i-1][j] += data->cells[i][j];
                    data->cells[i][j]=0;
                }
        
        int addCol = doRand(data);
        if(data->cells[GROWS-1][addCol] == 0)
            data->cells[GROWS-1][addCol] = 2;
        break;

    case GDOWN:
        for(int i=GROWS-1;i > 0;i--)
            for(int j=0;j < GCOLS;j++)
                if(data->cells[i][j]==0)
                    for(int k = i-1;k > 0;k--)
                        if(data->cells[k][j]!=0){
                            data->cells[i][j]=data->cells[k][j];
                            data->cells[k][j]=0;
                            break;
                        }

        for(int i = GROWS-2;i>=0;i--)
            for(int j = 0;j<GCOLS;j++)
                if(data->cells[i][j] == data->cells[i+1][j] || data->cells[i+1][j]==0){
                    data->cells[i+1][j] += data->cells[i][j];
                    data->cells[i][j]=0;
                }
        addCol = doRand(data);
        if(data->cells[0][addCol] == 0)
            data->cells[0][addCol] = 2;
        break;
        
    case GLEFT:
        for(int j = 0;j<GCOLS-1;j++)
            for(int i = 0;i<GROWS;i++)
                if(data->cells[i][j] == 0)
                    for(int k = j+1;k<GCOLS;k++)
                        if(data->cells[i][k]!=0){
                            data->cells[i][j] = data->cells[i][k];
                            data->cells[i][k] = 0;
                            break;
                        }

        for(int j = 1;j < GCOLS;j++)
            for(int i = 0;i < GROWS;i++)
                if(data->cells[i][j] == data->cells[i][j-1] || data->cells[i][j-1] == 0){
                    data->cells[i][j-1] += data->cells[i][j];
                    data->cells[i][j] = 0;
                }
        int addRow = doRand(data);
        if(data->cells[addRow][GCOLS-1] == 0)
            data->cells[addRow][GCOLS-1] = 2;
        break;
        
    case GRIGHT:
        for(int j = GCOLS-1;j > 0;j--)
            for(int i = 0;i < GROWS;i++)
                if(data->cells[i][j] == 0)
                    for(int k = j-1;k >= 0;k--)
                        if(data->cells[i][k]!=0){
                            data->cells[i][j] = data->cells[i][k];
                            data->cells[i][k] = 0;
                            break;
                        } 

                

        for(int j = GCOLS-2;j>=0;j--)
            for(int i = 0;i<GROWS;i++)
                if(data->cells[i][j]==data->cells[i][j+1] || data->cells[i][j+1]==0){
                    data->cells[i][j+1] += data->cells[i][j];
                    data->cells[i][j] = 0;
                }
        addRow = doRand(data);
        if(data->cells[addRow][0] == 0)
            data->cells[addRow][0] = 2;

        break;
    default:
        printf("move error!");
        break;
    }

}

PUBLIC void Gpow()
{
    GDATA data;
    initGame(&data);
    clearAndPrint(&data);
    printf("\ngame process waiting...\npress ");
    print_color_str(data.p_con,"w a s d ",GREEN_CHAR);
    printf("to move, ");
    print_color_str(data.p_con,"l ",GREEN_CHAR);
    printf("to exit.\n");
    MESSAGE msg;
    reset_msg(&msg);
    while(1)
    {
        send_recv(RECEIVE,ANY,&msg);
        if(msg.RETVAL == ENDGAME)
        {
            initGame(&data);
            clearAndPrint(&data);
            printf("\ngame process waiting...\npress ");
            print_color_str(data.p_con,"w a s d ",GREEN_CHAR);
            printf("to move, ");
            print_color_str(data.p_con,"l ",GREEN_CHAR);
            printf("to exit.\n");
            reset_msg(&msg);
            continue;
        }
        tryMove(msg.RETVAL,&data);
        clearAndPrint(&data);
        reset_msg(&msg);
    }
}

PUBLIC void Gticks()
{
    int gameover_flag = 0;
    int best_score = 5000;
    GDATA data;
    data.p_con = &(console_table[1]);
    MESSAGE msg;
    reset_msg(&msg);
    clear_screen(data.p_con); 
    print_color_str(data.p_con,"Try to stop at ",YELLOW_CHAR);
    print_color_str(data.p_con,"1000 TICKS !",RED_CHAR);
    printf("\n");
    print_color_str(data.p_con,"Scan in ",YELLOW_CHAR);
    print_color_str(data.p_con,"'a'",RED_CHAR);
    print_color_str(data.p_con," to start and stop.",YELLOW_CHAR);
    printf("\n");
    print_color_str(data.p_con,"GoodLuck!.",YELLOW_CHAR);
    printf("\n");
    while(1)
    {
        send_recv(RECEIVE,ANY,&msg);
        if(msg.RETVAL == ENDGAME)
        {
            clear_screen(data.p_con);
            reset_msg(&msg);
            continue;
        }
        if(msg.RETVAL == GUP)
        {
            if(preticks == -1){
                preticks = get_game_ticks();
                printf("\n");
                print_color_str(data.p_con,"GameStart",RED_CHAR);
                printf("\n");
                msg.RETVAL = GLEFT;
				send_recv(SEND,TASK_TICKS,&msg);      //process to print ticks
            }
            else{
                if(gameover_flag == 1){
                    continue;
                }
                print = 0;
                int gap = get_game_ticks()-preticks-1000;
                if(gap<0){
                    gap = -gap;
                    printf("\nYou are early for %d ticks!\n",gap);
                }   
                else if(gap>0)
                    printf("\nYou are late for %d ticks!\n",gap);
                else
                    printf("\nYou win!\n");
                if(gap<best_score)
                    best_score = gap;
                printf("Best score is %d\n", best_score);
                print_color_str(data.p_con,"Press 'r' to Restart.",RED_CHAR);
                printf("\n");
                gameover_flag = 1;
            }
        }
        if(msg.RETVAL == GDOWN)
        {
            clear_screen(data.p_con); 
            print_color_str(data.p_con,"Try to stop at ",YELLOW_CHAR);
            print_color_str(data.p_con,"1000 TICKS !",RED_CHAR);
            printf("\n");
            print_color_str(data.p_con,"Scan in ",YELLOW_CHAR);
            print_color_str(data.p_con,"'a'",RED_CHAR);
            print_color_str(data.p_con," to start and stop.",YELLOW_CHAR);
            printf("\n");
            print_color_str(data.p_con,"GoodLuck!.",YELLOW_CHAR);
            printf("\n");
            gameover_flag = 0;
            preticks = -1;
        }
    }
}

PUBLIC int get_game_ticks()
{
    return get_ticks()/10;
}

PUBLIC void task_tick()
{
    MESSAGE msg;
    reset_msg(&msg);
    while(1)
    {
        send_recv(RECEIVE,ANY,&msg);
        if(msg.RETVAL == GLEFT)
        {
            while(print)
            {
                printf("%d ",get_game_ticks()-preticks);
                milli_delay(300);
            }
            reset_msg(&msg);
        }
        print = 1;
    }
}