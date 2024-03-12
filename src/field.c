#include "field.h"

#include <stdlib.h>
#include <time.h>

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
