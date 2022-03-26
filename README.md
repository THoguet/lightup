# README

[![Build](https://gitlab.emi.u-bordeaux.fr/pt2/teams-s4/tm07/lightup-07c/badges/main/pipeline.svg?job=build-job&key_text=Build)](https://gitlab.emi.u-bordeaux.fr/pt2/teams-s4/tm07/lightup-07c/-/commits/main)
[![Tests V1](https://gitlab.emi.u-bordeaux.fr/pt2/teams-s4/tm07/lightup-07c/badges/main/pipeline.svg?job=test-v1-job&key_text=Tests+V1)](https://gitlab.emi.u-bordeaux.fr/pt2/teams-s4/tm07/lightup-07c/-/commits/main)
[![Tests V2](https://gitlab.emi.u-bordeaux.fr/pt2/teams-s4/tm07/lightup-07c/badges/main/pipeline.svg?job=test-v2-job&key_text=Tests+V2)](https://gitlab.emi.u-bordeaux.fr/pt2/teams-s4/tm07/lightup-07c/-/commits/main)
[![Tests](https://gitlab.emi.u-bordeaux.fr/pt2/teams-s4/tm07/lightup-07c/badges/main/pipeline.svg?job=make-test-job&key_text=Tests)](https://gitlab.emi.u-bordeaux.fr/pt2/teams-s4/tm07/lightup-07c/-/commits/main)
[![Mem Check](https://gitlab.emi.u-bordeaux.fr/pt2/teams-s4/tm07/lightup-07c/badges/main/pipeline.svg?job=mem-check-job&key_text=MemCheck)](https://gitlab.emi.u-bordeaux.fr/pt2/teams-s4/tm07/lightup-07c/-/commits/main)
[![Coverage report](https://gitlab.emi.u-bordeaux.fr/pt2/teams-s4/tm07/lightup-07c/badges/main/coverage.svg)](https://gitlab.emi.u-bordeaux.fr/pt2/teams-s4/tm07/lightup-07c/-/commits/main)

## lightup-07c

Règles du jeu:

- Toutes les cases (qui ne sont pas des murs) doivent être illuminées.
- Une lumière ne doit pas être illuminé par une autre lumière.
- Les murs avec un chiffre doivent avoir exactement le chiffre indiqué de lumière(s) autour d'eux.
- Les murs sans chiffre peuvent avoir autant de lumière que désiré (0 compris).

FONCTIONNEMENT INTERFACE GRAPHIQUE:
Pour mettre une lumière il suffit de faire un clique gauche sur la case voulu, pour une marque c'est un clique droit. Pour enlever une lumière ou une marque d'une case il faut recliquer dessus avec le clique précédement utilisé. De plus nous avons mis des boutons en haut de l'interface sur lesquels vous pouvez cliquer pour exécuter les instructions UNDO, REDO, RESTART, SOLVE, SAVE et MUTE.
Mais vous pouvez également exécuter ces instructions avec le clavier en tapant sur les raccourcis suivants (clavier AZERTY):

- r ou F5 pour restart
- z pour undo
- y pour redo
- w pour save la partie dans son état actuel dans le fichier save.txt
- s pour solve la partie (si c'est possible)
- m pour désactiver / activer le son
- F11 pour activer / désactiver le Fullscreen
- ESC pour quitter le jeu
