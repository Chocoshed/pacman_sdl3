#pragma once

#include <stdbool.h>

#define FRUIT_TYPES 8

typedef struct {
    int  score;
    int  high_score;
    int  lives;
    int  level;
    bool power_pellet_eaten;
    int  ghost_combo;
} Score;

extern const int FRUIT_VALUES[FRUIT_TYPES];

void score_init(Score *score);
void score_add_points(Score *score, int points);
void score_lose_life(Score *score);
void score_next_level(Score *score);
int  score_fruit_value(int level);
int  score_eat_ghost(Score *score);
