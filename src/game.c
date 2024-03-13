#include "game.h"
#include "field.h"

#include <ncurses.h>
#include <string.h>
#include <stdbool.h>

// windows to be used
WINDOW *field_w;
WINDOW *stats_w;
WINDOW *modes_w;

// special ""global"" variables
static field f;
static bool FLAG_MODE = false; // its either reveal (false) or flag (true)
static game_state state;

void game_init(int field_size){
  initscr();
  keypad(stdscr, true);

  field_w = newwin(field_size*2+1, field_size*2+1, 1, 1);
  stats_w = newwin(3, 21, 1, (field_w->_begx+field_w->_maxx)+1);
  modes_w = newwin(5, 28, (stats_w->_begy+stats_w->_maxy)+1, (field_w->_begx+field_w->_maxx)+1);

  f = field_init(BOARD_SIZE);
  
  refresh();
}

void game_free(){
  endwin();

  field_w = NULL;
  stats_w = NULL;
  modes_w = NULL;

  field_free(&f);
}

void field_draw(){
  if(field_w == NULL)
    return;

  box(field_w, 0, 0);

  for(int y=0;y<f.size;y++)
    for(int x=0;x<f.size;x++){
      if(f.tiles[y][x].revealed == FLAGGED)
	mvwprintw(field_w, y*2+1, x*2+1, "F");
      else if(f.tiles[y][x].revealed == HIDDEN)
	mvwprintw(field_w, y*2+1, x*2+1, "#");
      else if(f.tiles[y][x].type == TYPE_BOMB)
	mvwprintw(field_w, y*2+1, x*2+1, "@");
      else
	mvwprintw(field_w, y*2+1, x*2+1, "%d", f.tiles[y][x].type);

      
      int test = 0;
      if((x*2+2) < field_w->_maxx){
	mvwprintw(field_w, y*2+1, x*2+2, "|");
	test+=1;
      }
      if(y*2+2 < field_w->_maxy){
	mvwprintw(field_w, y*2+2, x*2+1, "-");
	test+=1;
      }
      if(test>=2)
	mvwprintw(field_w, y*2+2, x*2+2, "+");
    }
  
  wrefresh(field_w);
}

void stats_draw(){
  if(stats_w == NULL){
    printw("lol");
    return;
  }

  mvwprintw(stats_w, 1, 1, "BOMBS REMAINING: %2d", BOMB_COUNT-return_flag_count(f));
  
  box(stats_w, 0, 0);
  wrefresh(stats_w);
}


void modes_draw(){
  if(modes_w == NULL)
    return;

  box(modes_w, 0, 0);

  char f = (FLAG_MODE == true) ? '>' : ' ';
  char r = (FLAG_MODE != true) ? '>' : ' ';
  
  mvwprintw(modes_w, 1, 1, "MODES: (press M to switch)");
  mvwprintw(modes_w, 2, 1, "%c REVEAL", r);
  mvwprintw(modes_w, 3, 1, "%c FLAG", f);
  
  wrefresh(modes_w);
}

void move_curs(int c, int *x, int *y){
  switch(c){
  case KEY_LEFT:
    *x -= 1;
    break;
  case KEY_RIGHT:
    *x += 1;
    break;
  case KEY_UP:
    *y -= 1;
    break;
  case KEY_DOWN:
    *y += 1;
    break;
  default:
    break;
  }

  *x = (*x < 0) ? 0 : *x;
  *x = (*x > f.size-1) ? f.size-1 : *x;

  
  *y = (*y < 0) ? 0 : *y;
  *y = (*y > f.size-1) ? f.size-1 : *y;
}

void game_start(){
  // generate the board
  field_generate(&f, BOMB_COUNT);
  state = STARTED;
  
  // player cursor "position"
  int curs_x = 5;
  int curs_y = 5;
  
  // enter the game loop
  while(true){

    clear();
    refresh();
    
    stats_draw();
    modes_draw();
    field_draw();
    refresh();
    
    move(field_w->_begy+1+(curs_y*2), field_w->_begx+1+(curs_x*2));

    int c = getch();

    switch(c){
    case KEY_LEFT:
    case KEY_RIGHT:
    case KEY_UP:
    case KEY_DOWN:
      move_curs(c, &curs_x, &curs_y);
      break;
    case 'm':
    case 'M':
      FLAG_MODE = (FLAG_MODE == false) ? true : false;
      break;
    case 'f':
    case 'F':
      if (FLAG_MODE == false)
	state = field_reveal(&f, curs_x, curs_y);
      else
	state = field_flag(&f, curs_x, curs_y);
      break;
      
    default:
      break;
    }

    if (state == WIN || state == LOSE)
      break;
  }
}
