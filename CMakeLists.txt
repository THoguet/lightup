#version minimale de cmake nécessaire
cmake_minimum_required(VERSION 2.6)

enable_testing()

#nom du projet
project (game_text)

#positionne la variable
set(CMAKE_C_FLAGS "-std=c99 -g -Wall")

#définit le nom du programme ainsi que
#ses sources
add_executable(game_text game_text.c)
add_executable(game_test_thoguet game_test_thoguet.c)

#les dépendances sont calculées automatiquement
target_link_libraries(game_text game)
add_library (game game.o game_aux.c)
add_test(game_test_dummy ./game_test_thoguet dummy)