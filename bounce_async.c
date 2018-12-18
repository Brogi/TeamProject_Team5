#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <termio.h> 
#include <locale.h>
#include <wchar.h>
#define	MESSAGE	"XXXXXX"
#define	BLANK	"      "

int	row = 13;	/* current row		*/
int	col = 24;	/* current column	*/
int 	charRow = 10;
int	charCol = 0;
int	dir   =  -1;	/* where we are going	*/
int	delay = 20;	/* how long to wait	*/
int	done  = 0;
void set_cr_noecho_mode(void); 
int set_ticker(int); 		

char character[5][5] = {{'X','X','X','X','X'}, 
			{'0','X','X','X','0'},
			{'0','0','X','0','0'},
			{'0','X','X','X','0'},
			{'X','X','X','X','X'}};

int main(void)
{	void	on_alarm(int);	/* handler for alarm	*/
	void	on_input(int);	/* handler for keybd    */
	void	enable_kbd_signals();
	int i, j;
	
	initscr();	
	
	row = LINES/2;
	col = COLS - strlen(MESSAGE);

	set_cr_noecho_mode(); 

	clear();
	
	for(i=0; i<COLS; i++)
	{
		mvaddstr(row+3, i, "+");
		mvaddstr(row+4, i, "+");
		mvaddstr(row+5, i, "+");
		mvaddstr(row+6, i, "+");
	}
	noecho();

	signal(SIGIO, on_input);    
	enable_kbd_signals();       
	signal(SIGALRM, on_alarm);  
	set_ticker(delay);
	
	move(row,col);
	
	while( !done )			  
		pause();
	endwin();

	return 0;
}

void on_input(int signum)
{		
	int 	c = getchar();		/* grab the char */

	if ( c == 'Q' || c == EOF )
		done = 1;
	else if ( c == ' ' )
		dir = -dir;
}

void on_alarm(int signum)
{
	signal(SIGALRM, on_alarm);	/* reset, just in case	*/
	mvaddstr(row, col, BLANK);	/* note mvaddstr()	*/
	col += dir;			/* move to new column	*/

	mvaddstr(row, col, "<---X");
	/* redo message		*/
	refresh();			/* and show it		*/

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
