#include <ncurses.h>
#include <string.h>
#include <stdbool.h>

// temp macros
#define BOARD_SIZE 9
#define BOMB_COUNT 9

// windows to be used
WINDOW *field_w = newwin(BOARD_SIZE*2+1, BOARD_SIZE*2+1, 1, 1);
WINDOW *stats_w = newwin(20, 4, (field_w->_begx+field_w->_maxx)+1, 1);
WINDOW *modes_w = newwin(26, 3, (field_w->_begx+field_w->_maxx)+1, (stats_w->_begy+stats_w->_maxy)+1);
