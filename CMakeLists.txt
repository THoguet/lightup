#version minimale de cmake nécessaire
cmake_minimum_required(VERSION 2.6...3.13)

enable_testing()

#nom du projet
project (game_text)

#positionne la variable
set(CMAKE_C_FLAGS "-std=c99 -g -Wall")

#définit le nom du programme ainsi que
#ses sources
add_executable(game_text game_text.c)
add_executable(game_test_thoguet game_test_thoguet.c)
add_executable(game_test_thperrier game_test_thperrier.c)

#les dépendances sont calculées automatiquement
target_link_libraries(game_text game)
target_link_libraries(game_test_thoguet game)
add_library (game game.o game_aux.c)
add_test(test_thoguet_dummy ./game_test_thoguet dummy)
add_test(test_thoguet_new ./game_test_thoguet game_new)
add_test(test_thoguet_game_print ./game_test_thoguet game_print)
add_test(test_thoguet_get_square ./game_test_thoguet game_get_square)
add_test(test_thoguet_default ./game_test_thoguet game_default)
add_test(test_thoguet_game_default_solution ./game_test_thoguet game_default_solution)

add_test(test_thperrier_dummy ./game_test_thperrier dummy)
add_test(test_thperrier_game_new_empty ./game_test_thperrier game_new_empty)