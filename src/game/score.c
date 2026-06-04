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
    score->score              = 0;
    score->high_score         = 0;
    score->lives              = 3;
    score->level              = 1;
    score->power_pellet_eaten = false;
    score->ghost_combo        = 0;
    score->fruit_history_count = 0;
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
}

int score_eat_ghost(Score *score) {
    static const int COMBO_VALUES[4] = { 200, 400, 800, 1600 };
    int idx = score->ghost_combo < 4 ? score->ghost_combo : 3;
    int pts = COMBO_VALUES[idx];
    if (score->ghost_combo < 3)
        score->ghost_combo++;
    score_add_points(score, pts);
    return pts;
}

int score_fruit_value(int level) {
    int idx = level - 1;
    if (idx >= FRUIT_TYPES) idx = FRUIT_TYPES - 1;
    return FRUIT_VALUES[idx];
}

void score_record_fruit(Score *score, int level) {
    int sprite_idx = (level - 1 < FRUIT_TYPES) ? level - 1 : FRUIT_TYPES - 1;
    if (score->fruit_history_count < FRUIT_HISTORY_MAX) {
        score->fruit_history[score->fruit_history_count++] = sprite_idx;
    } else {
        for (int i = 0; i < FRUIT_HISTORY_MAX - 1; i++)
            score->fruit_history[i] = score->fruit_history[i + 1];
        score->fruit_history[FRUIT_HISTORY_MAX - 1] = sprite_idx;
    }
}
