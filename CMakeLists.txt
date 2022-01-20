#version minimale de cmake nécessaire
cmake_minimum_required(VERSION 2.6...3.13)

enable_testing()

#nom du projet
project (game_text)
include(CTest)

#positionne la variable
set(CMAKE_C_FLAGS "-std=c99 -g -Wall")

#définit le nom du programme ainsi que
#ses sources
add_executable(game_text game_text.c)
add_executable(game_test_thoguet game_test_thoguet.c)
add_executable(game_test_thperrier game_test_thperrier.c)
add_executable(game_test_nnunes game_test_nnunes.c)
#add_executable(history_test history_test.c)

#les dépendances sont calculées automatiquement
target_link_libraries(game_text game)
target_link_libraries(game_test_thoguet game)
target_link_libraries(game_test_thperrier game)
target_link_libraries(game_test_nnunes game)
#target_link_libraries(history_test game)

#Libraries
add_library (game game.c game_aux.c game_ext.c game_private.c history.c)

#Tests THOGUET
add_test(test_thoguet_dummy ./game_test_thoguet dummy)
add_test(test_thoguet_game_new ./game_test_thoguet game_new)
add_test(test_thoguet_game_print ./game_test_thoguet game_print)
add_test(test_thoguet_game_get_square ./game_test_thoguet game_get_square)
add_test(test_thoguet_game_default ./game_test_thoguet game_default)
add_test(test_thoguet_game_default_solution ./game_test_thoguet game_default_solution)
add_test(test_thoguet_game_restart ./game_test_thoguet game_restart)
add_test(test_thoguet_game_is_over ./game_test_thoguet game_is_over)
add_test(test_thoguet_game_update_flags ./game_test_thoguet game_update_flags)
add_test(test_thoguet_game_play_move ./game_test_thoguet game_play_move)
add_test(test_thoguet_game_check_move ./game_test_thoguet game_check_move)
add_test(test_thoguet_game_new_empty_ext ./game_test_thoguet game_new_empty_ext)
add_test(test_thoguet_game_new_ext ./game_test_thoguet game_new_ext)

#Tests THPERRIER
add_test(test_thperrier_dummy ./game_test_thperrier dummy)
add_test(test_thperrier_game_copy ./game_test_thperrier game_copy)
add_test(test_thperrier_game_new_empty ./game_test_thperrier game_new_empty)
add_test(test_thperrier_game_equal ./game_test_thperrier game_equal)
add_test(test_thperrier_game_delete ./game_test_thperrier game_delete)
add_test(test_thperrier_game_is_blank ./game_test_thperrier game_is_blank)
add_test(test_thperrier_game_is_lightbulb ./game_test_thperrier game_is_lightbulb)
add_test(test_thperrier_game_is_black ./game_test_thperrier game_is_black)
add_test(test_thperrier_game_get_black_number ./game_test_thperrier game_get_black_number)
add_test(test_thperrier_game_undo ./game_test_thperrier game_undo)
add_test(test_thperrier_game_redo ./game_test_thperrier game_redo)

#Tests NNUNES
add_test(test_nnunes_dummy ./game_test_nnunes dummy)
add_test(test_nnunes_game_set_square ./game_test_nnunes game_set_square)
add_test(test_nnunes_game_get_state ./game_test_nnunes game_get_state)
add_test(test_nnunes_game_get_flags ./game_test_nnunes game_get_flags)
add_test(test_nnunes_game_is_marked ./game_test_nnunes game_is_marked)
add_test(test_nnunes_game_is_lighted ./game_test_nnunes game_is_lighted)
add_test(test_nnunes_game_has_error ./game_test_nnunes game_has_error)
add_test(test_nnunes_game_nb_rows ./game_test_nnunes game_nb_rows)
add_test(test_nnunes_game_nb_cols ./game_test_nnunes game_nb_cols)
add_test(test_nnunes_game_is_wrapping ./game_test_nnunes game_is_wrapping)

#Tests HISTORY
# add_test(test_history_history_alloc ./history_test history_alloc)
# add_test(test_history_create_empty ./history_test history_create_empty)
# add_test(test_history_delete_entire_history ./history_test history_delete_entire_history)
# add_test(test_history_is_empty ./history_test history_is_empty)
# add_test(test_history_next ./history_test history_next)
# add_test(test_history_prev ./history_test history_prev)
# add_test(test_history_insert_first ./history_test history_insert_first)
# add_test(test_history_delete_first ./history_test history_delete_first)
# add_test(test_history_insert_after ./history_test history_insert_after)
# add_test(test_history_delete_after ./history_test history_delete_after)
# add_test(test_history_insert_before ./history_test history_insert_before)
# add_test(test_history_delete_before ./history_test history_delete_before)
# add_test(test_history_last ./history_test history_last)
# add_test(test_history_first ./history_test history_first)
# add_test(test_history_append ./history_test history_append)
# add_test(test_history_copy ./history_test history_copy)