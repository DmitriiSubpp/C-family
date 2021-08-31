#include <iostream>
#include <thread>
#include <vector>
#include <ctime>     // for clock()
#include <windows.h> // for work with console
#include <conio.h>   // for _getch()

using std::vector;
using std::thread;

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define SPACE 32
#define FIELD_Y 26
#define FIELD_X 77

typedef struct body {int x; int y;} body_t;       /* tank body */

vector<body_t> tank_body {{20, 10}, {21, 10}, {22, 10}, {23, 10}, {24, 10},   // "  |  "
                          {20, 11}, {21, 11}, {22, 11}, {23, 11}, {24, 11},   // "-[O]-"
                          {20, 12}, {21, 12}, {22, 12}, {23, 12}, {24, 12}};  // "  |  "

short int up = 0, down = 0, left = 0, right = 0;  /* tank direction */
bool shot_flag = false;
int shot_counter = 0;
const int tank_len = 15;

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

typedef struct Shot
{
  int up = false;
  int down = false;
  int left = false;
  int right = false;

  int x;
  int y;
} shot_t;

vector<shot_t> shots{};

class Tank
{
  private:

  public:

    void shot()
    {
      if ( shot_counter < 10 )
      {
        shots.resize(shot_counter+1);
        if ( up )
        {
          shots[shot_counter].up = true;
          shots[shot_counter].x = tank_body[2].x;
          shots[shot_counter].y = tank_body[2].y-2;
        }
        if ( down )
        {
          shots[shot_counter].down = true;
          shots[shot_counter].x = tank_body[12].x;
          shots[shot_counter].y = tank_body[12].y+2;
        }
        if ( left )
        {
          shots[shot_counter].left = true;
          shots[shot_counter].x = tank_body[5].x-2;
          shots[shot_counter].y = tank_body[5].y;
        }
        if ( right )
        {
          shots[shot_counter].right = true;
          shots[shot_counter].x = tank_body[9].x+2;
          shots[shot_counter].y = tank_body[9].y;
        }
        shot_counter++;
      }
      shot_flag = false;
    }

    void update_shots()
    {
      for (int i = 0; i < shot_counter; i++)
      {
        if ( shots[i].up && shots[i].y >= 2 )
          shots[i].y-=1;
        if ( shots[i].down && shots[i].y < FIELD_Y - 2 )
          shots[i].y+=1;
        if ( shots[i].left && shots[i].x > 2 )
          shots[i].x-=2;
        if ( shots[i].right && shots[i].x < FIELD_X - 3 )
          shots[i].x+=2;
      }
    }

    void move()
    {
        if ( up )
        {
            for ( int i = 0; i < tank_len; i++ )
              if ( tank_body[7].y > 3 ) tank_body[i].y--;
        } // end if

        if ( down )
        {
            for ( int i = 0; i < tank_len; i++ )
              if ( tank_body[7].y < FIELD_Y - 4 ) tank_body[i].y++;
        } // end if

        if ( left )
        {
            for (int i = 0; i < tank_len; i++)
              if (tank_body[7].x > 4) tank_body[i].x--;
        } // end if

        if ( right )
        {
            for ( int i = 0; i < tank_len; i++ )
              if ( tank_body[7].x < FIELD_X - 4 ) tank_body[i].x++;
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
            {"0---------------------------------------------------------------------------x",
             "|                                                                           |",
             "|                                                                           |",
             "|                                                                           |\n",
             "|                                                                           |",
             "|                                                                           |\n",
             "|                                                                           |\n",
             "|                                                                           |\n",
             "|                                                                           |\n",
             "|                                                                           |\n",
             "|                                                                           |\n",
             "|                                                                           |\n",
             "|                                                                           |\n",
             "|                                                                           |\n",
             "|                                                                           |\n",
             "|                                                                           |\n",
             "|                                                                           |\n",
             "|                                                                           |\n",
             "|                                                                           |\n",
             "|                                                                           |\n",
             "|                                                                           |\n",
             "|                                                                           |\n",
             "|                                                                           |\n",
             "|                                                                           |\n",
             "|                                                                           |\n",
             "y---------------------------------------------------------------------------n"};

      char m_tank[10][10]=
             {"  |  ",
              "-[O]-",
              "  |  "};

  public:

    Field ( int delay = 200 ): m_delay( delay ) {}

    void clr_delay( int ms ) /* rendiring delay */
    {
        const int end_time = clock() + ms;
        while ( clock() < end_time );
    } // end clr_delay()

    void print_shot()
    {
      for ( int i = 0; i < shot_counter; i++ )
        if ( shots[i].y < FIELD_Y - 2 && shots[i].y > 1 && shots[i].x > 2 && shots[i].x < FIELD_X - 3 )
          m_field[shots[i].y][shots[i].x] = '*';
    }

    void set_tank()
    {
      for ( int i = 0; i < tank_len; i++ )
        m_field[tank_body[i].y][tank_body[i].x] = m_tank[tank_body[i].y - tank_body[0].y][tank_body[i].x - tank_body[0].x];
    }

    void update_tank()
    {
        for ( int i = 0; i < tank_len; i++ )
        {
          if ( up )
            if ( i != 5 && i != 9 && i != 12 )
              m_field[tank_body[i].y][tank_body[i].x] = m_tank[tank_body[i].y - tank_body[0].y][tank_body[i].x - tank_body[0].x];
            else m_field[tank_body[i].y][tank_body[i].x] = ' ';

          if ( down )
            if ( i != 5 && i != 9 && i != 2 )
              m_field[tank_body[i].y][tank_body[i].x] = m_tank[tank_body[i].y - tank_body[0].y][tank_body[i].x - tank_body[0].x];
            else m_field[tank_body[i].y][tank_body[i].x] = ' ';

          if ( left )
            if ( i != 12 && i != 9 && i != 2 )
              m_field[tank_body[i].y][tank_body[i].x] = m_tank[tank_body[i].y - tank_body[0].y][tank_body[i].x - tank_body[0].x];
            else m_field[tank_body[i].y][tank_body[i].x] = ' ';

          if ( right )
            if ( i != 12 && i != 5 && i != 2 )
              m_field[tank_body[i].y][tank_body[i].x] = m_tank[tank_body[i].y - tank_body[0].y][tank_body[i].x - tank_body[0].x];
            else m_field[tank_body[i].y][tank_body[i].x] = ' ';
        }
    }

    void clear()
    {
      for ( int i = 0; i < tank_len; i++ ) /* clear field */
          m_field[tank_body[i].y][tank_body[i].x] = ' ';

      for ( int i = 0; i < shot_counter; i++ )
          m_field[shots[i].y][shots[i].x] = ' ';
    }

    void print()
    {
        printf("%s", " tankus v0.1 by dmitrii_subb\n");

        for ( int i = 0; i < FIELD_Y+2; i++ ) /* print field */
        {
          switch ( i )
          {
              case 0:
                  printf("%s    score: %d\n", m_field[i], m_score);
                  break;
              case 1:
                  printf("%s    time spent (seconds): %d\n", m_field[i], clock()/1000);
                  break;
              case 2:
                  printf("%s    shots left: %2d\n", m_field[i], 10 - shot_counter);
                  break;
              case 4:
                  printf("%s    shot with space \n", m_field[i]);
                  break;
              default:
                  printf("%s", m_field[i]);
            } // end switch
        } // end for
        clr_delay( m_delay );
        update_console();
    } // end print_field()
};

// ------=[main part]=------

void key_handler( void )
{
  int key = 0;
  while ( key != 'q' )
  {
    key = _getch();
    //printf("%d\n", key);
    switch ( key )
    {
        case KEY_UP:
            down = left = right = 0; up = 1;
            break;
        case KEY_DOWN:
            up = left = right = 0; down = 1;
            break;
        case KEY_LEFT:
            up = down = right = 0; left = 1;
            break;
        case KEY_RIGHT:
            up = down = left = 0; right = 1;
            break;
        case SPACE:
            shot_flag = true;
            break;
    } // end switch
  } // end while
} // end key_handler()


void game(void)
{
  Field field( 100 );
  Tank tank;

  while ( 1 )
  {

    tank.update_shots();
    field.print_shot();

    if ( shot_flag )
      tank.shot();

    tank.move();
    field.update_tank();

    field.print();
    field.clear();

    //std::cout << GetKeyState('W') << std::endl;

  } // end while
} // end game()

int main( int argc, char **argv )
{
      cursor_blinking_off();

      thread handler_stream( key_handler );
      thread game_stream( game );

      handler_stream.join();
      game_stream.join();
}
