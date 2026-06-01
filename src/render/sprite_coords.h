#pragma once

/*
 * Coordonnées des sprites dans la spritesheet (assets/spritesheet.bmp)
 * Format : { x, y, largeur, hauteur } en pixels
 *
 * À remplir avec les coordonnées relevées dans GIMP :
 *   Fenêtre → Informations → Pointeur
 *   Cliquer sur le coin supérieur gauche de chaque sprite
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
 * Les autres directions sont obtenues par rotation dans draw_pacman()
 * frame 0 : bouche fermée
 * frame 1 : mi-ouverte
 * frame 2 : grande ouverte
 * -------------------------------------------------------------------------*/
static const SDL_FRect PACMAN[3] = {
    { 103, 168, 16, 16 },  /* frame 0 : bouche fermée  — À COMPLÉTER */
    { 103, 151, 16, 16 },  /* frame 1 : mi-ouverte      — À COMPLÉTER */
    { 103, 134, 16, 16 },  /* frame 2 : grande ouverte  — À COMPLÉTER */
};

/* Angles de rotation par direction (utilisés avec SDL_RenderTextureRotated) */
static const double PACMAN_ANGLE[] = {
    [0] =   0.0,   /* DIR_NONE  → pas de rendu   */
    [1] = 270.0,   /* DIR_UP                      */
    [2] =  90.0,   /* DIR_DOWN                    */
    [3] = 180.0,   /* DIR_LEFT                    */
    [4] =   0.0,   /* DIR_RIGHT                   */
};

/* -------------------------------------------------------------------------
 * PAC-MAN — animation de mort (11 frames, pas de rotation)
 * Séquence jouée une seule fois quand Pac-Man perd une vie.
 * frame 0  : début (Pac-Man plein, bouche ouverte vers le haut)
 * frame 10 : fin   (Pac-Man complètement effacé)
 * -------------------------------------------------------------------------*/
#define PACMAN_DEATH_FRAMES 11

static const SDL_FRect PACMAN_DEATH[PACMAN_DEATH_FRAMES] = {
    { 1, 134, 16, 16 },   /* frame  0 */
    { 18, 134, 16, 16 },   /* frame  1 */
    { 35, 134, 16, 16 },   /* frame  2 */
    { 52, 134, 16, 16 },   /* frame  3 */
    { 69, 134, 16, 16 },   /* frame  4 */
    { 86, 134, 16, 16 },   /* frame  5 */
    { 1, 151, 16, 16 },   /* frame  6 */
    { 18, 151, 16, 16 },   /* frame  7 */
    { 35, 151, 16, 16 },   /* frame  8 */
    { 52, 151, 16, 16 },   /* frame  9 */
    { 69, 151, 16, 16 },   /* frame 10 */
};

/* -------------------------------------------------------------------------
 * FANTÔMES — déplacement (2 frames × 4 directions × 4 fantômes)
 * Indexé directement par les valeurs de l'enum Direction :
 *   DIR_UP=1, DIR_DOWN=2, DIR_LEFT=3, DIR_RIGHT=4
 * Accès : GHOST_SPRITES[ghost->id][ghost->dir][frame]
 * -------------------------------------------------------------------------*/

/* Blinky (rouge) — [5][2] : index 0 inutilisé (DIR_NONE) */
static const SDL_FRect GHOST_BLINKY[5][2] = {
    [1] /* DIR_UP    */ = {{ 103,83,16,16 }, { 120,83,16,16 }}, 
    [2] /* DIR_DOWN  */ = {{ 35,83,16,16 }, { 52,83,16,16 }}, 
    [3] /* DIR_LEFT  */ = {{ 69,83,16,16 }, { 86,83,16,16 }}, 
    [4] /* DIR_RIGHT */ = {{ 1,83,16,16 }, { 18,83,16,16 }}, 
};

/* Pinky (rose) */
static const SDL_FRect GHOST_PINKY[5][2] = {
    [1] /* DIR_UP    */ = {{ 303,83,16,16 }, { 320,83,16,16 }}, 
    [2] /* DIR_DOWN  */ = {{ 235,83,16,16 }, { 252,83,16,16 }}, 
    [3] /* DIR_LEFT  */ = {{ 269,83,16,16 }, { 286,83,16,16 }}, 
    [4] /* DIR_RIGHT */ = {{ 201,83,16,16 }, { 218,83,16,16 }}, 
};

/* Inky (cyan) */
static const SDL_FRect GHOST_INKY[5][2] = {
    [1] /* DIR_UP    */ = {{ 503,83,16,16 }, { 520,83,16,16 }}, 
    [2] /* DIR_DOWN  */ = {{ 435,83,16,16 }, { 452,83,16,16 }}, 
    [3] /* DIR_LEFT  */ = {{ 469,83,16,16 }, { 486,83,16,16 }}, 
    [4] /* DIR_RIGHT */ = {{ 401,83,16,16 }, { 418,83,16,16 }}, 
};

/* Clyde (orange) */
static const SDL_FRect GHOST_CLYDE[5][2] = {
    [1] /* DIR_UP    */ = {{ 703,83,16,16 }, { 720,83,16,16 }}, /* À COMPLÉTER */
    [2] /* DIR_DOWN  */ = {{ 635,83,16,16 }, { 652,83,16,16 }}, /* À COMPLÉTER */
    [3] /* DIR_LEFT  */ = {{ 669,83,16,16 }, { 686,83,16,16 }}, /* À COMPLÉTER */
    [4] /* DIR_RIGHT */ = {{ 601,83,16,16 }, { 618,83,16,16 }}, /* À COMPLÉTER */
};

/* tableau indexé par GhostId — accès : GHOST_SPRITES[ghost->id][ghost->dir][frame] */
static const SDL_FRect (* const GHOST_SPRITES[4])[2] = {
    GHOST_BLINKY,
    GHOST_PINKY,
    GHOST_INKY,
    GHOST_CLYDE,
};

/* -------------------------------------------------------------------------
 * FANTÔMES — modes spéciaux
 * -------------------------------------------------------------------------*/

/* Mode frightened (bleu) — 2 frames */
static const SDL_FRect GHOST_FRIGHTENED[2] = {
    { 201, 354, 16, 16 },   /* frame 0 — À COMPLÉTER */
    { 218, 354, 16, 16 },   /* frame 1 — À COMPLÉTER */
};

/* Mode frightened clignotant (blanc) — 2 frames */
static const SDL_FRect GHOST_FRIGHTENED_FLASH[2] = {
    { 1, 354, 16, 16 },   /* frame 0 — À COMPLÉTER */
    { 18, 354, 16, 16 },   /* frame 1 — À COMPLÉTER */
};

/* Mode eaten (yeux) — 1 frame par direction
 * Taille [5] : index 0 (DIR_NONE) inutilisé, accès : GHOST_EATEN[ghost->dir] */
static const SDL_FRect GHOST_EATEN[5] = {
    [1] /* DIR_UP    */ = { 303, 269, 16, 16 }, /* À COMPLÉTER */
    [2] /* DIR_DOWN  */ = { 235, 269, 16, 16 }, /* À COMPLÉTER */
    [3] /* DIR_LEFT  */ = { 269, 269, 16, 16 }, /* À COMPLÉTER */
    [4] /* DIR_RIGHT */ = { 201, 269, 16, 16 }, /* À COMPLÉTER */
};

/* -------------------------------------------------------------------------
 * SCORES — popup affiché quand Pac-Man mange un fantôme (8×8 px)
 * Indexé par le nombre de fantômes mangés dans la même séquence frightened :
 *   [0] = 200 pts  (1er fantôme)
 *   [1] = 400 pts  (2e)
 *   [2] = 800 pts  (3e)
 *   [3] = 1600 pts (4e)
 * -------------------------------------------------------------------------*/
#define SCORE_POPUP_COUNT 4

static const SDL_FRect SCORE_POPUP[SCORE_POPUP_COUNT] = {
    { 835, 354, 16, 16 },   /* 200  pts — À COMPLÉTER */
    { 852, 354, 16, 16 },   /* 400  pts — À COMPLÉTER */
    { 869, 354, 16, 16 },   /* 800  pts — À COMPLÉTER */
    { 886, 354, 16, 16 },   /* 1600 pts — À COMPLÉTER */
};

/* -------------------------------------------------------------------------
 * FRUITS — 1 frame par type, dans l'ordre des niveaux
 * -------------------------------------------------------------------------*/
static const SDL_FRect FRUIT_SPRITES[8] = {
    { 401, 489, 16, 16 },   /* niveau 1 : cerise    — À COMPLÉTER */
    { 418, 489, 16, 16 },   /* niveau 2 : fraise    — À COMPLÉTER */
    { 435, 489, 16, 16 },   /* niveau 3 : orange    — À COMPLÉTER */
    { 452, 489, 16, 16 },   /* niveau 4 : pomme     — À COMPLÉTER */
    { 469, 489, 16, 16 },   /* niveau 5 : melon     — À COMPLÉTER */
    { 486, 489, 16, 16 },   /* niveau 6 : galaxian  — À COMPLÉTER */
    { 503, 489, 16, 16 },   /* niveau 7 : cloche    — À COMPLÉTER */
    { 520, 489, 16, 16 },   /* niveau 8+: clé       — À COMPLÉTER */
};

/* -------------------------------------------------------------------------
 * LABYRINTHE — tuiles de sol
 * -------------------------------------------------------------------------*/
static const SDL_FRect TILE_DOT          = { 736, 196, 16, 16 };
static const SDL_FRect TILE_POWER_PELLET = { 736, 214, 16, 16 };

/* -------------------------------------------------------------------------
 * LABYRINTHE — tuiles de murs extérieurs (8×8 px, grille 4 col × 4 lignes)
 * Origine : (745, 214), pas : 9 px (8 px + 1 px espacement)
 * Murs droits (H/V) : TILE_GHOST_HOUSE[GHOST_H_HAUT/BAS] et [GHOST_V_GAUCHE/DROITE]
 *
 * Disposition dans la spritesheet :
 *   col 0  col 1  col 2  col 3
 *   [L0C0] [L0C1] [L0C2] [L0C3]   ligne 0 (y=214)
 *   [L1C0] [L1C1] [L1C2] [L1C3]   ligne 1 (y=223)
 *   [L2C0] [L2C1] [L2C2] [L2C3]   ligne 2 (y=232)
 *   [L3C0] [L3C1] [L3C2] [L3C3]   ligne 3 (y=241)
 * -------------------------------------------------------------------------*/
typedef enum {
    /* Ligne 0 : coins convexes (saillants vers l'extérieur) */
    WALL_CONV_HG =  0,   /* coin convexe haut-gauche */
    WALL_CONV_HD =  1,   /* coin convexe haut-droite */
    WALL_CONV_BD =  2,   /* coin convexe bas-droite  */
    WALL_CONV_BG =  3,   /* coin convexe bas-gauche  */
    /* Ligne 1 : coins concaves (rentrants vers l'intérieur) */
    WALL_CONC_HG =  4,   /* coin concave haut-gauche */
    WALL_CONC_HD =  5,   /* coin concave haut-droite */
    WALL_CONC_BD =  6,   /* coin concave bas-droite  */
    WALL_CONC_BG =  7,   /* coin concave bas-gauche  */
    /* Ligne 2 — à identifier (variante convexe ? arc plus grand ?) */
    WALL_L2_HG   =  8,
    WALL_L2_HD   =  9,
    WALL_L2_BD   = 10,
    WALL_L2_BG   = 11,
    /* Ligne 3 — à identifier (variante concave ?) */
    WALL_L3_HG   = 12,
    WALL_L3_HD   = 13,
    WALL_L3_BD   = 14,
    WALL_L3_BG   = 15,
    WALL_EXT_COUNT = 16,
} WallExtType;

static const SDL_FRect TILE_WALL_EXT[WALL_EXT_COUNT] = {
    /* ligne 0 : y = 214 */
    [WALL_CONV_HG] = { 745, 214, 8, 8 },
    [WALL_CONV_HD] = { 754, 214, 8, 8 },
    [WALL_CONV_BD] = { 763, 214, 8, 8 },
    [WALL_CONV_BG] = { 772, 214, 8, 8 },
    /* ligne 1 : y = 223 */
    [WALL_CONC_HG] = { 745, 223, 8, 8 },
    [WALL_CONC_HD] = { 754, 223, 8, 8 },
    [WALL_CONC_BD] = { 763, 223, 8, 8 },
    [WALL_CONC_BG] = { 772, 223, 8, 8 },
    /* ligne 2 : y = 232 */
    [WALL_L2_HG]   = { 745, 232, 8, 8 },
    [WALL_L2_HD]   = { 754, 232, 8, 8 },
    [WALL_L2_BD]   = { 763, 232, 8, 8 },
    [WALL_L2_BG]   = { 772, 232, 8, 8 },
    /* ligne 3 : y = 241 */
    [WALL_L3_HG]   = { 745, 241, 8, 8 },
    [WALL_L3_HD]   = { 754, 241, 8, 8 },
    [WALL_L3_BD]   = { 763, 241, 8, 8 },
    [WALL_L3_BG]   = { 772, 241, 8, 8 },
};

/* -------------------------------------------------------------------------
 * LABYRINTHE — murs droits (8×8 px)
 * Partagés entre le contour extérieur et la ghost house.
 * -------------------------------------------------------------------------*/
static const SDL_FRect TILE_WALL_H_HAUT   = { 781, 187, 8, 8 };  /* face haute (mur horizontal) */
static const SDL_FRect TILE_WALL_H_BAS    = { 781, 205, 8, 8 };  /* face basse                  */
static const SDL_FRect TILE_WALL_V_GAUCHE = { 790, 196, 8, 8 };  /* face gauche (mur vertical)  */
static const SDL_FRect TILE_WALL_V_DROITE = { 772, 196, 8, 8 };  /* face droite                 */

/* -------------------------------------------------------------------------
 * GHOST HOUSE — tuiles (8×8 px, grille 3×3)
 * Origine : (772, 187), pas : 9 px (8 px + 1 px espacement)
 * Les murs droits sont les mêmes sprites que le contour extérieur,
 * utilisés en sens inverse : GHOST_V_DROITE == TILE_WALL_V_GAUCHE, etc.
 *
 *   [COIN_HG]  [H_HAUT]   [COIN_HD]     y = 187
 *   [V_GAUCHE] [VIDE]     [V_DROITE]    y = 196
 *   [COIN_BG]  [H_BAS]    [COIN_BD]     y = 205
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
    [GHOST_COIN_HG]  = { 772, 187, 8, 8 },
    [GHOST_H_HAUT]   = { 781, 187, 8, 8 },  /* == TILE_WALL_H_HAUT */
    [GHOST_COIN_HD]  = { 790, 187, 8, 8 },
    [GHOST_V_GAUCHE] = { 772, 196, 8, 8 },  /* == TILE_WALL_V_DROITE */
    [GHOST_VIDE]     = { 781, 196, 8, 8 },
    [GHOST_V_DROITE] = { 790, 196, 8, 8 },  /* == TILE_WALL_V_GAUCHE */
    [GHOST_COIN_BG]  = { 772, 205, 8, 8 },
    [GHOST_H_BAS]    = { 781, 205, 8, 8 },  /* == TILE_WALL_H_BAS */
    [GHOST_COIN_BD]  = { 790, 205, 8, 8 },
};

static const SDL_FRect TILE_GHOST_DOOR = { 527, 223, 8, 8 };

/* -------------------------------------------------------------------------
 * LABYRINTHE — murs intérieurs (8×8 px, grille 3×3)
 * Origine : (745, 187), pas : 9 px (8 px + 1 px espacement)
 *
 *   [COIN_HG]  [H_HAUT]   [COIN_HD]     y = 187
 *   [V_GAUCHE] [VIDE]     [V_DROITE]    y = 196
 *   [COIN_BG]  [H_BAS]    [COIN_BD]     y = 205
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
    [MAZE_COIN_HG]  = { 745, 187, 8, 8 },
    [MAZE_H_HAUT]   = { 754, 187, 8, 8 },
    [MAZE_COIN_HD]  = { 763, 187, 8, 8 },
    [MAZE_V_GAUCHE] = { 745, 196, 8, 8 },
    [MAZE_VIDE]     = { 754, 196, 8, 8 },
    [MAZE_V_DROITE] = { 763, 196, 8, 8 },
    [MAZE_COIN_BG]  = { 745, 205, 8, 8 },
    [MAZE_H_BAS]    = { 754, 205, 8, 8 },
    [MAZE_COIN_BD]  = { 763, 205, 8, 8 },
};
