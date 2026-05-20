#pragma once

#include <stdbool.h>

typedef struct {
    int  score;
    int  high_score;
    int  lives;
    int  level;
    bool power_active;
} Score;

void score_init(Score *score);
void score_add_points(Score *score, int points);
void score_lose_life(Score *score);
void score_next_level(Score *score);
