

#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

#define MAX_horizon 30
#define MAX_vertical 20

//checking player
//making object -> using random function
//get signal

void temp_moving(){
		int ch;

		while((ch=getch()) !=KEY_F(1)){
		clear();
        addstr("Check line : ");

        switch(ch){
            case KEY_LEFT:
                move(1,15);
                addstr(" Key Left");
                break;
       		case KEY_RIGHT:
                move(1,15);
                addstr(" Key Right");
                break;
         	case KEY_UP:
                move(1,15);
                addstr(" Key Up");
                break;
         	case KEY_DOWN:
                move(1,15);
                addstr(" Key Down");
                break;
        	case KEY_F(1):
                addstr("Introduce Game Rule");
                break;
        	refresh();
        	}
		 }
}

void field(){
	int i,j;

	for(i=0;i<MAX_vertical;i++){
		if(i==0){
			for(j=0;j<MAX_horizon;j++)
			addstr("*");
			continue;
		}

		move(i,0);
		addstr("*");
		move(i,MAX_horizon);
		addstr("*");

	
		if(i==MAX_vertical-1){
			move(i,0);
			for(j=0;j<MAX_horizon;j++)
			addstr("*");
			continue;
		}
	}




}


int main( int argc, char *argv[]){


	initscr();
	field();
	cbreak();
	keypad(stdscr, TRUE);
	

	
	temp_moving();


	endwin();

return 0;
}
