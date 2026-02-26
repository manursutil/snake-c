#include "../engine.h"

#include <assert.h>
#include <stdio.h>
#include <stddef.h>

static void assert_initial_state(void) {
    engine_init(42);
    GameState *s = engine_get_state();

    assert(s != NULL);
    assert(s->score == 0);
    assert(s->gameOver == 0);
    assert(s->snake.length == 1);
    assert(s->snake.v.vx == 1);
    assert(s->snake.v.vy == 0);
    assert(engine_get_reward() == 0);
    assert(s->apple.w == CELL);
    assert(s->apple.h == CELL);
    assert(s->apple.x % CELL == 0);
    assert(s->apple.y % CELL == 0);
}

static void assert_movement_and_invalid_action(void) {
    engine_init(42);
    GameState *s = engine_get_state();
    int x0 = s->snake.body[0].pos.x;
    int y0 = s->snake.body[0].pos.y;

    engine_step(0);
    assert(s->snake.body[0].pos.x == x0 + CELL);
    assert(s->snake.body[0].pos.y == y0);
    assert(engine_get_reward() == 0);

    engine_step(99);
    assert(s->snake.body[0].pos.x == x0 + (2 * CELL));
    assert(s->snake.body[0].pos.y == y0);
}

static void assert_apple_eating(void) {
    engine_init(42);
    GameState *s = engine_get_state();

    int head_x = s->snake.body[0].pos.x;
    int head_y = s->snake.body[0].pos.y;

    s->apple.x = head_x + CELL;
    s->apple.y = head_y;

    engine_step(0);

    assert(s->score == 1);
    assert(s->snake.length == 2);
    assert(engine_get_reward() == 1);
    assert(s->apple.x % CELL == 0);
    assert(s->apple.y % CELL == 0);
}

static void assert_wall_collision_and_post_game_over_step(void) {
    engine_init(42);
    GameState *s = engine_get_state();

    s->snake.body[0].pos.x = WIDTH - CELL;
    s->snake.body[0].pos.y = 0;
    s->snake.v.vx = 1;
    s->snake.v.vy = 0;

    engine_step(0);
    assert(engine_is_done() == 1);
    assert(s->gameOver == 1);
    assert(engine_get_reward() == -1);

    int dead_x = s->snake.body[0].pos.x;
    int dead_y = s->snake.body[0].pos.y;

    engine_step(0); // should be ignored after game over
    assert(s->snake.body[0].pos.x == dead_x);
    assert(s->snake.body[0].pos.y == dead_y);
    assert(engine_get_reward() == -1);
}

static void assert_reset_clears_game_over_and_score(void) {
    engine_init(42);
    GameState *s = engine_get_state();

    s->gameOver = 1;
    s->score = 7;
    s->snake.length = 4;
    s->snake.v.vx = 0;
    s->snake.v.vy = 1;

    engine_reset();

    assert(s->gameOver == 0);
    assert(s->score == 0);
    assert(s->snake.length == 1);
    assert(s->snake.v.vx == 1);
    assert(s->snake.v.vy == 0);
    assert(engine_get_reward() == 0);
}

int main(void) {
    assert_initial_state();
    assert_movement_and_invalid_action();
    assert_apple_eating();
    assert_wall_collision_and_post_game_over_step();
    assert_reset_clears_game_over_and_score();
    printf("All tests passed!\n");
    return 0;
}
