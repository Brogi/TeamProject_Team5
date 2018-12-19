#include <stdio.h>
#include <curses.h>
#include <ncurses.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <termio.h> 
#include <pthread.h>
#include <locale.h>
#include <wchar.h>
#define	MESSAGE	"XXXXXX"
#define	BLANK	"     "

int	row = 13;	/* current row		*/
int	col = 24;	/* current column	*/
int 	charRow = 10;
int	charCol = 0;
int	dir   =  -1;	/* where we are going	*/
int	delay = 20;	/* how long to wait	*/
int	done  = 0;
int level = 1;
int arrow_exsist = 0;
void set_cr_noecho_mode(void); 
int set_ticker(int); 		

char character1[5] = {'X','X','X','X','X'};
char character2[5] = {' ','X','X','X',' '};
char     character3[5] = {' ',' ','X',' ',' '};
char     character4[5] = {' ','X','X','X',' '};
char     character5[5] = {'X','X','X','X','X'};

int main(void)
{	void	on_alarm(int);	/* handler for alarm	*/
	void	on_input(int);	/* handler for keybd    */
	void	enable_kbd_signals();
	pthread_t t1,t2;
	void *hit_check();
	void *cal_score();
	int i, j;
	
	initscr();	
	
	row = LINES/2;
	col = COLS - strlen(MESSAGE);

	set_cr_noecho_mode(); 

	clear();
	
	for(i=0; i<COLS; i++)
	{
		mvaddstr(row+1, i, "+");
		mvaddstr(row+2, i, "+");
		mvaddstr(row+3, i, "+");
		mvaddstr(row+4, i, "+");
	}
	noecho();

	signal(SIGIO, on_input);    
	enable_kbd_signals();       
	signal(SIGALRM, on_alarm);  
	set_ticker(delay);
	charRow = LINES/2;
	charCol = 5;

	mvaddstr(charRow-4, charCol, character5);
	mvaddstr(charRow-3, charCol, character5);
	mvaddstr(charRow-2, charCol, character5);
    mvaddstr(charRow-1, charCol, character5);
	mvaddstr(charRow, charCol, character5);

	keypad(stdscr, TRUE);

	pthread_create(&t1,NULL,hit_check,NULL);
	pthread_create(&t2,NULL,cal_score,NULL);
	
	while( !done )			  
		pause();
	mvaddstr(LINES/2, COLS/2 - 4,"GAME OVER");
	set_ticker(0);
	signal(SIGIO, SIG_IGN);
	getch();
	endwin();

	return 0;
}

void *hit_check(){
	int i;
	while(!done){
		for(i=0;i<5;i++){
			if(row <= charRow && row >= charRow-4 && col + i >= charCol && col + i <= charCol +4)
				done = 1;
		}
	}
}

void *cal_score(){
	int score_int = 0;
	char st[] = "SCORE : ";
	char lt[] = "LEVEL : ";
	char level_string[128] = "1";
	char score_string[128] = "0";


	mvaddstr(1,COLS - strlen(lt) - strlen(level_string),lt);
	mvaddstr(1,COLS - strlen(level_string),level_string);
	mvaddstr(2,COLS - strlen(st) - strlen(score_string),st);
	mvaddstr(2,COLS - strlen(score_string),score_string);	
	while(!done){
		sleep(1);
		score_int+= 50;
		sprintf(score_string,"%d",score_int);
		mvaddstr(2,COLS - strlen(st) - strlen(score_string),st);
		mvaddstr(2,COLS - strlen(score_string),score_string);
		if(level < 10 && score_int / 500 && !(score_int % 500)){
			level++;
			delay-= 2;
			set_ticker(delay);
		}
		sprintf(level_string,"%d",level);
		mvaddstr(1,COLS - strlen(lt) - strlen(level_string),lt);
		mvaddstr(1,COLS - strlen(level_string),level_string);
	}
}


void on_input(int signum)
{		
	int 	c = getch();		/* grab the char */
	int i, j;
	
	switch(c)
	{
		case KEY_UP:
			for(i=0; i<8; i++)
			{
				mvaddstr(charRow-4, charCol, BLANK);
		        	mvaddstr(charRow-3, charCol, BLANK);
       				mvaddstr(charRow-2, charCol, BLANK);
       				mvaddstr(charRow-1, charCol, BLANK);
        			mvaddstr(charRow, charCol, BLANK);
				charRow--;
				mvaddstr(charRow-4, charCol, character5);
                                mvaddstr(charRow-3, charCol, character5);
                                mvaddstr(charRow-2, charCol, character5);
                                mvaddstr(charRow-1, charCol, character5);
                                mvaddstr(charRow, charCol, character5);
				sleep(2000);
			}
			sleep(20000);
			for(i=0; i<8; i++)
                        {
                                mvaddstr(charRow-4, charCol, BLANK);
                                mvaddstr(charRow-3, charCol, BLANK);
                                mvaddstr(charRow-2, charCol, BLANK);
                                mvaddstr(charRow-1, charCol, BLANK);
                                mvaddstr(charRow, charCol, BLANK);
                                charRow++;
                                mvaddstr(charRow-4, charCol, character5);
                                mvaddstr(charRow-3, charCol, character5);
                                mvaddstr(charRow-2, charCol, character5);
                                mvaddstr(charRow-1, charCol, character5);
                                mvaddstr(charRow, charCol, character5);
                                sleep(2000);
                        }
			break;
		case 'Q': case EOF: case 'q':
			  done = 1;
	}
}

void on_alarm(int signum)
{
	signal(SIGALRM, on_alarm);	/* reset, just in case	*/
	mvaddstr(row, col, BLANK);	/* note mvaddstr()	*/
	col += dir;			/* move to new column	*/

	if(col<0){
		row = LINES/2;
		col = COLS - strlen(MESSAGE);
	}

	mvaddstr(row, col, "<---X");
	/* redo message		*/
	move(LINES-1, COLS-1);
	refresh();
	/*
	 * now handle borders
	 */
/*	
	if ( dir == -1 && col <= 0 )
		dir = 1;
	else if ( dir == 1 && col+strlen(MESSAGE) >= COLS )
		dir = -1;
*/
}

/*
 * install a handler, tell kernel who to notify on input, enable signals
 */
void enable_kbd_signals()
{
	int  fd_flags;

	fcntl(0, F_SETOWN, getpid());         /*set io signal to current pid*/
	fd_flags = fcntl(0, F_GETFL);         /*get status of the file*/
	fcntl(0, F_SETFL, (fd_flags|O_ASYNC));/*set status to tty with O_ASYNC*/
}

void set_cr_noecho_mode(void)
{
	struct termios ttystate;
	
	tcgetattr( 0, &ttystate );
	ttystate.c_lflag	&= ~ICANON;
	ttystate.c_lflag	&= ~ECHO;
	ttystate.c_cc[VMIN]	= 1;
	tcsetattr( 0, TCSANOW, &ttystate);
}

int set_ticker( int n_msecs )
{
	struct itimerval new_timeset;
	long    n_sec, n_usecs;
	n_sec = n_msecs / 1000 ;		/* int part*/
	n_usecs = ( n_msecs % 1000 ) * 1000L ;  /* remainder*/

	new_timeset.it_interval.tv_sec = n_sec;		/* set reload */
	new_timeset.it_interval.tv_usec = n_usecs;  /* new ticker value */
	new_timeset.it_value.tv_sec     = n_sec;	/* store this */
	new_timeset.it_value.tv_usec    = n_usecs;	/* and this */

	return setitimer(ITIMER_REAL, &new_timeset, NULL);
}
