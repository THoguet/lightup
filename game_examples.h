/**
 * @file game_examples.h
 * @brief All Test Functions.
 **/

#ifndef __GAME_EXAMPLES_H__
#define __GAME_EXAMPLES_H__

#include "game.h"
#include "game_ext.h"

/* ************************************************************************** */
/*                               GAME EXAMPLES                                */
/* ************************************************************************** */

// V1
extern square default_squares[];
extern square solution_squares[];
extern square other_squares[];

// V2
extern square ext_4x4_squares[];
extern square sol_4x4_squares[];
extern square ext_3x10_squares[];
extern square sol_3x10_squares[];
extern square ext_5x1_squares[];
extern square ext_5x3w_squares[];
extern square sol_5x3w_squares[];
extern square ext_3x3w_squares[];
extern square sol_3x3w_squares[];
extern square ext_2x2w_squares[];
extern square sol_2x2w_squares[];

// V3
extern char default_str[];
extern char ext_5x3w_str[];
extern char other_str[];
extern square ext_hard_10x10[];
extern square ext_1sol_1x1[];
extern square ext_2sol_2x1[];
extern square ext_2sol_2x2[];
extern square ext_4sol_3x3[];

#endif  // __GAME_EXAMPLES_H__
