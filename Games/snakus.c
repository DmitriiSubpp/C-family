#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <windows.h>

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define FIELD_Y 14
#define FIELD_X 40

void cursor_blinking_off()
{
  void* handle = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO structCursorInfo;
  GetConsoleCursorInfo(handle,&structCursorInfo);
  structCursorInfo.bVisible = FALSE;
  SetConsoleCursorInfo( handle, &structCursorInfo );
}

void update_console()
{
  COORD position = {0,0};  /* update console */
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleCursorPosition(hConsole, position);
}

short int up = 0, down = 0, left = 0, right = 0;
int key = 0, sn_len = 1, apple_flag = 0;
int delay = 200, score = 0;

typedef struct body {int x; int y;} body_t; /* snake body */

void clr_delay(int ms) // rendiring delay
{
    const int end_time = clock() + ms;
    while (clock() < end_time);
} // end clr_delay()

void game_over()
{
    clr_delay(1000);
    system("cls");

    char gm_over[3][40] =
            {"  _  _  _ _  _    _    _  _  \n",
             " (_|(_|| | |(/_  (_)\\/(/_|  \n",
             "  _|                         \n\n"};

    for (int i = 0; i < 4; i++)
        printf("%s", gm_over[i]);
    printf("\nresult: %d points and snake lenght: %d\n\n", score, sn_len);
} // end game_over()

void check_intersection(body_t *sn_body)
{
    if (sn_len >= 3)
        for (int i = 1; i < sn_len; i++)
            if (sn_body[0].x == sn_body[i].x && sn_body[0].y == sn_body[i].y)
            {
                game_over(); free(sn_body); exit(1);
            } // end if
} // end check_intersection()

body_t* add_part(body_t *sn_body) /* add snake part */
{
    sn_body = (body_t*)realloc(sn_body, (sn_len+1) * sizeof(body_t));
    if (sn_body == NULL)
        perror("add_part, realloc");

    sn_body[sn_len++] = sn_body[sn_len-1];
    return sn_body;
} // end add_part()

void add_apple(char field[FIELD_Y+2][FIELD_X+4], body_t *sn_body)
{
    int x_apple, y_apple;
    while (1)
    {
        srand(time(NULL));
        x_apple = rand() % FIELD_X;
        y_apple = rand() % FIELD_Y;
        if (field[y_apple][x_apple] == ' ') {field[y_apple][x_apple] = '@'; break;}
    } // end while
} // end add_apple()

void move(char field[FIELD_Y+4][FIELD_X+4], body_t *sn_body)
{
    for (int i = 0; i < sn_len; i++) /* clear field */
      field[sn_body[i].y][sn_body[i].x] = ' ';

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

void print_field(void)
{
    body_t *sn_body = (body_t*)realloc(NULL, sn_len+1 * sizeof(body_t));
    if ( sn_body == NULL)
        perror("print_field, realloc");
    sn_body[0].x = 20; sn_body[0].y = 4; /* start point */

    char field[FIELD_Y+2][FIELD_X+4] =
            {"0----------------------------------------x",
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

    while (key != 'q')
    {
        printf("%s", " snakus v0.1 by dmitrii_subb\n");
        if (apple_flag == 0)
        {
            add_apple(field, sn_body);
            apple_flag = 1;
        } // end if

        move(field, sn_body); /* move snake in given direction */
        check_intersection(sn_body); /* check intersection of the head with the tail */

        if (field[sn_body[0].y][sn_body[0].x] == '@')
        { /* add part of snake if head of the snake eat apple */
            sn_body = add_part(sn_body);
            apple_flag = 0; score += 10;
            if (delay > 60 ) delay -= 3;
        } // end if

        for (int i = 0; i < sn_len; i++) /* add snake on the field */
            field[sn_body[i].y][sn_body[i].x] = '*';

        for (int i = 0; i < FIELD_Y+2; i++) /* print field */
        {
          switch (i)
          {
              case 0:
                  printf("%s    score: %d\n", field[i], score);
                  break;
              case 1:
                  printf("%s    time spent (seconds): %d\n", field[i], clock()/1000);
                  break;
              case 2:
                  printf("%s    snake lenght: %d\n", field[i], sn_len);
                  break;
              case 4:
                  printf("%s    control snake with: up, down, left and right arrows\n", field[i]);
                  break;
              default:
                  printf("%s", field[i]);
            } // end switch
        } // end for
        clr_delay(delay);
        update_console();
    } // end while
} // end print_field()

void key_handler(void)
{
    while (key!= 'q')
    {
        key = _getch();
        switch (key)
        {
            case KEY_UP:
                if (down && sn_len >= 2) break;
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

int main(int argc, char **argv)
{
    cursor_blinking_off();

    pthread_t handler;
    pthread_t field;

    pthread_create(&handler, NULL, key_handler, NULL);
    pthread_create(&field, NULL, print_field, NULL);

    pthread_join(handler, NULL);
    pthread_join(field, NULL);

} // end main()
