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
        printf("move error!\n");
        break;
    }

}

PUBLIC void G2048()
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
            reset_msg(&msg);
            continue;
        }
        tryMove(msg.RETVAL,&data);
        clearAndPrint(&data);
        reset_msg(&msg);
    }
}