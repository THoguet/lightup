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

#les dépendances sont calculées automatiquement
target_link_libraries(game_text game)
target_link_libraries(game_test_thoguet game)
target_link_libraries(game_test_thperrier game)
target_link_libraries(game_test_nnunes game)
add_library (game game.o game_aux.c)

add_test(test_thoguet_dummy ./game_test_thoguet dummy)
add_test(test_thoguet_game_new ./game_test_thoguet game_new)
add_test(test_thoguet_game_print ./game_test_thoguet game_print)
add_test(test_thoguet_game_get_square ./game_test_thoguet game_get_square)
add_test(test_thoguet_game_default ./game_test_thoguet game_default)
add_test(test_thoguet_game_default_solution ./game_test_thoguet game_default_solution)
add_test(test_thoguet_game_restart ./game_test_thoguet game_restart)
add_test(test_thoguet_game_is_over ./game_test_thoguet game_is_over)
add_test(test_thoguet_game_update_flags ./game_test_thoguet game_update_flags)
add_test(test_game_play_move ./game_test_thoguet game_play_move)
add_test(test_game_check_move ./game_test_thoguet game_check_move)

add_test(test_thperrier_dummy ./game_test_thperrier dummy)
add_test(test_thperrier_game_copy ./game_test_thperrier game_copy)
add_test(test_thperrier_game_new_empty ./game_test_thperrier game_new_empty)
add_test(test_thperrier_game_equal ./game_test_thperrier game_equal)
add_test(test_thperrier_game_delete ./game_test_thperrier game_delete)
add_test(test_thperrier_game_is_blank ./game_test_thperrier game_is_blank)
add_test(test_thperrier_game_is_lightbulb ./game_test_thperrier game_is_lightbulb)
add_test(test_thperrier_game_is_black ./game_test_thperrier game_is_black)

add_test(test_nnunes_dummy ./game_test_thperrier dummy)
add_test(test_nnunes_game_get_square ./game_test_nnunes game_get_square)
add_test(test_nnunes_game_get_state ./game_test_nnunes game_get_state)
add_test(test_nnunes_game_get_flags ./game_test_nnunes game_get_flags)
add_test(test_nnunes_game_is_marked ./game_test_nnunes game_is_marked)
add_test(test_nnunes_game_is_lighted ./game_test_nnunes game_is_lighted)
add_test(test_nnunes_game_has_error ./gametest_nnunes game_has_error)
