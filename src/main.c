#include "field.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define BOARDSIZE 9
#define BOMBCOUNT 9

bool field_print(field f);

void drawline(int length);
int getplayermove(field * fp);

// TODO: get rid of the self made io
// and just use ncurses
// this is too ugly
int main(){
  srand(time(0));
  field f = field_init(BOARDSIZE);
  field_generate(&f, BOMBCOUNT);

  game_state res = STARTED; //res is the result of the move (start just means program was started)
  while(1){
    system("clear");
    field_print(f);

    switch(res){
    case STARTED:
      printf("welcome to cminesweeper!");
      break;
    case LOSE:
      printf("tough luck, you lost...");
      break;
    case WIN:
      printf("congratulations! you won!");
      break;
    case ONGOING:
      break;
    default:
      printf("sorry? didnt get that. (invalid input)");
      break;
    }

    printf("\n\n");

    if(res==LOSE || res==WIN)
      break;

    res = getplayermove(&f);
  }
  
  field_free(&f);
  return 0;
}

bool field_print(field f){
  printf(" |");
  for(int i=0;i<f.size;i++)
    printf("%d|",i);
  printf("\n");
  drawline(f.size*2+2);

  for(int y=0;y<f.size;y++){
    printf("%d|",y);

    for(int x=0;x<f.size;x++){
      if(f.tiles[y][x].revealed == HIDDEN)
	printf("#|");
      else if (f.tiles[y][x].revealed == FLAGGED)
	printf("F|");
      else if (f.tiles[y][x].type == TYPE_BOMB)
	printf("@|");
      else
	printf("%d|",f.tiles[y][x].type);
    }

    printf("\n");
    drawline(f.size*2+2);
  }

  printf("\n");
  return true;
}

void drawline(int length){
  for(int i=0;i<length;i++)
    printf("-");
  printf("\n");
}

int getplayermove(field * fp){
  drawline((fp->size)*6);
  printf("Please enter your next move.\n");
  printf("You write your move in in the following format:\n\t<MODE><X><Y> (e.g. f35)\n");
  printf("\n<MODE>:\n\tthe type of move you wanna commit\n\t(flag = f, reveal = r)\n");
  printf("<X>/<Y>:\n\tcoordinates on the board you wish to make the move upon.\n");

  drawline((fp->size)*6);
  printf("> ");

  char input[4];
  scanf("%3s", input);

  game_state (*method)(field *, int, int) = NULL;
  switch(input[0]){
  case 'f':
  case 'F':
    method = field_flag;
    break;
  case 'r':
  case 'R':
    method = field_reveal;
    break;
  default:
    return ONGOING;
  }

  if(method == NULL)
    return ONGOING;

  if(input[1]-'0'>10 &&
     input[2]-'0'>10)
    return ONGOING;

  game_state res = method(fp, input[1]-'0', input[2]-'0');
  res = (res==ONGOING) ? field_didiwin(*fp) : res;
    
  return res;
}
