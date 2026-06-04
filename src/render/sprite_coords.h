#pragma once

/*
 * Coordonnées des sprites dans la spritesheet (assets/spritesheetPacman.bmp)
 * Format : { x, y, largeur, hauteur } en pixels
 *
 * STRATÉGIE DE ROTATION (SDL_RenderTextureRotated) :
 *   Pac-Man : un seul jeu de frames (direction droite), rotation appliquée :
 *     droite →   0°
 *     bas    →  90°
 *     gauche → 180°
 *     haut   → 270°
 */

#include <SDL3/SDL.h>

/* -------------------------------------------------------------------------
 * PAC-MAN — direction droite uniquement (3 frames)
 * frame 0 : bouche fermée
 * frame 1 : mi-ouverte
 * frame 2 : grande ouverte
 * -------------------------------------------------------------------------*/
static const SDL_FRect PACMAN[3] = {
    {301, 726, 16, 16}, /* frame 0 : bouche fermée  */
    {301, 709, 16, 16}, /* frame 1 : mi-ouverte      */
    {301, 692, 16, 16}, /* frame 2 : grande ouverte  */
};

/* Angles de rotation par direction (utilisés avec SDL_RenderTextureRotated) */
static const double PACMAN_ANGLE[] = {
    [0] = 0.0,   /* DIR_NONE  */
    [1] = 270.0, /* DIR_UP    */
    [2] = 90.0,  /* DIR_DOWN  */
    [3] = 180.0, /* DIR_LEFT  */
    [4] = 0.0,   /* DIR_RIGHT */
};

/* -------------------------------------------------------------------------
 * PAC-MAN — animation de mort (11 frames, sans rotation)
 * -------------------------------------------------------------------------*/
static const SDL_FRect PACMAN_DEATH[11] = {
    {401, 320, 16, 16},  /* frame  0 */
    {418, 320, 16, 16}, /* frame  1 */
    {435, 320, 16, 16}, /* frame  2 */
    {452, 320, 16, 16}, /* frame  3 */
    {469, 320, 16, 16}, /* frame  4 */
    {486, 320, 16, 16}, /* frame  5 */
    {401, 337, 16, 16},  /* frame  6 */
    {418, 337, 16, 16}, /* frame  7 */
    {435, 337, 16, 16}, /* frame  8 */
    {452, 337, 16, 16}, /* frame  9 */
    {469, 337, 16, 16}, /* frame 10 */
};

/* -------------------------------------------------------------------------
 * FANTÔMES — déplacement (2 frames × 4 directions × 4 fantômes)
 * Indexé par Direction : DIR_UP=1, DIR_DOWN=2, DIR_LEFT=3, DIR_RIGHT=4
 * Accès : GHOST_SPRITES[ghost->id][ghost->dir][frame]
 * -------------------------------------------------------------------------*/

static const SDL_FRect SPRITE_BLINKY[5][2] = {
    [1] /* DIR_UP    */ = {{103, 83, 16, 16}, {120, 83, 16, 16}},
    [2] /* DIR_DOWN  */ = {{35, 83, 16, 16}, {52, 83, 16, 16}},
    [3] /* DIR_LEFT  */ = {{69, 83, 16, 16}, {86, 83, 16, 16}},
    [4] /* DIR_RIGHT */ = {{1, 83, 16, 16}, {18, 83, 16, 16}},
};

static const SDL_FRect SPRITE_PINKY[5][2] = {
    [1] /* DIR_UP    */ = {{303, 83, 16, 16}, {320, 83, 16, 16}},
    [2] /* DIR_DOWN  */ = {{235, 83, 16, 16}, {252, 83, 16, 16}},
    [3] /* DIR_LEFT  */ = {{269, 83, 16, 16}, {286, 83, 16, 16}},
    [4] /* DIR_RIGHT */ = {{201, 83, 16, 16}, {218, 83, 16, 16}},
};

static const SDL_FRect SPRITE_INKY[5][2] = {
    [1] /* DIR_UP    */ = {{503, 83, 16, 16}, {520, 83, 16, 16}},
    [2] /* DIR_DOWN  */ = {{435, 83, 16, 16}, {452, 83, 16, 16}},
    [3] /* DIR_LEFT  */ = {{469, 83, 16, 16}, {486, 83, 16, 16}},
    [4] /* DIR_RIGHT */ = {{401, 83, 16, 16}, {418, 83, 16, 16}},
};

static const SDL_FRect SPRITE_CLYDE[5][2] = {
    [1] /* DIR_UP    */ = {{703, 83, 16, 16}, {720, 83, 16, 16}},
    [2] /* DIR_DOWN  */ = {{635, 83, 16, 16}, {652, 83, 16, 16}},
    [3] /* DIR_LEFT  */ = {{669, 83, 16, 16}, {686, 83, 16, 16}},
    [4] /* DIR_RIGHT */ = {{601, 83, 16, 16}, {618, 83, 16, 16}},
};

/* Indexé par GhostId — accès : GHOST_SPRITES[ghost->id][ghost->dir][frame] */
static const SDL_FRect (*const GHOST_SPRITES[4])[2] = {
    SPRITE_BLINKY,
    SPRITE_PINKY,
    SPRITE_INKY,
    SPRITE_CLYDE,
};

/* -------------------------------------------------------------------------
 * FANTÔMES — modes spéciaux
 * -------------------------------------------------------------------------*/

/* Mode frightened (bleu) — 2 frames */
static const SDL_FRect SPRITE_FRIGHTENED[2] = {
    {201, 354, 16, 16},
    {218, 354, 16, 16},
};

/* Mode frightened clignotant (blanc) — 2 frames */
static const SDL_FRect SPRITE_FRIGHTENED_FLASH[2] = {
    {1, 354, 16, 16},
    {18, 354, 16, 16},
};

/* Mode eaten (yeux) — 1 frame par direction
 * Taille [5] : index 0 (DIR_NONE) inutilisé, accès : SPRITE_EATEN[ghost->dir] */
static const SDL_FRect SPRITE_EATEN[5] = {
    [1] /* DIR_UP    */ = {303, 269, 16, 16},
    [2] /* DIR_DOWN  */ = {235, 269, 16, 16},
    [3] /* DIR_LEFT  */ = {269, 269, 16, 16},
    [4] /* DIR_RIGHT */ = {201, 269, 16, 16},
};

/* -------------------------------------------------------------------------
 * SCORES — popup quand Pac-Man mange un fantôme
 * [0]=200 pts, [1]=400 pts, [2]=800 pts, [3]=1600 pts
 * -------------------------------------------------------------------------*/
#define SCORE_POPUP_COUNT 4

static const SDL_FRect SCORE_POPUP[SCORE_POPUP_COUNT] = {
    {835, 354, 16, 16}, /*  200 pts */
    {852, 354, 16, 16}, /*  400 pts */
    {869, 354, 16, 16}, /*  800 pts */
    {886, 354, 16, 16}, /* 1600 pts */
};

/* -------------------------------------------------------------------------
 * FRUITS — 1 frame par type, dans l'ordre des niveaux
 * -------------------------------------------------------------------------*/
static const SDL_FRect FRUIT_SPRITES[8] = {
    {401, 489, 16, 16}, /* niveau 1 : cerise   */
    {418, 489, 16, 16}, /* niveau 2 : fraise   */
    {435, 489, 16, 16}, /* niveau 3 : orange   */
    {452, 489, 16, 16}, /* niveau 4 : pomme    */
    {469, 489, 16, 16}, /* niveau 5 : melon    */
    {486, 489, 16, 16}, /* niveau 6 : galaxian */
    {503, 489, 16, 16}, /* niveau 7 : cloche   */
    {520, 489, 16, 16}, /* niveau 8+: clé      */
};

/* -------------------------------------------------------------------------
 * LABYRINTHE — tuiles de sol
 * -------------------------------------------------------------------------*/
static const SDL_FRect TILE_DOT          = {736, 196, 8, 8};
static const SDL_FRect TILE_POWER_PELLET = {736, 214, 8, 8};

/* -------------------------------------------------------------------------
 * LABYRINTHE — tuiles de murs extérieurs (8×8 px, grille 4×4)
 * -------------------------------------------------------------------------*/
typedef enum {
    WALL_CONV_HG =  0,
    WALL_CONV_HD =  1,
    WALL_CONV_BD =  2,
    WALL_CONV_BG =  3,
    WALL_CONC_HG =  4,
    WALL_CONC_HD =  5,
    WALL_CONC_BD =  6,
    WALL_CONC_BG =  7,
    WALL_L2_HG   =  8,
    WALL_L2_HD   =  9,
    WALL_L2_BD   = 10,
    WALL_L2_BG   = 11,
    WALL_L3_HG   = 12,
    WALL_L3_HD   = 13,
    WALL_L3_BD   = 14,
    WALL_L3_BG   = 15,
    WALL_EXT_COUNT = 16,
} WallExtType;

static const SDL_FRect TILE_WALL_EXT[WALL_EXT_COUNT] = {
    [WALL_CONV_HG] = {745, 214, 8, 8},
    [WALL_CONV_HD] = {754, 214, 8, 8},
    [WALL_CONV_BD] = {763, 214, 8, 8},
    [WALL_CONV_BG] = {772, 214, 8, 8},
    [WALL_CONC_HG] = {745, 223, 8, 8},
    [WALL_CONC_HD] = {754, 223, 8, 8},
    [WALL_CONC_BD] = {763, 223, 8, 8},
    [WALL_CONC_BG] = {772, 223, 8, 8},
    [WALL_L2_HG]   = {745, 232, 8, 8},
    [WALL_L2_HD]   = {754, 232, 8, 8},
    [WALL_L2_BD]   = {763, 232, 8, 8},
    [WALL_L2_BG]   = {772, 232, 8, 8},
    [WALL_L3_HG]   = {745, 241, 8, 8},
    [WALL_L3_HD]   = {754, 241, 8, 8},
    [WALL_L3_BD]   = {763, 241, 8, 8},
    [WALL_L3_BG]   = {772, 241, 8, 8},
};

/* -------------------------------------------------------------------------
 * LABYRINTHE — murs droits (8×8 px)
 * -------------------------------------------------------------------------*/
static const SDL_FRect TILE_WALL_H_HAUT   = {781, 187, 8, 8};
static const SDL_FRect TILE_WALL_H_BAS    = {781, 205, 8, 8};
static const SDL_FRect TILE_WALL_V_GAUCHE = {790, 196, 8, 8};
static const SDL_FRect TILE_WALL_V_DROITE = {772, 196, 8, 8};

/* -------------------------------------------------------------------------
 * GHOST HOUSE — tuiles (8×8 px, grille 3×3)
 * -------------------------------------------------------------------------*/
typedef enum {
    GHOST_COIN_HG  = 0,
    GHOST_H_HAUT   = 1,
    GHOST_COIN_HD  = 2,
    GHOST_V_GAUCHE = 3,
    GHOST_VIDE     = 4,
    GHOST_V_DROITE = 5,
    GHOST_COIN_BG  = 6,
    GHOST_H_BAS    = 7,
    GHOST_COIN_BD  = 8,
    GHOST_TILE_COUNT = 9,
} GhostHouseTile;

static const SDL_FRect TILE_GHOST_HOUSE[GHOST_TILE_COUNT] = {
    [GHOST_COIN_HG]  = {772, 187, 8, 8},
    [GHOST_H_HAUT]   = {781, 187, 8, 8},
    [GHOST_COIN_HD]  = {790, 187, 8, 8},
    [GHOST_V_GAUCHE] = {772, 196, 8, 8},
    [GHOST_VIDE]     = {781, 196, 8, 8},
    [GHOST_V_DROITE] = {790, 196, 8, 8},
    [GHOST_COIN_BG]  = {772, 205, 8, 8},
    [GHOST_H_BAS]    = {781, 205, 8, 8},
    [GHOST_COIN_BD]  = {790, 205, 8, 8},
};

static const SDL_FRect TILE_GHOST_DOOR = {527, 223, 8, 8};

/* -------------------------------------------------------------------------
 * LABYRINTHE — murs intérieurs (8×8 px, grille 3×3)
 * -------------------------------------------------------------------------*/
typedef enum {
    MAZE_COIN_HG  = 0,
    MAZE_H_HAUT   = 1,
    MAZE_COIN_HD  = 2,
    MAZE_V_GAUCHE = 3,
    MAZE_VIDE     = 4,
    MAZE_V_DROITE = 5,
    MAZE_COIN_BG  = 6,
    MAZE_H_BAS    = 7,
    MAZE_COIN_BD  = 8,
    MAZE_TILE_COUNT = 9,
} MazeTile;

static const SDL_FRect TILE_MAZE[MAZE_TILE_COUNT] = {
    [MAZE_COIN_HG]  = {745, 187, 8, 8},
    [MAZE_H_HAUT]   = {754, 187, 8, 8},
    [MAZE_COIN_HD]  = {763, 187, 8, 8},
    [MAZE_V_GAUCHE] = {745, 196, 8, 8},
    [MAZE_VIDE]     = {754, 196, 8, 8},
    [MAZE_V_DROITE] = {763, 196, 8, 8},
    [MAZE_COIN_BG]  = {745, 205, 8, 8},
    [MAZE_H_BAS]    = {754, 205, 8, 8},
    [MAZE_COIN_BD]  = {763, 205, 8, 8},
};

/* -------------------------------------------------------------------------
 * GLYPHS — caractères spéciaux (8×8 px)
 * -------------------------------------------------------------------------*/
static const SDL_FRect GLYPH_SLASH   = {891, 196, 8, 8};
static const SDL_FRect GLYPH_DASH    = {900, 196, 8, 8};
static const SDL_FRect GLYPH_APOS    = {909, 196, 8, 8};
static const SDL_FRect GLYPH_DQUOTE  = {891, 205, 8, 8};
static const SDL_FRect GLYPH_AT      = {900, 205, 8, 8};
static const SDL_FRect GLYPH_EXCLAIM = {909, 205, 8, 8};

/* -------------------------------------------------------------------------
 * GLYPHS — chiffres 0-9 (8×8 px)
 * -------------------------------------------------------------------------*/
static const SDL_FRect GLYPH_DIGIT[10] = {
    [0] = {801, 187, 8, 8},
    [1] = {810, 187, 8, 8},
    [2] = {819, 187, 8, 8},
    [3] = {828, 187, 8, 8},
    [4] = {837, 187, 8, 8},
    [5] = {846, 187, 8, 8},
    [6] = {855, 187, 8, 8},
    [7] = {864, 187, 8, 8},
    [8] = {873, 187, 8, 8},
    [9] = {882, 187, 8, 8},
};

/* -------------------------------------------------------------------------
 * GLYPHS — lettres A-Z (8×8 px)
 * -------------------------------------------------------------------------*/
static const SDL_FRect GLYPH_LETTER[26] = {
    {801, 214, 8, 8}, /* A */
    {810, 214, 8, 8}, /* B */
    {819, 214, 8, 8}, /* C */
    {828, 214, 8, 8}, /* D */
    {837, 214, 8, 8}, /* E */
    {846, 214, 8, 8}, /* F */
    {855, 214, 8, 8}, /* G */
    {864, 214, 8, 8}, /* H */
    {873, 214, 8, 8}, /* I */
    {882, 214, 8, 8}, /* J */
    {891, 214, 8, 8}, /* K */
    {900, 214, 8, 8}, /* L */
    {909, 214, 8, 8}, /* M */
    {801, 223, 8, 8}, /* N */
    {810, 223, 8, 8}, /* O */
    {819, 223, 8, 8}, /* P */
    {828, 223, 8, 8}, /* Q */
    {837, 223, 8, 8}, /* R */
    {846, 223, 8, 8}, /* S */
    {855, 223, 8, 8}, /* T */
    {864, 223, 8, 8}, /* U */
    {873, 223, 8, 8}, /* V */
    {882, 223, 8, 8}, /* W */
    {891, 223, 8, 8}, /* X */
    {900, 223, 8, 8}, /* Y */
    {909, 223, 8, 8}, /* Z */
};
