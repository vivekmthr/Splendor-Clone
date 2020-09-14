#ifndef	TERMFUNCS_H
#define	TERMFUNCS_H
//
//   termfuncs2.h -- some simple functions for using the terminal nicely
//
//   char getachar()         -- waits for and returns next char 
//                              with no echo and no Enter needed
//   char getacharnow(n)     -- like getachar but times out at n / 10 sec
//			     -- if timeout, return '\0'
//   void screen_clear()     -- clears the screen
//   void screen_home()      -- moves cursor to top of screen
//
//   int get_screen_rows()  -- return dimensions of terminal
//   int get_screen_cols()
//

using namespace std;

char getachar();
char getacharnow(int decisecs);
void screen_clear();
void screen_home();

//
// colors can be "black", "red", "green", "yellow", "blue"
// "purple", "cyan", "white"
//
// attributes can be "reset", "bright", "dim", "underscore",
// "blink", "reverse", "hidden"
//
void screen_fg(string color);
void screen_bg(string color);
void screen_attr(string color);

void screen_bright();
void screen_reset();

void place_cursor(int, int);
void place_char(char, int, int);

void hide_cursor();
void show_cursor();

int get_screen_rows();
int get_screen_cols();
#endif
