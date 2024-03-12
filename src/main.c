#include "field.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOARDSIZE 9
#define BOMBCOUNT 9

int field_print(field f);

void drawline(int length);
int getplayermove(field * fp);

// TODO: get rid of the self made io
// and just use ncurses
// this is too ugly
int main(){
  srand(time(0));
  field f = field_init(BOARDSIZE);
  field_generate(&f, BOMBCOUNT);

  int res = START; //res is the result of the move (start just means program was started)
  while(1){
    system("clear");
    field_print(f);

    switch(res){
    case START:
      printf("welcome to cminesweeper!");
      break;
    case FAIL:
      printf("tough luck, you lost...");
      break;
    case WIN:
      printf("congratulations! you won!");
      break;
    case FINE:
      break;
    case BADINPUT:
    default:
      printf("sorry? didnt get that. (invalid input)");
      break;
    }

    printf("\n\n");

    if(res==FAIL || res==WIN)
      break;

    res = getplayermove(&f);
  }
  
  field_free(&f);
  return 0;
}

int field_print(field f){
  printf(" |");
  for(int i=0;i<f.size;i++)
    printf("%d|",i);
  printf("\n");
  drawline(f.size*2+2);

  for(int y=0;y<f.size;y++){
    printf("%d|",y);

    for(int x=0;x<f.size;x++){
      if(f.tiles[y][x].revealed == FALSE)
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
  return TRUE;
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

  int (*method)(field *, int, int) = NULL;
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
    return BADINPUT;
  }

  if(method == NULL)
    return BADINPUT;

  if(input[1]-'0'>10 &&
     input[2]-'0'>10)
    return BADINPUT;

  int res = method(fp, input[1]-'0', input[2]-'0');
  res = (res==FINE) ? field_didiwin(*fp) : res;
    
  return res;
}
