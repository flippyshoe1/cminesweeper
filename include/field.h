#ifndef FIELD_H
#define FIELD_H

// special bomb types
#define TYPE_NORMAL 0
#define TYPE_BOMB  -1

// flag types?
#define TRUE    1
#define FALSE   0
#define FLAGGED 2

// return codes apparently
#define BADINPUT 0
#define FINE     1
#define FAIL     2
#define WIN      3
#define START    4
// TODO: get rid of this ASAP

typedef struct {
  int type;
  int revealed;
} tile;

typedef struct {
  int size;
  tile ** tiles;
} field;


// TODO: ...too many functions
// clean it up!!!
field field_init(int size);
void field_generate(field * fp, int bomb_count);
void field_free(field * fp);
int field_iswithinborders(field f, int x, int y);
int field_dowithinborders(field f, int i);
int field_print(field f);
int field_reveal(field * fp, int x, int y);
int field_didiwin(field f);
int field_flag(field * fp, int x, int y);

#endif
