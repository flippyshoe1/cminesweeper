#include "game.h"
#include "field.h"

#include <ncurses.h>

int main(){
  game_init(BOARD_SIZE);

  game_start();
  
  game_free();
  
  return 0;
}
