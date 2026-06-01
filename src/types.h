#pragma once

/*
 * Types primitifs partagés entre les modules du jeu.
 * Ce fichier ne dépend d'aucun autre header du projet.
 */

typedef enum {
    DIR_NONE  = 0,
    DIR_UP    = 1,
    DIR_DOWN  = 2,
    DIR_LEFT  = 3,
    DIR_RIGHT = 4,
} Direction;
