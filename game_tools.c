#include "game_tools.h"
#include <stdlib.h>
#include "game_aux.h"
#include "game_ext.h"
#include "game_private.h"

#define NB_CHAR_HEADER_WITHTOUT_DIMENSIONS 4

#define JWRAPPING (j + game_nb_cols(g) + gap_position * dir[index_dir + 1]) % game_nb_cols(g)
#define IWRAPPING (i + game_nb_rows(g) + gap_position * dir[index_dir]) % game_nb_rows(g)
#define JNORMAL j + gap_position* dir[index_dir + 1]
#define INORMAL i + gap_position* dir[index_dir]

game game_load(char* filename) {
	checkPointer((void*)filename, "NULL pointer filename on game_load\n");
	// ouvrons le fichier
	FILE* f = fopen(filename, "r");
	checkPointer((void*)f, "Couldn't open file\n");
	// récuperer les valeurs dont on a beson pour créer le jeu initail
	uint nb_rows, nb_cols;
	int wrapping;
	test_output(fscanf(f, "%u %u %d\n", &nb_rows, &nb_cols, &wrapping), 3, "Error in scan of file");
	// créer le jeu initail
	game g1 = game_new_empty_ext(nb_rows, nb_cols, wrapping);
	checkPointer((void*)g1, "Couldn't create game\n");
	// récuperer les coups à jouer
	for (uint i = 0; i < nb_rows; i++) {
		for (uint j = 0; j < nb_cols; j++) {
			test_output(feof(f), 0, "Malformed file : Reached end of file\n");
			// obtenir les squares pour les jouer dans g1
			char play = fgetc(f);
			if (play == '\n')
				play = fgetc(f);
			if (play == 'b')
				game_set_square(g1, i, j, S_BLANK);
			else if (play == '*')
				game_set_square(g1, i, j, S_LIGHTBULB);
			else if (play == '-')
				game_set_square(g1, i, j, S_MARK);
			else if (play == 'w')
				game_set_square(g1, i, j, S_BLACKU);
			else if (play == '0')
				game_set_square(g1, i, j, S_BLACK0);
			else if (play == '1')
				game_set_square(g1, i, j, S_BLACK1);
			else if (play == '2')
				game_set_square(g1, i, j, S_BLACK2);
			else if (play == '3')
				game_set_square(g1, i, j, S_BLACK3);
			else if (play == '4')
				game_set_square(g1, i, j, S_BLACK4);
			else {
				fprintf(stderr, "Malformed file : Unrecognized character %c.\n", play);
				exit(EXIT_FAILURE);
			}
		}
	}
	fclose(f);
	game_update_flags(g1);
	return g1;
}

/**
 * @brief Get the char from state object
 *
 * @param g game to get the square from
 * @param i the i coordinate
 * @param j the j coordinate
 * @return char of the corresponding square
 */
char get_char_from_state(cgame g, uint i, uint j) {
	// char tab[index] => index in enum square
	char tab[] = {'b', '*', '-', '?', '?', '?', '?', '?', '0', '1', '2', '3', '4', 'w'};
	return tab[(int)game_get_state(g, i, j)];
}

void game_save(cgame g, char* filename) {
	checkPointer((void*)filename, "NULL pointer filename on game_save\n");
	checkPointer((void*)g, "NULL pointer game g on game_save\n");
	// try create or open file named filename
	FILE* f = fopen(filename, "w");
	checkPointer((void*)f, "Couldn't open file in game_save\n");
	// try to print the first line (game_nb_rows(g) / 10 + 1 = nb char of a number)
	test_output(fprintf(f, "%d %d %d\n", game_nb_rows(g), game_nb_cols(g), game_is_wrapping(g)),
	            NB_CHAR_HEADER_WITHTOUT_DIMENSIONS + game_nb_rows(g) / 10 + 1 + game_nb_cols(g) / 10 + 1, "Couldn't print in file.\n");
	// for each case of the game get the char of the given case and try to print it on the file
	for (uint i = 0; i < game_nb_rows(g); i++) {
		for (uint j = 0; j < game_nb_cols(g); j++) {
			test_output(fprintf(f, "%c", get_char_from_state(g, i, j)), 1, "Couldn't print in file.\n");
		}
		// print a \n at the end of each row
		test_output(fprintf(f, "\n"), 1, "Couldn't print in file.\n");
	}
	fclose(f);
}

/**
 * @brief Test if there is any error on the entire game
 *
 * @param g the game to analyze
 * @return true if there is 1 or more error on the game g
 * @return false if there is no one error on the game g
 */
bool game_has_error_general(cgame g) {
	for (uint i = 0; i < game_nb_rows(g); i++) {
		for (uint j = 0; j < game_nb_cols(g); j++) {
			if (game_has_error(g, i, j))
				return true;
		}
	}
	return false;
}

/**
 * @brief Change the array diagToTest who must containt coordinate i,j of a case else -1 if the cases are not on the same row / column take the cross point of
 * the two points
 * // clang off
 *   0 1 2
 * 0 a - b
 * 1 x 2 y
 * 2 c z d
 * // clang on
 * in this case the diagToTest must containt (zi,zj,xi,xj,yi,yj,?,?) (order doesn't matter) coordinates [2,1,0,1,1,0,-1,-1] and the function change this
 * coordinate of the points in the array to the cross points of these points in this case the array should look like this : [2,0,2,2,?,?,?,?]
 * ([ci,cj,di,dj,?,?,?,?]) and return the number of changed elements on diagToTest
 * @param diagToTest the tab to test and change
 * @param size the size of diagToTest
 * @return uint the number of element on the changed diagToTest
 */
uint coordinateCrossPointMaker(int* diagToTest, uint size) {
	int tab[size];
	uint cpt = 0;
	for (uint i = 0; i < size; i += 2) {
		for (uint j = i + 2; j < size; j += 2) {
			if (diagToTest[i] != -1 && diagToTest[j] != -1 && diagToTest[i] != diagToTest[j] && diagToTest[i + 1] != -1 && diagToTest[j + 1] != -1 &&
			    diagToTest[i + 1] != diagToTest[j + 1]) {
				tab[cpt++] = diagToTest[i];
				tab[cpt++] = diagToTest[j + 1];
			}
		}
	}
	for (uint i = 0; i < cpt; i++) {
		diagToTest[i] = tab[i];
	}
	return cpt;
}

/**
 * @brief try to add a s square on blank cases in the directions of the array dir (who must have be fill with coef (-1 ; 0 ; 1) of directions like this :
 * [i,j,i,j,i,j,i,j]) to the i,j case in a 1 to max_gap distance from the i,j case if force is set to true place the square on lighted blank too (if there is an
 * error undo all moves and return 0)
 *
 * @param g the game to change
 * @param i the i coordinate of the case to add around
 * @param j the j coordinate of the case to add around
 * @param s the square to add around the i j case
 * @param max_gap the max distance to add the s square around the i j case
 * @param dir the array containing coef of directions
 * @param force	true => place the s square on lighted blank too else only on blank
 * @return uint - the number of placed square (if there is an error undo all moves and return 0)
 */
uint addAround(game g, uint i, uint j, square s, uint max_gap, int* dir, bool force) {
	uint move_played = 0;
	for (uint gap_position = 1; gap_position <= max_gap; gap_position++) {
		for (uint index_dir = 0; index_dir < 8; index_dir += 2) {
			if (/* test if both dir are not 0*/ (dir[index_dir] != 0 || 0 != dir[index_dir + 1]) &&
			    /* normal check*/ ((JNORMAL < game_nb_cols(g) && INORMAL < game_nb_rows(g)) ||
			                       /*wrapping check*/ (game_is_wrapping(g) &&
			                                           /*not the same case*/ !(IWRAPPING == i && JWRAPPING == j)))) {
				if (game_is_black(g, IWRAPPING, JWRAPPING)) {
					dir[index_dir] = 0;
					dir[index_dir + 1] = 0;
				} else {
					square state = game_get_state(g, IWRAPPING, JWRAPPING);
					square sqr = game_get_square(g, IWRAPPING, JWRAPPING);
					if ((force && state == S_BLANK) || sqr == S_BLANK) {
						game_play_move(g, IWRAPPING, JWRAPPING, s);
						move_played++;
						if (game_has_error_general(g)) {
							for (; move_played != 0; move_played--) {
								game_undo(g);
							}
							return move_played;
						}
					}
				}
			}
		}
	}
	return move_played;
}

/**
 * @brief look the cases around the i j case of the game g from 1 case distance to max_gap distance and fill not_empty, lightbulb, mark, blank_not_lighted
 * variables with the number of occurence found. dirDiagoToEdit is an optional param who contain the coordinates of the blank squares
 *
 * @param g the game
 * @param i the i coordinate of the case to check around
 * @param j the j coordinate of the case to check around
 * @param max_gap the max distance to check around the i j coordinate
 * @param not_empty the variable to edit with the number of not empty case around i j
 * @param lightbulb the variable to edit with the number of lightbulb around i j
 * @param mark the variable to edit with the number of mark around i j
 * @param blank_not_lighted the variable to edit with the number of blank_not_lighted around i j
 * @param dirDiagoToEdit (optional) must be allocated with the maximum possible coordinate (4*2*gap_distance) will be edit with the coordinate of all blank
 * squares around i j
 */
void checkAround(cgame g, uint i, uint j, uint max_gap, int* not_empty, int* lightbulb, int* mark, int* blank_not_lighted, int* dirDiagoToEdit) {
	int dir[] = {1, 0, -1, 0, 0, 1, 0, -1};
	for (uint gap_position = 1; gap_position <= max_gap; gap_position++) {
		for (uint index_dir = 0; index_dir < sizeof(dir) / sizeof(dir[0]); index_dir += 2) {
			if (/* test if both dir are not 0*/ dir[index_dir] != dir[index_dir + 1] &&
			    /* normal check*/ ((JNORMAL < game_nb_cols(g) && INORMAL < game_nb_rows(g)) ||
			                       /*wrapping check*/ (game_is_wrapping(g) &&
			                                           /*not the same case*/ !(IWRAPPING == i && JWRAPPING == j)))) {
				if (game_is_black(g, IWRAPPING, JWRAPPING)) {
					dir[index_dir] = 0;
					dir[index_dir + 1] = 0;
					(*not_empty)++;
				} else if (game_is_lighted(g, IWRAPPING, JWRAPPING) || !game_is_blank(g, IWRAPPING, JWRAPPING)) {
					(*not_empty)++;
					if (game_is_marked(g, IWRAPPING, JWRAPPING))
						(*mark)++;
					if (game_is_lightbulb(g, IWRAPPING, JWRAPPING))
						(*lightbulb)++;
				} else {
					if (dirDiagoToEdit != NULL) {
						dirDiagoToEdit[index_dir] = IWRAPPING;
						dirDiagoToEdit[index_dir + 1] = JWRAPPING;
					}
					(*blank_not_lighted)++;
				}
			} else {
				(*not_empty)++;
			}
		}
	}
}
/**
 * @brief analyze the game and play moves that are logical / mandatory. It stops only when it don't play any move and return the number of move played / special
 * number
 *
 * @param g the game to analyze
 * @return -1 if game_is_over ; -2 if the game has no sol ; else the number of move played
 */
int game_analyze(game g) {
	uint total_move_played = 0, move_played;
	do {
		move_played = 0;
		for (uint i = 0; i < game_nb_rows(g); i++) {
			for (uint j = 0; j < game_nb_cols(g); j++) {
				int dirLine[] = {1, 0, -1, 0, 0, 1, 0, -1};
				int dirDiago[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
				uint tmp = 0;
				/**
				 * @brief if the case is a blank (not lighted) test if there is only one blank square (itself) in all the directions if yes set a lightbulb (if
				 * the lightbulb create an error the game is unsolvable => undo all moves and return -2)
				 *
				 */
				if (game_get_square(g, i, j) == S_BLANK) {
					int lb = 0, not_empty = 0, mark = 0, blank = 0;
					checkAround(g, i, j, max(game_nb_cols(g), game_nb_rows(g)), &not_empty, &lb, &mark, &blank, NULL);
					if (blank == 0) {
						game_play_move(g, i, j, S_LIGHTBULB);
						move_played++;
						if (game_is_over(g))
							return -1;
						if (game_has_error_general(g)) {
							total_move_played += move_played;
							for (; total_move_played != 0; total_move_played--) {
								game_undo(g);
							}
							return -2;
						}
					}
					/**
					 * @brief if the square is a wall between 1 and 4 test if there is the number of lightbulb already placed => place marks on all the blanks
					 * (if there is an error the game is unsolvable) square around the wall, else test if there is the number of blank (not lighted) needed by
					 * the wall around it => set lightbulb around the wall (if there is an error the game is unsolvable)
					 *
					 */
				} else if (game_get_state(g, i, j) >= S_BLACK1 && game_get_state(g, i, j) < S_BLACKU) {
					int lb = 0, not_empty = 0, mark = 0, blank = 0;
					checkAround(g, i, j, 1, &not_empty, &lb, &mark, &blank, dirDiago);
					if (lb == game_get_black_number(g, i, j)) {
						move_played += addAround(g, i, j, S_MARK, 1, dirLine, true);
					} else if (not_empty - lb == abs(game_get_black_number(g, i, j) - 4)) {
						tmp = addAround(g, i, j, S_LIGHTBULB, 1, dirLine, false);
						if (tmp == 0) {
							total_move_played += move_played;
							for (; total_move_played != 0; total_move_played--) {
								game_undo(g);
							}
							return -2;
						}
						move_played += tmp;
						if (game_is_over(g))
							return -1;
					}
					/**
					 * @brief if this statement is true that means a single lightbulb (set on diagonals of the wall) can obfuscate two blanks around the wall
					 * and produce an error so we can place mark on these cases
					 */
					else if (blank == game_get_black_number(g, i, j) - lb + 1) {
						uint cpt = coordinateCrossPointMaker(dirDiago, 8);
						for (uint cpt_index = 0; cpt_index < cpt; cpt_index += 2) {
							if (game_get_square(g, dirDiago[cpt_index], dirDiago[cpt_index + 1]) == S_BLANK) {
								game_play_move(g, dirDiago[cpt_index], dirDiago[cpt_index + 1], S_MARK);
								move_played++;
							}
						}
					}
				}  // we can set marks all around a wall0 because it must not have lightbulb right next to it
				else if (game_get_state(g, i, j) == S_BLACK0) {
					move_played += addAround(g, i, j, S_MARK, 1, dirLine, true);
					if (game_is_over(g))
						return -1;
				} /**
				   * @brief if the mark is not lighted and there is only one blank in all the directions we can place a lightbulb (if there is an error the game
				   * is unsolvable)
				   *
				   */
				else if (game_get_state(g, i, j) == S_MARK && !game_is_lighted(g, i, j)) {
					int lb = 0, not_empty = 0, mark = 0, blank = 0;
					checkAround(g, i, j, max(game_nb_cols(g), game_nb_rows(g)), &not_empty, &lb, &mark, &blank, NULL);
					if (blank == 1)
						tmp = addAround(g, i, j, S_LIGHTBULB, max(game_nb_cols(g), game_nb_rows(g)), dirLine, false);
					if (blank == 0 || (blank == 1 && tmp == 0)) {
						total_move_played += move_played;
						for (; total_move_played != 0; total_move_played--) {
							game_undo(g);
						}
						return -2;
					}
					move_played += tmp;
					if (game_is_over(g))
						return -1;
				}
			}
		}
		total_move_played += move_played;
	} while (move_played != 0);
	return total_move_played;
}

/**
 * @brief the brutforce fonction set a lightbulb on each case (recursive call while deep > 1) if the lightbulb produce an error undo it and go to the next
 * coordinate on each new lightbulb who isn't producing error call game_analyze to see if it can help / say if the game is unsolvable (and set a mark on this
 * case)
 *
 * @param g the game to brutforce
 * @param deep the deepness of the brutforce
 * @param move_played the number of moves played (to undo in case of unsolvable game)
 * @return true if the game is solved
 * @return false if the game is unsolvable
 */
bool aux_game_solve(game g, uint deep, int* move_played, bool nb_sol, uint* cpt) {
	for (uint i = 0; i < game_nb_rows(g); i++) {
		for (uint j = 0; j < game_nb_cols(g); j++) {
			if (game_get_state(g, i, j) == S_BLANK) {
				int tmp = 0;
				game_play_move(g, i, j, S_LIGHTBULB);
				if (!game_has_error_general(g)) {
					if (game_is_over(g)) {
						if (!nb_sol)
							return true;
						else
							(*cpt)++;
					}
					tmp = game_analyze(g);
					if (tmp == -1) {
						if (!nb_sol)
							return true;
						else
							(*cpt)++;
					}
					if (tmp == -2) {
						game_undo(g);
						game_play_move(g, i, j, S_MARK);
						(*move_played)++;
					} else
						(*move_played) += tmp;
					if (deep > 1) {
						if (aux_game_solve(g, deep - 1, move_played, nb_sol, cpt)) {
							return true;
						}
					}
				}
				if (tmp != -2) {
					for (; (*move_played) > 0; (*move_played)--) {
						game_undo(g);
					}
					game_undo(g);
				}
			}
		}
	}
	return false;
}

uint total_number_of_blank_cases(game g) {
	uint cpt = 0;
	for (uint i = 0; i < game_nb_rows(g); i++) {
		for (uint j = 0; j < game_nb_cols(g); j++) {
			if (game_get_state(g, i, j) == S_BLANK)
				cpt++;
		}
	}
	return cpt;
}

bool game_solve(game g) {
	// first analyze of the game to reduce the number of brutforce needed
	int total_move_played = game_analyze(g);
	// give move_played and not total_move_played to keep the first analyze
	int move_played = 0;
	if (total_move_played == -1)
		return true;
	// if the game is unsolvalbe at the first analyze it's unsolvable even with brutforce
	if (total_move_played == -2)
		return false;
	// else try brutforce with deepness going from 1 to the total number of blank cases on the game
	for (uint deep = 1; deep < total_number_of_blank_cases(g); deep++) {
		total_move_played += move_played;
		if (aux_game_solve(g, deep, &move_played, false, NULL)) {
			return true;
		}
	}
	for (int i = 0; i < total_move_played; i++) {
		game_undo(g);
	}
	return false;
}

uint game_nb_solutions(cgame g) {
	game copy_g = game_copy(g);
	// first analyze of the game to reduce the number of brutforce needed
	int total_move_played = game_analyze(copy_g);
	// give move_played and not total_move_played to keep the first analyze
	int move_played = 0;
	uint nb_solutions = 0;
	if (total_move_played == -1)
		return 1;
	// if the game is unsolvalbe at the first analyze it's unsolvable even with brutforce
	if (total_move_played == -2)
		return 0;
	// else try brutforce with deepness going from 1 to the total number of blank cases on the game
	for (uint deep = 1; deep < total_number_of_blank_cases(copy_g); deep++) {
		aux_game_solve(copy_g, deep, &move_played, true, &nb_solutions);
	}
	return nb_solutions;
}