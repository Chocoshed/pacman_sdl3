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

/* -------------------------------------------------------------------------
 * SCORE POPUP — affiché brièvement à la position d'un fantôme mangé
 * -------------------------------------------------------------------------*/
#define SCORE_POPUP_MAX      4
#define SCORE_POPUP_DURATION 1.0f

typedef struct {
    int   col;
    int   row;
    int   sprite_idx; /* 0=200pts, 1=400pts, 2=800pts, 3=1600pts */
    float timer;      /* <= 0 : inactif */
} ScorePopup;

extern const int FRUIT_VALUES[FRUIT_TYPES];

void score_init(Score *score);
void score_add_points(Score *score, int points);
void score_lose_life(Score *score);
void score_next_level(Score *score);
int  score_fruit_value(int level);
int  score_eat_ghost(Score *score);
