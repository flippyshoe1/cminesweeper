#ifndef GAME_H
#define GAME_H

// temp macros
#define BOARD_SIZE 9
#define BOMB_COUNT 9

void game_init(int field_size);
void game_free();

void game_start();

void field_draw();
void stats_draw();
void modes_draw();

#endif
