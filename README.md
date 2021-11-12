# lightup-a42e

Projet Techno
## Tests

| Test                                                | REF                                                                          | Assigned to | Notes   |        Finished        |
| --------------------------------------------------- | ---------------------------------------------------------------------------- | ----------- | ------- | :--------------------: |
| [game_new](./game_test_thoguet.c#L49)               | [game game_new(square *squares)](./game.h#L73-81)                            | @thoguet    |         | <ul><li>[x] </li></ul> |
| [game_new_empty](./game_test_thperrier.c#L16)       | [game game_new_empty(void)](./game.h#L83-88)                                 | @thperrier  |         | <ul><li>[X] </li></ul> |
| [game_copy](./game_test_thperrier.c#L32)            | [game game_copy(cgame g)](./game.h#L90-96)                                   | @thperrier  |         | <ul><li>[X] </li></ul> |
| [game_equal](./game_test_thperrier.c#L?)            | [bool game_equal(cgame g1, cgame g2)](./game.h#L98-106)                      | @thperrier  |         | <ul><li>[ ] </li></ul> |
| [game_delete](./game_test_thperrier.c#L?)           | [void game_delete(game g);](./game.h#L108-113)                               | @thperrier  |         | <ul><li>[ ] </li></ul> |
| [game_set_square](./game_test_nnunes.c#L?)          | [void game_set_square(game g, uint i, uint j, square s)](./game.h#L115-128)  | @nnunes     |         | <ul><li>[ ] </li></ul> |
| [game_get_square](./game_test_thoguet.c#L89)        | [square game_get_square(cgame g, uint i, uint j)](./game.h#L130-140)         | @thoguet    |         | <ul><li>[x] </li></ul> |
| [game_get_state](./game_test_nnunes.c#L?)           | [square game_get_state(cgame g, uint i, uint j)](./game.h#L142-153)          | @nnunes     |         | <ul><li>[ ] </li></ul> |
| [game_get_flags](./game_test_nnunes.c#L?)           | [square game_get_flags(cgame g, uint i, uint j)](./game.h#L155-166)          | @nnunes     |         | <ul><li>[ ] </li></ul> |
| [game_is_blank](./game_test_thperrier.c#L?)         | [bool game_is_blank(cgame g, uint i, uint j)](./game.h#L168-178)             | @thperrier  |         | <ul><li>[ ] </li></ul> |
| [game_is_lightbulb](./game_test_thperrier.c#L?)     | [bool game_is_lightbulb(cgame g, uint i, uint j)](./game.h#L180-190)         | @thperrier  |         | <ul><li>[ ] </li></ul> |
| [game_is_black](./game_test_thperrier.c#L?)         | [bool game_is_black(cgame g, uint i, uint j)](./game.h#L192-202)             | @thperrier  |         | <ul><li>[ ] </li></ul> |
| [game_get_black_number](./game_test_thperrier.c#L?) | [int game_get_black_number(cgame g, uint i, uint j)](./game.h#L204-214)      | @thperrier  |         | <ul><li>[ ] </li></ul> |
| [game_is_marked](./game_test_nnunes.c#L?)           | [bool game_is_marked(cgame g, uint i, uint j)](./game.h#L216-226)            | @nnunes     |         | <ul><li>[ ] </li></ul> |
| [game_is_lighted](./game_test_nnunes.c#L?)          | [bool game_is_lighted(cgame g, uint i, uint j)](./game.h#L228-238)           | @nnunes     |         | <ul><li>[ ] </li></ul> |
| [game_has_error](./game_test_nnunes.c#L?)           | [bool game_has_error(cgame g, uint i, uint j)](./game.h#L240-252)            | @nnunes     |         | <ul><li>[ ] </li></ul> |
| [game_check_move](./game_test_nnunes.c#L?)          | [bool game_check_move(cgame g, uint i, uint j, square s)](./game.h#L242-269) | @nnunes     |         | <ul><li>[ ] </li></ul> |
| [game_play_move](./game_test_nnunes.c#L?)           | [void game_play_move(game g, uint i, uint j, square s)](./game.h#L271-284)   | @nnunes     |         | <ul><li>[ ] </li></ul> |
| [game_update_flags](./game_test_thoguet.c#L?)       | [void game_update_flags(game g)](./game.h#L286-293)                          | @thoguet    | a debug | <ul><li>[ ] </li></ul> |
| [game_is_over](./game_test_thoguet.c#L17)           | [bool game_is_over(cgame g)](./game.h#L295-304)                              | @thoguet    |         | <ul><li>[X] </li></ul> |
| [game_restart](./game_test_thoguet.c#L37)           | [void game_restart(game g)](./game.h#L306-313)                               | @thoguet    |         | <ul><li>[X] </li></ul> |
| [game_print](./game_test_thoguet.c#L70)             | [void game_print(cgame g)](./game_aux.h#L13-20)                              | @thoguet    |         | <ul><li>[X] </li></ul> |
| [game_default](./game_test_thoguet.c#L107)          | [game game_default(void)](./game_aux.h#L22-28)                               | @thoguet    |         | <ul><li>[X] </li></ul> |
| [game_default_solution](./game_test_thoguet.c#L125) | [game game_default_solution(void)](./game_aux.h#L30-36)                      | @thoguet    |         | <ul><li>[X] </li></ul> |