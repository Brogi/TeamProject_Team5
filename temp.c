

#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
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


int main( int argc, char *argv[]){


	initscr();
	cbreak();
	keypad(stdscr, TRUE);
	addstr("Check line :  ");

	
	temp_moving();


	endwin();

return 0;
}
