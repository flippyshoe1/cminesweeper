#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TYPE_NORMAL 0
#define TYPE_BOMB  -1

#define TRUE    1
#define FALSE   0
#define FLAGGED 2

#define BOARDSIZE 9
#define BOMBCOUNT 9

#define BADINPUT 0
#define FINE     1
#define FAIL     2
#define WIN      3
#define START    4

typedef struct {
  int type;
  int revealed;
} tile;

typedef struct {
  int size;
  tile ** tiles;
} field;

field field_init(int size);
void field_generate(field * fp, int bomb_count);
void field_free(field * fp);
int field_iswithinborders(field f, int x, int y);
int field_dowithinborders(field f, int i);
int field_print(field f);
int field_reveal(field * fp, int x, int y);
int field_didiwin(field f);
int field_flag(field * fp, int x, int y);

void drawline(int length);
int getplayermove(field * fp);

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

field field_init(int s){
  field f;
  f.size = s;
  
  f.tiles = malloc(sizeof(tile *) * f.size);
  for(int i=0;i<f.size;i++)
    f.tiles[i] = malloc(sizeof(tile) * f.size);

  return f;
}

void field_generate(field * fp, int bc){
  // hide all the tiles
  for(int i=0;i<fp->size;i++)
    for(int j=0;j<fp->size;j++){
      fp->tiles[i][j].type = TYPE_NORMAL;
      fp->tiles[i][j].revealed = FALSE;
    }
  
  // "plant" bombs
  int x,y;
  for(int i=0;i<bc;i++) {
    do {
      x = rand() % fp->size-1;
      y = rand() % fp->size-1;
    } while(field_iswithinborders(*fp,x,y) == FALSE ||
	    fp->tiles[y][x].type == TYPE_BOMB);
    fp->tiles[y][x].type = TYPE_BOMB;
    
    // make the neighboring tiles "type" match accordingly
    int xf=field_dowithinborders(*fp, x+1);
    int yf=field_dowithinborders(*fp, y+1);

    int k,l;
    for(k=field_dowithinborders(*fp, y-1);k<=yf;k++)
      for(l=field_dowithinborders(*fp, x-1);l<=xf;l++){
	if(fp->tiles[k][l].type == TYPE_BOMB)
	  continue;
	fp->tiles[k][l].type += 1;
      }
  }
  
}

void field_free(field * fp){
  for(int i=0;i<fp->size;i++)
    free(fp->tiles[i]);

  free(fp->tiles);
  fp->size = 0;
}

int field_iswithinborders(field f, int x, int y){
  if (x>=0 && x<f.size && y>=0 && y<f.size)
    return TRUE;
  else
    return FALSE;
}

int field_dowithinborders(field f, int i){
  i = (i<0) ? 0 : i;
  i = (i>f.size) ? f.size : i;

  return i;
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

int field_reveal(field * fp, int x, int y){
  if(field_iswithinborders(*fp, x, y)==FALSE)
    return BADINPUT;
  else if(fp->tiles[y][x].revealed >= TRUE)
    return BADINPUT;

  fp->tiles[y][x].revealed = TRUE;
  
  if(fp->tiles[y][x].type == TYPE_BOMB)
    return FAIL;
  else if(fp->tiles[y][x].type > TYPE_NORMAL)
    return FINE;
  else{
    field_reveal(fp, x+1, y);
    field_reveal(fp, x, y+1);
    field_reveal(fp, x-1, y);
    field_reveal(fp, x, y-1);
    
    return FINE;
  } 
}

int field_didiwin(field f){
  for(int i=0;i<f.size;i++)
    for(int j=0;j<f.size;j++){
      if(f.tiles[i][j].type==TYPE_BOMB)
	continue;
      if(f.tiles[i][j].revealed==FALSE)
	return FINE;
    }

  return WIN;
}

int field_flag(field * fp, int x, int y){
  if(fp->tiles[y][x].revealed == FALSE)
    fp->tiles[y][x].revealed = (fp->tiles[y][x].revealed!=FLAGGED) ? FLAGGED : FALSE;

  return FINE;
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
