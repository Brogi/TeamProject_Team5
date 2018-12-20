//박상면 시스템함수구현

#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

#define MAX_horizon 50
#define MAX_vertical 30

//checking player
//making object -> using random function
//get signal

void temp_moving(){
	//방향키 입력받는 함수사용
		int ch;

		while((ch=getch()) !=KEY_F(1)){
        addstr("              ");
	
        switch(ch){
            case KEY_LEFT:
                move(2,MAX_vertical/2);
                addstr("Key Left");
		break;
       		case KEY_RIGHT:
                move(2,MAX_vertical/2);
                addstr("Key Right");
                break;
         	case KEY_UP:
                move(2,MAX_vertical/2);
                addstr("Key Up");
                break;
         	case KEY_DOWN:
                move(2,MAX_vertical/2);
                addstr("Key Down");
                break;
        	
        	refresh();
        	}
	 }
}

void field(){
	//게임 필드 구현
	//가로 화면크기  MAX_horizon, 세로화면크기 MAX_vertical
	int i,j;
//	clear();
	for(i=0;i<MAX_vertical;i++){
		if(i==0){
			for(j=0;j<MAX_horizon+1;j++)
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
	move(1,MAX_horizon/3);
	addstr("Press F1 to exit");
}


int main( int argc, char *argv[]){


	initscr();
	//초기화면
	field();
	cbreak();
	keypad(stdscr, TRUE);
	

	
	temp_moving();


	endwin();

return 0;
}
