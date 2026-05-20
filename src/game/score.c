#include "score.h"

void score_init(Score *score) {
    score->score        = 0;
    score->high_score   = 0;
    score->lives        = 3;
    score->level        = 1;
    score->power_active = false;
}

void score_add_points(Score *score, int points) {
    score->score += points;
    if (score->score > score->high_score)
        score->high_score = score->score;
}

void score_lose_life(Score *score) {
    if (score->lives > 0)
        score->lives--;
}

void score_next_level(Score *score) {
    score->level++;
    score->score = 0;
}
