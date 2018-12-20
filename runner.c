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
#include <time.h>
#include <pthread.h>
#include <locale.h>
#include <wchar.h>
#include <time.h>
#define	MESSAGE	"XXXXXX"
#define BLANK "     "
#define BLANK2 "       "


int	row = 13;	/* current row		*/
int	col = 24;	/* current column	*/
int 	charRow = 10;
int	charCol = 0;
int	dir   =  -1;	/* where we are going	*/
int	delay = 20;	/* how long to wait	*/
int	done  = 0;
int level = 1;
int arrow_exsist = 0;
int status = 0;
int downstatus = 0;
char character5[6] = {'M','M','M','M','M','\0'};
char character6[8] = {'M','M','M','M','M','M','M','\0'};

void set_cr_noecho_mode(void); 
int set_ticker(int); 		

void start()
{
	char sys;

	mvaddstr(LINES/3-1,COLS/3,"<--XX           <--------XX");
	mvaddstr(LINES/3,COLS/3,"<----XX	   War of Arrows. <----XX");
	mvaddstr(LINES/3+1,COLS/3,"     <------XX");

	mvaddstr(LINES/3+7, COLS/3,"  ===Press Any key to Start===");
	mvaddstr(LINES/3+9, COLS/3,"  Jump key:Up-direction key");

	sys=getch();
}

int main(void)
{	
	void	on_alarm(int);	/* handler for alarm	*/
	void	on_input(int);	/* handler for keybd    */
	void	enable_kbd_signals();
	pthread_t t1,t2, t3;
	void *hit_check();
	void *cal_score();
	int i, j;
	
	srand(time(NULL));

	initscr();	
	
	set_cr_noecho_mode(); 

	start();

	clear();

	if(rand() % 2 == 0)
		row = LINES-5;
	else
		row = LINES - 9;
	col = COLS - strlen(MESSAGE);
	
	for(i=0; i<COLS; i++)
	{
		mvaddstr(LINES-1, i, "=");
		mvaddstr(LINES-2, i, "=");
		mvaddstr(LINES-3, i, "=");
		mvaddstr(LINES-4, i, "=");
	}
	noecho();

	signal(SIGIO, on_input);    
	enable_kbd_signals();       
	signal(SIGALRM, on_alarm);  
	set_ticker(delay);
	charRow = LINES-5;
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

	mvaddstr(LINES/2, COLS/2 - 4,"GAME OVER");	move(LINES-1, COLS-1);

	set_ticker(0);
	signal(SIGIO, SIG_IGN);
	
	getch();
	endwin();

	return 0;
}

void *hit_check()
{
	int i;

	while(!done)
	{
		for(i=0;i<5;i++)
		{
			if(!downstatus)
			{
				if((row <= charRow) && (row >= charRow - 4) && (col + i >= charCol + 1) && (col + i <= charCol + 5))
				{
					done = 1;
					break;
				}
			}
			else
			{
				if((row <= charRow) && (row >= charRow - 2) && (col + i >= charCol - 1) && (col + i <= charCol + 6))
				{
					done = 1;
					break;
				}
			}
		}
	}
}

void *cal_score()
{
	int score_int = 0;
	char st[] = "SCORE : ";
	char lt[] = "LEVEL : ";
	char level_string[128] = "1";
	char score_string[128] = "0";


	mvaddstr(1,COLS - strlen(lt) - strlen(level_string),lt);
	mvaddstr(1,COLS - strlen(level_string),level_string);
	mvaddstr(2,COLS - strlen(st) - strlen(score_string),st);
	mvaddstr(2,COLS - strlen(score_string),score_string);
	move(LINES-1, COLS-1);	

	while(!done)
	{
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
	int c = getch();		/* grab the char */
	int i, j;

	switch(c)
	{
		case KEY_UP:
			if(status || downstatus)
				break;
			status = 1;
			for(i=0; i<8; i++)
			{
				mvaddstr(charRow-4, charCol, BLANK); move(LINES-1, COLS-1);
		        mvaddstr(charRow-3, charCol, BLANK); move(LINES-1, COLS-1);
       			mvaddstr(charRow-2, charCol, BLANK); move(LINES-1, COLS-1);
       			mvaddstr(charRow-1, charCol, BLANK); move(LINES-1, COLS-1);
        		mvaddstr(charRow, charCol, BLANK); move(LINES-1, COLS-1);
				charRow--;
				mvaddstr(charRow-4, charCol, character5); move(LINES-1, COLS-1);
                mvaddstr(charRow-3, charCol, character5); move(LINES-1, COLS-1);
                mvaddstr(charRow-2, charCol, character5); move(LINES-1, COLS-1);
                mvaddstr(charRow-1, charCol, character5); move(LINES-1, COLS-1);
                mvaddstr(charRow, charCol, character5); move(LINES-1, COLS-1);
				sleep(2000);
			}
			sleep(2000);
			for(i=0; i<8; i++)
            {
                mvaddstr(charRow-4, charCol, BLANK); move(LINES-1, COLS-1);
                mvaddstr(charRow-3, charCol, BLANK); move(LINES-1, COLS-1);
                mvaddstr(charRow-2, charCol, BLANK); move(LINES-1, COLS-1);
                mvaddstr(charRow-1, charCol, BLANK); move(LINES-1, COLS-1);
                mvaddstr(charRow, charCol, BLANK); move(LINES-1, COLS-1);
                charRow++;
                mvaddstr(charRow-4, charCol, character5); move(LINES-1, COLS-1);
                mvaddstr(charRow-3, charCol, character5); move(LINES-1, COLS-1);
                mvaddstr(charRow-2, charCol, character5); move(LINES-1, COLS-1);
                mvaddstr(charRow-1, charCol, character5); move(LINES-1, COLS-1);
                mvaddstr(charRow, charCol, character5);	move(LINES-1, COLS-1);
                sleep(2000);
            }
			status = 0;
			break;

		case KEY_DOWN:
			if(status)
				break;
			else if (downstatus == 0)
			{
				status = 1;
				mvaddstr(charRow-4, charCol, BLANK); move(LINES-1, COLS-1);
                mvaddstr(charRow-3, charCol, BLANK); move(LINES-1, COLS-1);
                mvaddstr(charRow-2, charCol, BLANK); move(LINES-1, COLS-1);
                mvaddstr(charRow-1, charCol, BLANK); move(LINES-1, COLS-1);
                mvaddstr(charRow, charCol, BLANK); move(LINES-1, COLS-1);

				mvaddstr(charRow-2, charCol-1, character6); move(LINES-1, COLS-1);
                mvaddstr(charRow-1, charCol-1, character6); move(LINES-1, COLS-1);
                mvaddstr(charRow, charCol-1, character6); move(LINES-1, COLS-1);
					
				downstatus = 1;
				status = 0;
			}
			else if (downstatus == 1)
			{
				status = 1;
				mvaddstr(charRow-2, charCol-1, BLANK2); move(LINES-1, COLS-1);
                mvaddstr(charRow-1, charCol-1, BLANK2); move(LINES-1, COLS-1);
                mvaddstr(charRow, charCol-1, BLANK2); move(LINES-1, COLS-1);

				mvaddstr(charRow-4, charCol, character5); move(LINES-1, COLS-1);
                mvaddstr(charRow-3, charCol, character5); move(LINES-1, COLS-1);
                mvaddstr(charRow-2, charCol, character5); move(LINES-1, COLS-1);
                mvaddstr(charRow-1, charCol, character5); move(LINES-1, COLS-1);
                mvaddstr(charRow, charCol, character5); move(LINES-1, COLS-1);
				
				downstatus = 0;
				status = 0;
			}
			break;

		case 'Q': case EOF: case 'q':
			  done = 1;
	}
}

void on_alarm(int signum)
{
	int num;

	signal(SIGALRM, on_alarm);	/* reset, just in case	*/
	mvaddstr(row, col, BLANK); move(LINES-1, COLS-1);	/* note mvaddstr()	*/

	col += dir;			/* move to new column	*/

	if(col < 0)
	{
		if(rand() % 2 == 0)
			row = LINES-5;
		else
			row = LINES - 9;
		col = COLS - strlen(MESSAGE);
	}
		
	mvaddstr(row, col, "<---X");
	/* redo message		*/
	move(LINES-1, COLS-1);
	refresh();
}

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
