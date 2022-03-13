#version minimale de cmake nécessaire
cmake_minimum_required(VERSION 2.6...3.13)

#nom du projet
project ("Lightup" C)

# enable testing
enable_testing()
set(MEMORYCHECK_COMMAND_OPTIONS "--error-exitcode=1 --leak-check=full")
include(CTest)

## use pkgconfig to solve SDL2 dependencies (Linux only)
include(FindPkgConfig) # pkg-config --list-all  | grep -i sdl2
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2_IMG REQUIRED SDL2_image)
pkg_search_module(SDL2_TTF REQUIRED SDL2_ttf)
set(SDL2_ALL_LIBS ${SDL2_LIBRARIES} ${SDL2_IMG_LIBRARIES} ${SDL2_TTF_LIBRARIES})
set(SDL2_ALL_INC ${SDL2_INCLUDE_DIRS} ${SDL2_IMG_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

## use a CMake3 package to solve SDL2 dependencies
# find_package(SDL2 REQUIRED)
# set(SDL2_ALL_INC ${SDL2_INCLUDE_DIRS})
# set(SDL2_ALL_LIBS ${SDL2_LIBRARIES} -lSDL2_ttf -lSDL2_image) # add extra SDL2 libraries


message(STATUS "SDL2 include dir: ${SDL2_ALL_INC}")
message(STATUS "SDL2 all libraries: ${SDL2_ALL_LIBS}")

#positionne la variable
set(CMAKE_C_FLAGS "-std=c99 -Wall")
set(CMAKE_C_FLAGS_DEBUG "-g --coverage  -pthread -Wextra -Wunused-parameter -Wpedantic -Werror")    # use CMake option: -DCMAKE_BUILD_TYPE=DEBUG
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")   # use CMake option: -DCMAKE_BUILD_TYPE=RELEASE

############################# SRC #############################

# game_text
add_executable(game_text game_text.c)
target_link_libraries(game_text game)

# game_tests
add_executable(game_test game_test.c)
target_link_libraries(game_test game)

# game_solve
add_executable(game_solve game_solve.c)
target_link_libraries(game_solve game)

# game_sdl
include_directories(${SDL2_ALL_INC})
add_executable(game_sdl main.c model.c)
target_link_libraries(game_sdl game ${SDL2_ALL_LIBS} m)

# libgame
add_library (game game.c game_aux.c game_ext.c game_private.c history.c game_tools.c game_examples.c)

# history tests
#add_executable(history_test history_test.c)
#target_link_libraries(history_test game)


############################# TEST V1 #############################

add_test(test_thoguet_game_new_v1 ./game_test game_new)
add_test(test_thoguet_game_default_v1 ./game_test game_default)
add_test(test_thoguet_game_default_solution_v1 ./game_test game_default_solution)
add_test(test_nnunes_game_set_square_v1 ./game_test game_set_square)
add_test(test_nnunes_game_get_state_v1 ./game_test game_get_state)
add_test(test_nnunes_game_get_flags_v1 ./game_test game_get_flags)
add_test(test_nnunes_game_is_marked_v1 ./game_test game_is_marked)
add_test(test_nnunes_game_is_lighted_v1 ./game_test game_is_lighted)
add_test(test_thperrier_game_is_black_v1 ./game_test game_is_black)
add_test(test_thoguet_game_is_over_v1 ./game_test game_is_over)
add_test(test_thperrier_game_get_black_number_v1 ./game_test game_get_black_number)
add_test(test_thperrier_game_is_lightbulb_v1 ./game_test game_is_lightbulb)
add_test(test_thperrier_game_is_blank_v1 ./game_test game_is_blank)
add_test(test_thperrier_game_new_empty_v1 ./game_test game_new_empty)
add_test(test_game_equal_v1 ./game_test equal)


############################# TEST V2 #############################

add_test(test_thoguet_game_get_square_v2 ./game_test game_get_square)
add_test(test_thoguet_game_restart_v2 ./game_test game_restart)
add_test(test_thoguet_game_update_flags_v2 ./game_test game_update_flags)
add_test(test_thoguet_game_play_move_v2 ./game_test game_play_move)
add_test(test_thoguet_game_check_move_v2 ./game_test game_check_move)
add_test(test_thoguet_game_new_empty_ext_v2 ./game_test game_new_empty_ext)
add_test(test_thoguet_game_new_ext_v2 ./game_test game_new_ext)
add_test(test_thperrier_game_copy_v2 ./game_test game_copy)
add_test(test_game_equal_v2 ./game_test equal_ext)
add_test(test_thperrier_game_delete_v2 ./game_test game_delete)
add_test(test_thperrier_game_undo_v2 ./game_test game_undo)
add_test(test_thperrier_game_redo_v2 ./game_test game_redo)
add_test(test_nnunes_game_has_error_v2 ./game_test game_has_error)
add_test(test_nnunes_game_nb_rows_v2 ./game_test game_nb_rows)
add_test(test_nnunes_game_nb_cols_v2 ./game_test game_nb_cols)
add_test(test_nnunes_game_is_wrapping_v2 ./game_test game_is_wrapping)
add_test(test_thoguet_game_print_v2 ./game_test game_print)


############################# TEST V3 #############################

add_test(test_thoguet_game_save_v3 ./game_test game_save)
add_test(test_mfatih_game_load_v3 ./game_test game_load)
add_test(test_thoguet_game_solve_v3 ./game_test game_solve)
add_test(test_vcarmona_game_nb_solutions_v3 ./game_test game_nb_solutions)


############################# TEST HISTORY #############################

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
