#include <ncurses.h>
#include <time.h>
#include <stdlib.h>

static int max_x, max_y;

typedef struct _snake_node {
	int x, y;
	struct _snake_node *next;
} snake_node;

typedef enum _snake_dir {
	UP, DOWN, LEFT, RIGHT, STOP
} snake_dir;

int check_snake_overlap(snake_node *snake, int new_y, int new_x)
{
	while (snake != NULL) {
		if (snake->x == new_x && snake->y == new_y) return -1;
		snake = snake->next;
	}

	return 0;
}

snake_node* update_and_print_snake(snake_node *snake, int new_y, int new_x, int grow)
{
	snake_node *first = (snake_node *) malloc(sizeof(snake_node));
	first->x = new_x;
	first->y = new_y;
	first->next = snake;
	snake = first;

	while (snake != NULL)
	{
		mvaddch(snake->y, snake->x, 'X');
		if ((snake->next != NULL && snake->next->next == NULL) && grow == 0)
			snake->next = NULL;

		snake = snake->next;

	}

	return first;
}

void generate_random_pos(int *y, int *x)
{
	*y = rand() % max_y;
	*x = rand() % max_x;
}

void print_frame()
{
	int i;
	for (i = 0; i < max_y; i++)
	{
		mvaddch(i, 0, '|');
		mvaddch(i, max_x - 1, '|');
	}

	for (i = 0; i < max_x; i++)
	{
		mvaddch(0, i, '-');
		mvaddch(max_y - 1, i, '-');
	}
}

int main(int argc, const char *argv[])
{
	int x = 199, y = 5;
	int grow;
	int food_x = 10, food_y = 10;
	int ch;
	snake_dir dir = STOP;
	snake_node *snake = (snake_node *) malloc(sizeof(snake_node));
	snake->x = x;
	snake->y = y;
	snake->next = NULL;

	srand(time(NULL));

	initscr();						/* Start curses mode 		 		*/
	curs_set(0);
	noecho();
	timeout(100);
	keypad(stdscr, 1);
	getmaxyx(stdscr, max_y, max_x);
	printw("%dx%d\n", max_y, max_x);
	mvaddch(y, x, 'X');

	generate_random_pos(&food_y, &food_x);

	refresh();						/* Print it on to the real screen 	*/

	while (1)
	{
		switch (ch = getch())
		{
			case 27:
				endwin();
				return 0;
			case KEY_LEFT:
				dir = LEFT;
				break;
			case KEY_RIGHT:
				dir = RIGHT;
				break;
			case KEY_UP:
				dir = UP;
				break;
			case KEY_DOWN:
				dir = DOWN;
				break;
			default:
				break;
		}

		switch (dir)
		{
			case LEFT:
				x--;
				break;
			case RIGHT:
				x++;
				break;
			case UP:
				y--;
				break;
			case DOWN:
				y++;
				break;
			default:
				break;
		}

		grow = 0;
		clear();
		print_frame();

		move(0, 3);
		printw("[%3dx%3d]", y, x);

		if ((x < 1 || x >= max_x - 1) ||
			(y < 1 || y >= max_y - 1) ||
			(dir != STOP && check_snake_overlap(snake, y, x)))
		{
			endwin();
			return 0;
		}

		if (y == food_y && x == food_x)
		{
			generate_random_pos(&food_y, &food_x);
			grow = 1;

		}

		mvaddch(food_y, food_x, 'O');
		//mvaddch(y, x, 'X');
		snake = update_and_print_snake(snake, y, x, grow);
		refresh();
	}

	getch();						/* Wait for user input 				*/

	endwin();						/* End curses mode		 			*/

	return 0;
}