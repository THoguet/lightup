# lightup-a42e

### game game_new(square *squares) -> thoguet -> fait

### game game_new_empty(void) ->

### game game_copy(cgame g) ->

### bool game_equal(cgame g1, cgame g2) ->

### void game_delete(game g) ->

### void game_set_square(game g, uint i, uint j, square s) ->

### square game_get_square(cgame g, uint i, uint j) -> thoguet -> fini

### square game_get_state(cgame g, uint i, uint j) ->

### square game_get_flags(cgame g, uint i, uint j) ->

### bool game_is_blank(cgame g, uint i, uint j) ->

### bool game_is_lightbulb(cgame g, uint i, uint j) ->

### bool game_is_black(cgame g, uint i, uint j) ->

### int game_get_black_number(cgame g, uint i, uint j) ->

### bool game_is_marked(cgame g, uint i, uint j) ->

### bool game_is_lighted(cgame g, uint i, uint j) ->

### bool game_has_error(cgame g, uint i, uint j) ->

### bool game_check_move(cgame g, uint i, uint j, square s) ->

### void game_play_move(game g, uint i, uint j, square s) ->

### void game_update_flags(game g) ->

### bool game_is_over(cgame g) ->

### void game_restart(game g) ->

### void game_print(cgame g) -> thoguet -> fini

### game game_default(void) -> thoguet -> fini

### game game_default_solution(void) -> thoguet -> fini
