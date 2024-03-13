#include <stdbool.h>

#ifndef FIELD_H
#define FIELD_H

// special bomb types
#define TYPE_NORMAL 0
#define TYPE_BOMB  -1

// flag types
#define REVEALED 1
#define HIDDEN   0
#define FLAGGED  2

typedef enum {
  STARTED, ONGOING, LOSE, WIN
} game_state;


typedef struct {
  int type;
  int revealed;
} tile;

typedef struct {
  int size;
  tile ** tiles;
} field;


// function that returns an "empty" field struct with preset defaults
// input: size (int) - size (horizontal and veritcal) of the field
// output: the field itself
field field_init(int size);

// function that modifies the field, "planting bombs", and adjusting nearby tiles, to suit the rules of minesweeper
// input: fp (field pointer) - the field you wish to modify
//        bomb_count (int)   - the amount of "bombs" you want in said field
// output: nothing
void field_generate(field * fp, int bomb_count);

// frees the allocated space for field, rendering it unplayable (you may still access the field itself, just not .tiles)
// input: fp (field pointer) - the field you wish to deallocate
// output: nothing
void field_free(field * fp);

// checks if an x-y coordinate is within the borders of a field
// input: f (field) - field you wish to test upon
//        x,y (int) - coordinates to test
// output: boolean (true if within, false if not)
bool field_iswithinborders(field f, int x, int y);

// same as field_iswithinborders, but modifies a number to count within the size of the field
// input: f (field) - field you wish to.. test upon?
//        i (int)   - the number (could be either x or y) to suit the field size
// output: int that suits the size of .size (no greater than size, no smaller than 0)
int field_dowithinborders(field f, int i);

// "reveal" one of the tiles in the field.
// if tile is TYPE_BOMB, return LOSE game_state
// if tile is TYPE_NORMAL, recursively call this function to all nearby tiles until all TYPE_NORMAL tiles are revealed
// input: fp (field pointer) - field you wish to modify
//        x,y (int)          - coordinates of tile you wish to reveal
// output: the new game_state of the game (either you lose or game keeps going)
game_state field_reveal(field * fp, int x, int y);

// checks whether all tiles who are not TYPE_BOMB have been revealed, returns WIN game_state if so
// input: f (field) - field you wish to test upon
// output: the new game_state of the game (either you win or the game keeps going)
game_state field_didiwin(field f);

// flags one of the tiles in the field, making them "unrevealable" unless unflagged
// call this function on a flagged tile to unflag it
// input: fp (field pointer) - field you wish to modify
//        x,y (int)          - coordinates of tile you wish to reveal
// output: it returns the ONGOING game_state every time, soley to be compatible with the function pointer in main.c
// may be changed later?
game_state field_flag(field * fp, int x, int y);

// returns the amount of flags in a field
// input: f (field) - field you wish to test upon
// output: amount of flags in the field
int return_flag_count(field f);

#endif
