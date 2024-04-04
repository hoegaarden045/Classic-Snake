#include <ncurses.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

int snake_size = 7;
int dir = KEY_LEFT;
int fruit_check = 0, flag = 0, prev_dir = 0, max_x = 0, max_y = 0;

static void* thread(void *none){
	while(TRUE){
		dir = getch();
	}
}

typedef struct _coords{
	int x; 
	int y;
}coords;

coords snake[1000];
coords fruit;

void copy_snake(int head_x, int head_y){
	int tmp_x, tmp_y;
	for(int i = 1; i < snake_size; i++){
		tmp_x = snake[i].x;
		tmp_y = snake[i].y;
		snake[i].x = head_x;
		snake[i].y = head_y;
		head_x = tmp_x;
		head_y = tmp_y;
	}
}

int check_death(coords snake[1000], int snake_size){
	for(int i = 1; i < snake_size; i++){
		if(snake[i].x == snake[0].x && snake[i].y == snake[0].y){
			return 1;
		}
	}
	return 0;
}

int game(){
	bool done = FALSE;
	snake[0].x = max_x/2;
	snake[0].y = max_y/2;

	while (TRUE){
		int head_x, head_y;
		head_x = snake[0].x;
		head_y = snake[0].y;

		if (dir == KEY_UP && prev_dir == KEY_DOWN) dir = KEY_DOWN;
		else if (dir == KEY_DOWN && prev_dir == KEY_UP) dir = KEY_UP;
		else if (dir == KEY_LEFT && prev_dir == KEY_RIGHT) dir = KEY_RIGHT;
		else if (dir == KEY_RIGHT && prev_dir == KEY_LEFT) dir = KEY_LEFT;	
	
		switch (dir){
			case KEY_UP:
				snake[0].y -= 1;
				snake[0].y = (snake[0].y >= 0) ? snake[0].y % max_y : max_y;
				break;
			case KEY_DOWN:
				snake[0].y += 1;
				snake[0].y %= max_y;
				break;
			case KEY_RIGHT:
				snake[0].x += 1;
				snake[0].x %= max_x;
				break;
			case KEY_LEFT:
				snake[0].x -= 1;
				snake[0].x = (snake[0].x >= 0) ? snake[0].x % max_x : max_x;
				break;
			case 'q':
				done = TRUE;
				break;
		}
		if(done) return 0;
		
		copy_snake(head_x, head_y);
		prev_dir = dir;


		attron(COLOR_PAIR(2));
		for(int i = 0; i < snake_size; i++){
			mvaddch(snake[i].y , snake[i].x, '*' );
		}
		attroff(COLOR_PAIR(2));

		refresh();
		usleep(150000);
		for(int i = 0; i < snake_size; i++){
			mvaddch(snake[i].y , snake[i].x, ' ' );
		}
		
		if (check_death(snake, snake_size) == 1){
			break;
		}

		if(fruit_check == 0){
			fruit.x = rand() % max_x;
			fruit.y = rand() % max_y;
			attron(COLOR_PAIR(3));
			mvaddch(fruit.y, fruit.x, '@');
			attron(COLOR_PAIR(3));
			fruit_check = 1;
		}
		if(snake[0].x == fruit.x && snake[0].y == fruit.y){
			snake_size ++;
			fruit_check = 0;
		}
	}
	clear();
	mvprintw(max_y/2, max_x/2 - 12, "You loose! Your score: %d", snake_size);	
	mvaddstr(max_y/2 + 1, max_x/2 - 9, "Press 'q' to exit...");
	refresh();
	while(TRUE){
		if(dir == 'q') return 0;
	}
}
			

int main(){

	srand(time(NULL));
	initscr();
	noecho();
	start_color();
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	curs_set(0);
	keypad(stdscr, TRUE);

	pthread_t pid;
	pthread_create(&pid, NULL, thread, NULL);
	getmaxyx(stdscr, max_y, max_x);
	
	game();
	
	endwin();
	
	return 0;
}
