# lightup-07c

[![Build](https://gitlab.emi.u-bordeaux.fr/pt2/teams-s4/tm07/lightup-07c/badges/main/pipeline.svg?job=build-job&key_text=Build)](https://gitlab.emi.u-bordeaux.fr/pt2/teams-s4/tm07/lightup-07c/-/commits/main)
[![Tests V1](https://gitlab.emi.u-bordeaux.fr/pt2/teams-s4/tm07/lightup-07c/badges/main/pipeline.svg?job=test-v1-job&key_text=Tests+V1)](https://gitlab.emi.u-bordeaux.fr/pt2/teams-s4/tm07/lightup-07c/-/commits/main)
[![Tests V2](https://gitlab.emi.u-bordeaux.fr/pt2/teams-s4/tm07/lightup-07c/badges/main/pipeline.svg?job=test-v2-job&key_text=Tests+V2)](https://gitlab.emi.u-bordeaux.fr/pt2/teams-s4/tm07/lightup-07c/-/commits/main)
[![Tests](https://gitlab.emi.u-bordeaux.fr/pt2/teams-s4/tm07/lightup-07c/badges/main/pipeline.svg?job=make-test-job&key_text=Tests)](https://gitlab.emi.u-bordeaux.fr/pt2/teams-s4/tm07/lightup-07c/-/commits/main)
[![Mem Check](https://gitlab.emi.u-bordeaux.fr/pt2/teams-s4/tm07/lightup-07c/badges/main/pipeline.svg?job=mem-check-job&key_text=MemCheck)](https://gitlab.emi.u-bordeaux.fr/pt2/teams-s4/tm07/lightup-07c/-/commits/main)
[![Coverage report](https://gitlab.emi.u-bordeaux.fr/pt2/teams-s4/tm07/lightup-07c/badges/main/coverage.svg)](https://gitlab.emi.u-bordeaux.fr/pt2/teams-s4/tm07/lightup-07c/-/commits/main)

## Dépendances

```bash
apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev # for Linux Debian/Ubuntu
brew install sdl2 sdl2_image sdl2_ttf sdl2_mixer                            # for MacOS
```

## Compilation

Avec CMake:

```bash
mkdir build ; cd build ; cmake .. ; make
```

## Règles du jeu

- Toutes les cases (qui ne sont pas des murs) doivent être illuminées.
- Une lumière ne doit pas être illuminé par une autre lumière.
- Les murs avec un chiffre doivent avoir exactement le chiffre indiqué de lumière(s) autour d'eux.
- Les murs sans chiffre peuvent avoir autant de lumière que désiré (0 compris).

## Fonctionnement du jeu dans un terminal

### Execution

```bash
./game_text [game_saved]
```

[game_saved] est optionnel et doit être une game sauvegardé dans le bon format; si non spécifié la game par default sera chargée.

### Raccourcis

- h => Affiche l'aide.
- r => Réinitialise la partie.
- q => Quitte la partie (abandon).
- z => Annule le dernier coup (undo).
- y => Annule l'annulation du dernier coup (redo).
- l \<i> \<j> => pose une lumière aux coordonnées i j (i vertical, j horizontal).
- m \<i> \<j> => pose une marque aux coordonnées i j (i vertical, j horizontal).
- b \<i> \<j> => enleve ce qu'il y a dans la case aux coordonnées i j (i vertical, j horizontal).
- w \<n> : Sauvegarde votre partie au nom \<n>.

## Fonctionnement interface graphique

### Execution

```bash
./game_sdl [game_saved]
```

[game_saved] est optionnel et doit être une game sauvegardé dans le bon format; si non spécifié la game par default sera chargée.

### Raccourcis

Pour mettre une lumière il suffit de faire un clique gauche sur la case voulu, pour une marque c'est un clique droit. Pour enlever une lumière ou une marque d'une case il faut recliquer dessus avec le clique précédement utilisé. De plus nous avons mis des boutons en haut de l'interface sur lesquels vous pouvez cliquer pour exécuter les instructions UNDO, REDO, RESTART, SOLVE, SAVE et MUTE.
Mais vous pouvez également exécuter ces instructions avec le clavier en tapant sur les raccourcis suivants:

- r / F5 => Réinitialise la partie.
- z => Annule le dernier coup (undo).
- y => Annule l'annulation du dernier coup (redo).
- w => Sauvegarde votre partie dans le fichier save.txt.
- s => Résous la partie (si c'est possible).
- m => Désactive / active le son.
- F11 => Active / désactive le Fullscreen.
- ESC => Quitte la partie (abandon).
