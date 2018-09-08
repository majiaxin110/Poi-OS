
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			      console.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
						    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef _POI_CONSOLE_H_
#define _POI_CONSOLE_H_


/* CONSOLE */
typedef struct s_console
{
	unsigned int	current_start_addr;	/* 当前显示到了什么位置	  */
	unsigned int	original_addr;		/* 当前控制台对应显存位置 */
	unsigned int	v_mem_limit;		/* 当前控制台占的显存大小 */
	unsigned int	cursor;			/* 当前光标位置 */

}CONSOLE;

#define SCR_UP	1	/* scroll forward */
#define SCR_DN	-1	/* scroll backward */

#define SCR_SIZE		(80 * 25)
#define SCR_WIDTH		80

#define DEFAULT_CHAR_COLOR	(MAKE_COLOR(BLACK, WHITE))
#define GRAY_CHAR		(MAKE_COLOR(BLACK, BLACK) | BRIGHT)
#define RED_CHAR		(MAKE_COLOR(BLACK, RED) | BRIGHT)
#define YRED_CHAR		(MAKE_COLOR(BLACK,BLUE) | BRIGHT)
#define GREEN_CHAR		(MAKE_COLOR(BLACK,GREEN) | BRIGHT)
#define BLUE_CHAR		(MAKE_COLOR(BLACK,BLUE) | BRIGHT)
#define YELLOW_CHAR		(MAKE_COLOR(BLACK,YELLOW) | BRIGHT)
#endif /* _ORANGES_CONSOLE_H_ */
