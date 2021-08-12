#include <iostream>
#include <cstdlib>
#include <thread>
#include <vector>
#include <ctime>     // for clock()
#include <ncurses.h> // for getch() and work with console

using std::vector;
using std::thread;

#define FIELD_Y 14
#define FIELD_X 40

typedef struct body {int x; int y;} body_t;       /* snake body */
vector<body_t> sn_body {{20, 4}};

short int up = 0, down = 0, left = 0, right = 0;  /* snake direction */
int sn_len = 1;                                   /* snake lenght*/

void update_console()
{
	move(0,0);
}

class Snake
{
private:
	//int sn_len = 1;
public:
	//int get_len() { return sn_len; }
	int check_intersection()
	{
		if (sn_len >= 3)
			for (int i = 1; i < sn_len; i++)
				if (sn_body[0].x == sn_body[i].x && sn_body[0].y == sn_body[i].y)
					return 1;
				return 0;
	} // end check_cross()
	
	void add_part() /* add snake part */
	{
		sn_body.resize(sn_len+1);
		sn_body[sn_len++] = sn_body[sn_len-1];
	} // end add_part()
	
	void move()
	{
		if (up && sn_body[0].y > 1)                 /* move up   */
		{
			for (int i = sn_len-1; i > 0; i--)
				sn_body[i] = sn_body[i-1];
			sn_body[0].y--;
		} // end if
		
		if (down  && sn_body[0].y < FIELD_Y)        /* move down */
		{
			for (int i = sn_len-1; i > 0; i--)
				sn_body[i] = sn_body[i-1];
			sn_body[0].y++;
		} // end if
		
		if (left  && sn_body[0].x > 1)              /* move left */
		{
			for (int i = sn_len-1; i > 0; i--)
				sn_body[i] = sn_body[i-1];
			sn_body[0].x--;
		} // end if
		
		if (right  && sn_body[0].x < FIELD_X)       /* move right */
		{
			for (int i = sn_len-1; i > 0; i--)
				sn_body[i] = sn_body[i-1];
			sn_body[0].x++;
		} // end if
	} // end move()
	
};

class Field
{
private:
	int m_delay = 200;
	int m_apple_flag = 0;
	int m_score = 0;
	char m_field[FIELD_Y+2][FIELD_X+4] =
	{   "0----------------------------------------x",
		"|                                        |",
		"|                                        |",
		"|                                        |\n",
		"|                                        |",
		"|                                        |\n",
		"|                                        |\n",
		"|                                        |\n",
		"|                                        |\n",
		"|                                        |\n",
		"|                                        |\n",
		"|                                        |\n",
		"|                                        |\n",
		"|                                        |\n",
		"|                                        |\n",
		"y----------------------------------------+\n"};
public:
	
	Field (int delay = 200): m_delay(delay) {}
	
	void clr_delay(int ms) /* rendiring delay */
	{
		const int end_time = clock() + ms;
		while (clock() < end_time);
	} // end clr_delay()
	
	void game_over()
	{
		//clr_delay(100000);
		move(0,0);
		
		char gm_over[3][40] =
		{   "  _  _  _ _  _    _    _  _  \n",
			" (_|(_|| | |(/_  (_)\\/(/_|  \n",
			"  _|                         \n\n"};
			
		 for (int i = 0; i < 4; i++)
			 printw("%s", gm_over[i]);
		
		printw("\nresult: %d points and snake lenght: %d\n", m_score, sn_len);
		printw("ctrl+c to exit\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
		
		refresh();
		getchar();
		endwin();
		//exit(1);
	} // end game_over()
	
	int has_apple() { return m_apple_flag; } /* m_apple_flag getter. Check apple status */
	
	void add_apple()
	{ /*  */
		int x_apple, y_apple;
		m_apple_flag = 1; /* update apple status*/
		while (1)
		{
			srand(time(NULL));
			x_apple = rand() % FIELD_X;
			y_apple = rand() % FIELD_Y;
			if ( m_field[y_apple][x_apple] == ' ' ) { m_field[y_apple][x_apple] = '@'; break; }
		} // end while
	} // end add_apple()
	
	int check_apple()
	{
		if ( m_field[sn_body[0].y][sn_body[0].x] == '@' )
		{ /* return 1 if head of the snake eat apple */
			m_apple_flag = 0; m_score += 10;
			if (m_delay > 60 ) m_delay -= 3;
			return 1;
		} // end if
		return 0;
	} // end
	
	void update()
	{
		for (int i = 0; i < sn_len; i++) /* clear field */
			m_field[sn_body[i].y][sn_body[i].x] = ' ';
	}
	
	void update_snake()
	{ /* update snake on the field */
		for (int i = 0; i < sn_len; i++)
			m_field[sn_body[i].y][sn_body[i].x] = '*';
	}
	
	void print()
	{
		printw("%s", " snakus v0.2 by dmitrii_subb\n");
		
		for (int i = 0; i < FIELD_Y+2; i++) /* print field */
		{
			switch (i)
			{
				case 0:
					printw("%s    score: %d\n", m_field[i], m_score);
					break;
				case 1:
					printw("%s    time spent (seconds): %d\n", m_field[i], clock()/1000000);
					break;
				case 2:
					printw("%s    snake lenght: %d\n", m_field[i], sn_len);
					break;
				case 4:
					printw("%s    control snake with: up, down, left and right arrows\n", m_field[i]);
					break;
				default:
					printw("%s", m_field[i]);
			} // end switch
		} // end for
	
		clr_delay(m_delay);
		refresh();
		update_console();
		
	} // end print_field()
};

// ------=[main part: g++ snake.cpp -o snake -lncurses -pthread]=------

void key_handler(void)
{
	int key = 0;
	while (key!= 'q')
	{
		key = getch();
		switch (key)
		{
			case KEY_UP:
				if (down && sn_len >= 2) break;  /* snake will not turn in the opposite direction */
					down = left = right = 0; up = 1;
				break;
			case KEY_DOWN:
				if (up && sn_len >= 2) break;
				up = left = right = 0; down = 1;
				break;
			case KEY_LEFT:
				if (right && sn_len >= 2) break;
				up = down = right = 0; left = 1;
				break;
			case KEY_RIGHT:
				if (left && sn_len >= 2) break;
				up = down = left = 0; right = 1;
				break;
		} // end switch
	} // end while
} // end key_handler()

void game(void)
{
	Snake snake;
	Field field(200000);
	
	while (1)
	{
		
		if ( !field.has_apple() )           /* if there isn't apple on the field */
			field.add_apple();
		
		field.update();                     /* delete snake from the field */
		snake.move();                       /* set new position for snake*/
		
		if ( snake.check_intersection() )
			field.game_over();
		
		if ( field.check_apple() )          /* if snake ate apple */
			snake.add_part();
		
		field.update_snake();               /* update snake position on the field */
		field.print();                      /* print field with snake */
		
	} // end while
} // end game()

int main(int argc, char **argv)
{
	//cursor_blinking_off();
	initscr();
	keypad(stdscr, TRUE);
	
	thread handler_stream(key_handler);
	thread game_stream(game);
	
	handler_stream.join();
	game_stream.join();
	
	endwin();
}

