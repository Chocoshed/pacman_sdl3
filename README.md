# 🎮 PAC-MAN Remake

Remake fidèle du jeu légendaire **PAC-MAN** de Namco (1980), développé en C avec SDL3.

![PAC-MAN](https://img.shields.io/badge/PAC--MAN-1980-yellow?style=for-the-badge)
![C](https://img.shields.io/badge/Language-C-blue?style=for-the-badge&logo=c)
![SDL3](https://img.shields.io/badge/SDL-3-green?style=for-the-badge)

---

## 📋 Description du projet

Ce projet vise à recréer l'expérience originale de PAC-MAN avec la plus grande fidélité possible. L'objectif est de reproduire le gameplay, les mécaniques et l'esthétique du jeu original sorti en arcade en 1980.

### Objectifs principaux

- ✅ Reproduction fidèle du jeu original PAC-MAN
- ✅ Développement en langage C
- ✅ Affichage graphique avec SDL3
- ✅ Gestion des sprites à partir de planches BMP
- ✅ Gestion précise de la vitesse du jeu
- ✅ Compilation multi-plateforme (Linux minimum)

---

## 🎯 Fonctionnalités attendues

### Gameplay

- [ ] Contrôle du personnage PAC-MAN
- [ ] Labyrinthe fidèle à l'original
- [ ] Système de collecte de pastilles (pac-dots)
- [ ] Pastilles de puissance (power pellets)
- [ ] Comportement intelligent des fantômes (IA)
- [ ] Système de score
- [ ] Système de vies
- [ ] Gestion des niveaux
- [ ] Écrans de démarrage et game over
- [ ] Sons et effets sonores (optionnel)

### Fantômes

Chaque fantôme doit avoir son propre comportement :
- **Blinky** (rouge) : Poursuit directement PAC-MAN
- **Pinky** (rose) : Tente d'embusquer PAC-MAN
- **Inky** (cyan) : Comportement basé sur Blinky et PAC-MAN
- **Clyde** (orange) : Comportement alternatif

---

## 🛠️ Technologies utilisées

- **Langage** : C
- **Bibliothèque graphique** : SDL3 (Simple DirectMedia Layer)
- **Format des sprites** : BMP
- **Build system** : CMake
- **Plateforme cible** : Linux (minimum), multi-plateforme idéalement

---

## 📦 Compilation et exécution

### Prérequis

- Compilateur C (gcc/clang)
- CMake (version 3.x ou supérieure)
- SDL3

### Instructions de compilation

```bash
# Depuis le répertoire racine du projet
mkdir -p build
cd build
cmake ..
make
```

### Lancement du jeu

```bash
# Depuis le répertoire build
./Projet
```

---

## 📁 Structure du projet

```
pacman_sdl3/
├── main.c              # Point d'entrée du programme
├── CMakeLists.txt      # Configuration CMake
├── consignes.txt       # Consignes du projet
├── README.md           # Ce fichier
└── build/              # Dossier de compilation
```

---

## 📚 Ressources

### Documentation officielle

- [Règles et détails du jeu PAC-MAN](https://strategywiki.org/wiki/Pac-Man)
- [Documentation SDL3](https://wiki.libsdl.org/SDL3/FrontPage)

### Références vidéo

Rechercher **"pacman namco 1980"** sur YouTube pour voir le gameplay original.

### Articles techniques

- Algorithmes de pathfinding pour les fantômes
- Gestion des collisions
- Synchronisation du framerate

---

## 📝 Critères d'évaluation

Le projet sera évalué selon les critères suivants :

1. **Contenu** : Complétude des fonctionnalités
2. **Structures de données** : Choix et implémentation
3. **Algorithmes** : Efficacité et pertinence
4. **Organisation du code** : Architecture et modularité
5. **Lisibilité** : Commentaires et conventions de nommage
6. **Élégance** : Qualité et propreté du code

---

## 🎤 Présentation

- **Format** : Démonstration interactive
- **Durée** : 10-15 minutes
- **Contenu attendu** :
  - Présentation du projet
  - Démonstration du jeu
  - Explication des choix techniques
  - Difficultés rencontrées et solutions

---

## 📅 Planning

- **Date limite de rendu** : Lundi 12h (veille de la dernière séance)
- **Présentation** : Dernière séance du semestre
- **Plateforme de soumission** : Moodle du CNAM

---

## 🎮 Règles du jeu PAC-MAN

### Objectif

Manger toutes les pastilles du labyrinthe tout en évitant les fantômes. Utiliser les power pellets pour renverser temporairement la situation.

### Commandes

- **Flèches directionnelles** : Déplacement de PAC-MAN
- **Échap** : Pause / Quitter

### Système de points

- Petite pastille : 10 points
- Power pellet : 50 points
- Fantôme (mode effrayé) : 200, 400, 800, 1600 points
- Fruits bonus : Variable

---

## 👨‍💻 Auteur

Projet réalisé dans le cadre du cours de **Structures de Données** - CNAM

---

## 📄 Licence

Projet éducatif - PAC-MAN est une marque déposée de Namco

---

## 🐛 Problèmes connus

_À compléter au fur et à mesure du développement_

---

## ✨ Améliorations futures

- [ ] Animation fluide des sprites
- [ ] Effets sonores authentiques
- [ ] Mode multijoueur
- [ ] High scores persistants
- [ ] Modes de difficulté

---

**Note** : Ce projet est réalisé à des fins éducatives uniquement.
