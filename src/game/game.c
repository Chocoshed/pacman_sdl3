#include "game.h"
#include "../render/renderer.h"
#include "../render/sprite.h"

/* ============================================================
 * HELPERS INTERNES
 * ============================================================ */

static void reset_positions(Game *game) {
    pacman_reset(&game->pacman);
    ghosts_reset(&game->ghosts);
}

static void clear_popups(Game *game) {
    for (int i = 0; i < SCORE_POPUP_MAX; i++)
        game->popups[i].timer = 0.0f;
}

static void start_new_game(Game *game) {
    score_init(&game->score);
    maze_init(&game->maze);
    reset_positions(game);
    clear_popups(game);
    game->phase       = PHASE_PLAY;
    game->phase_timer = 0.0f;
}

static void check_collisions(Game *game) {
    for (int i = 0; i < GHOST_COUNT; i++) {
        Ghost *g = &game->ghosts.ghosts[i];

        if (g->col != game->pacman.col || g->row != game->pacman.row)
            continue;

        if (g->mode == GHOST_FRIGHTENED) {
            int popup_idx = (game->score.ghost_combo < SCORE_POPUP_MAX)
                                ? game->score.ghost_combo : SCORE_POPUP_MAX - 1;
            ghost_set_eaten(g);
            score_eat_ghost(&game->score);
            for (int j = 0; j < SCORE_POPUP_MAX; j++) {
                if (game->popups[j].timer <= 0.0f) {
                    game->popups[j] = (ScorePopup){ g->col, g->row, popup_idx, SCORE_POPUP_DURATION };
                    break;
                }
            }
        } else if (g->mode == GHOST_SCATTER || g->mode == GHOST_CHASE) {
            score_lose_life(&game->score);
            game->phase       = (game->score.lives == 0) ? PHASE_GAME_OVER : PHASE_DYING;
            game->phase_timer = DEATH_PAUSE_DURATION;
            return;
        }
    }
}

/* ============================================================
 * MISE À JOUR PAR PHASE
 * ============================================================ */

static void update_title(Game *game) {
    if (game->input.confirm)
        game->phase = PHASE_MENU;
}

static void update_menu(Game *game) {
    if (game->input.nav_up)   game->menu.selected = MENU_NEW_GAME;
    if (game->input.nav_down) game->menu.selected = MENU_QUIT;

    if (game->input.confirm) {
        if (game->menu.selected == MENU_NEW_GAME)
            start_new_game(game);
        else
            game->should_quit = true;
    }
}

static void update_play(Game *game, float delta_time) {
    pacman_update(&game->pacman, &game->maze, &game->score,
                  input_get_direction(&game->input), delta_time);

    if (game->score.power_pellet_eaten) {
        game->score.power_pellet_eaten = false;
        game->score.ghost_combo        = 0;
        ghosts_set_frightened(&game->ghosts);
    }

    ghosts_update(&game->ghosts, &game->maze, &game->pacman, delta_time);
    maze_update_fruit(&game->maze, delta_time);

    if (maze_try_eat_fruit(&game->maze, game->pacman.col, game->pacman.row))
        score_add_points(&game->score, score_fruit_value(game->score.level));

    check_collisions(game);

    if (game->phase == PHASE_PLAY && maze_dots_remaining(&game->maze) == 0) {
        game->phase       = PHASE_LEVEL_CLEAR;
        game->phase_timer = LEVEL_PAUSE_DURATION;
    }

    for (int i = 0; i < SCORE_POPUP_MAX; i++)
        if (game->popups[i].timer > 0.0f)
            game->popups[i].timer -= delta_time;

    if (game->input.pause)
        game->phase = PHASE_PAUSE;
}

static void update_pause(Game *game) {
    if (game->input.pause)
        game->phase = PHASE_PLAY;
}

static void update_transition(Game *game, float delta_time) {
    game->phase_timer -= delta_time;
    if (game->phase_timer > 0.0f)
        return;

    if (game->phase == PHASE_LEVEL_CLEAR) {
        score_next_level(&game->score);
        maze_init(&game->maze);
    }
    reset_positions(game);
    game->phase = PHASE_PLAY;
}

static void update_game_over(Game *game) {
    if (game->input.confirm) {
        game->menu.selected = MENU_NEW_GAME;
        game->phase         = PHASE_MENU;
    }
}

/* ============================================================
 * RENDU PAR PHASE
 * ============================================================ */

static void render_gameplay(const Game *game, SDL_Renderer *renderer) {
    draw_maze(renderer, game->sheet, &game->maze);
    draw_pacman(renderer, game->sheet, &game->pacman);
    for (int i = 0; i < GHOST_COUNT; i++)
        draw_ghost(renderer, game->sheet, &game->ghosts.ghosts[i]);
    draw_fruit(renderer, game->sheet, &game->maze, game->score.level);
    draw_score_popups(renderer, game->sheet, game->popups, SCORE_POPUP_MAX);
    draw_hud(renderer, game->sheet, &game->score);
}

/* ============================================================
 * API PUBLIQUE
 * ============================================================ */

void game_init(Game *game) {
    maze_init(&game->maze);
    input_init(&game->input);
    pacman_init(&game->pacman);
    score_init(&game->score);
    ghosts_init(&game->ghosts);
    game->menu.selected = MENU_NEW_GAME;
    game->phase         = PHASE_TITLE;
    game->phase_timer   = 0.0f;
    game->should_quit   = false;
    game->sheet         = NULL;
    clear_popups(game);
}

bool game_load_assets(Game *game, SDL_Renderer *renderer) {
    game->sheet = sprite_load(renderer, "assets/spritesheetPacman.bmp");
    return game->sheet != NULL;
}

void game_quit(Game *game) {
    sprite_destroy(game->sheet);
    game->sheet = NULL;
}

void game_update(Game *game, float delta_time) {
    if (game->phase == PHASE_PLAY || game->phase == PHASE_PAUSE) {
        pacman_update_anim(&game->pacman, delta_time);
        ghosts_update_anims(&game->ghosts, delta_time);
    }

    switch (game->phase) {
        case PHASE_TITLE:       update_title(game);                break;
        case PHASE_MENU:        update_menu(game);                 break;
        case PHASE_PLAY:        update_play(game, delta_time);     break;
        case PHASE_PAUSE:       update_pause(game);                break;
        case PHASE_DYING:
        case PHASE_LEVEL_CLEAR: update_transition(game, delta_time); break;
        case PHASE_GAME_OVER:   update_game_over(game);            break;
    }

    input_clear_actions(&game->input);
}

void game_render(const Game *game, SDL_Renderer *renderer) {
    switch (game->phase) {
        case PHASE_TITLE:
            draw_title(renderer);
            break;

        case PHASE_MENU:
            draw_menu(renderer, (int)game->menu.selected);
            break;

        case PHASE_PLAY:
        case PHASE_LEVEL_CLEAR:
            render_gameplay(game, renderer);
            break;

        case PHASE_DYING: {
            float elapsed     = DEATH_PAUSE_DURATION - game->phase_timer;
            int   death_frame = (int)(elapsed / DEATH_PAUSE_DURATION * PACMAN_DEATH_FRAMES);
            if (death_frame >= PACMAN_DEATH_FRAMES) death_frame = PACMAN_DEATH_FRAMES - 1;
            draw_maze(renderer, game->sheet, &game->maze);
            draw_pacman_death(renderer, game->sheet, &game->pacman, death_frame);
            draw_hud(renderer, game->sheet, &game->score);
            break;
        }

        case PHASE_PAUSE:
            render_gameplay(game, renderer);
            draw_pause(renderer);
            break;

        case PHASE_GAME_OVER:
            render_gameplay(game, renderer);
            draw_game_over(renderer, &game->score);
            break;
    }
}
