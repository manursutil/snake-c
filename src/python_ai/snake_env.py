import numpy as np
import ctypes
from ctypes import c_int, Structure, POINTER

MAX_LENGTH = 500


class Vector2i(Structure):
    _fields_ = [("x", c_int), ("y", c_int)]


class SnakeSegment(Structure):
    _fields_ = [("pos", Vector2i)]


class Velocity(Structure):
    _fields_ = [("vx", c_int), ("vy", c_int)]


class Snake(Structure):
    _fields_ = [
        ("body", SnakeSegment * MAX_LENGTH),
        ("length", c_int),
        ("v", Velocity),
    ]


class Apple(Structure):
    _fields_ = [("x", c_int), ("y", c_int), ("w", c_int), ("h", c_int)]


class GameState(Structure):
    _fields_ = [
        ("snake", Snake),
        ("apple", Apple),
        ("gameOver", c_int),
        ("score", c_int),
    ]


class SnakeEnv:
    CELL = 20
    WIDTH = 900
    HEIGHT = 600

    def __init__(self, lib_path="build/libsnake.so", seed=42) -> None:
        self.lib = ctypes.CDLL(lib_path)

        self.lib.engine_get_state.argtypes = []
        self.lib.engine_get_state.restype = POINTER(GameState)
        self.lib.engine_init.argtypes = [c_int]
        self.lib.engine_init.restype = None
        self.lib.engine_reset.argtypes = []
        self.lib.engine_reset.restype = None
        self.lib.engine_step.argtypes = [c_int]
        self.lib.engine_step.restype = None
        self.lib.engine_is_done.argtypes = []
        self.lib.engine_is_done.restype = c_int
        self.lib.engine_get_score.argtypes = []
        self.lib.engine_get_score.restype = c_int

        self.lib.engine_init(seed)

    def reset(self) -> np.ndarray:
        self.lib.engine_reset()
        return self._get_obs()

    def step(self, action) -> tuple[np.ndarray, float, bool]:
        self.lib.engine_step(action)
        obs = self._get_obs()
        reward = self.lib.engine_get_reward()
        done = self.lib.engine_is_done()

        return obs, reward, done

    def _get_obs(self) -> np.ndarray:
        state = self.lib.engine_get_state().contents

        head_x = int(state.snake.body[0].pos.x) // SnakeEnv.CELL
        head_y = int(state.snake.body[0].pos.y) // SnakeEnv.CELL

        vx = state.snake.v.vx
        vy = state.snake.v.vy

        moving_left = 1 if (vx, vy) == (-1, 0) else 0
        moving_right = 1 if (vx, vy) == (1, 0) else 0
        moving_up = 1 if (vx, vy) == (0, -1) else 0
        moving_down = 1 if (vx, vy) == (0, 1) else 0

        apple_x = state.apple.x // SnakeEnv.CELL
        apple_y = state.apple.y // SnakeEnv.CELL

        food_left = 1 if apple_x < head_x else 0
        food_right = 1 if apple_x > head_x else 0
        food_up = 1 if apple_y < head_y else 0
        food_down = 1 if apple_y > head_x else 0

        forward = (vx, vy)
        left = (-vy, vx)
        right = (vy, -vx)

        danger_straight = (
            1 if self._danger_at(state, head_x + forward[0], head_y + forward[1]) else 0
        )
        danger_left = (
            1 if self._danger_at(state, head_x + left[0], head_y + left[1]) else 0
        )
        danger_right = (
            1 if self._danger_at(state, head_x + right[0], head_y + right[1]) else 0
        )

        obs = np.array(
            [
                moving_left,
                moving_right,
                moving_up,
                moving_down,
                food_left,
                food_right,
                food_up,
                food_down,
                danger_left,
                danger_right,
                danger_straight,
            ],
            dtype=np.float32,
        )

        return obs

    def _danger_at(self, state, x, y):
        if (
            x < 0
            or x >= SnakeEnv.WIDTH // SnakeEnv.CELL
            or y < 0
            or y >= SnakeEnv.HEIGHT // SnakeEnv.CELL
        ):
            return True

        for i in range(1, state.snake.length):
            bx = int(state.snake.body[i].pos.x) // SnakeEnv.CELL
            by = int(state.snake.body[i].pos.y) // SnakeEnv.CELL
            if bx == x and by == y:
                return True

        return False
