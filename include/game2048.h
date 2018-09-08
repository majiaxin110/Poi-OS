//针对Game 2048 的一系列声明
//by MT

#define GROWS    5
#define GCOLS    5

//一系列操作
#define STARTGAME   0
#define ENDGAME     1
#define GUP          2
#define GDOWN        3
#define GLEFT        4
#define GRIGHT       5

typedef struct game2048_data
{
    int cells[5][5];
    int seed;
    int randNum;
    CONSOLE* p_con;
}GDATA;