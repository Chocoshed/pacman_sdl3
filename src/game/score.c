#include "score.h"

const int FRUIT_VALUES[FRUIT_TYPES] = {
    100,   /* niveau 1 : cerise   */
    300,   /* niveau 2 : fraise   */
    500,   /* niveau 3 : orange   */
    700,   /* niveau 4 : pomme    */
    1000,  /* niveau 5 : melon    */
    2000,  /* niveau 6 : galaxian */
    3000,  /* niveau 7 : cloche   */
    5000,  /* niveau 8+ : clé     */
};

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

int score_fruit_value(int level) {
    int idx = level - 1;
    if (idx >= FRUIT_TYPES) idx = FRUIT_TYPES - 1;
    return FRUIT_VALUES[idx];
}
