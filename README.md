# Snake C

Classic Snake game written in C using the **Raylib** and **Raymath** libraries.

The player controls a snake that grows by eating apples. The game ends when the snake collides with the walls or with its own body.

![snake gif](snake.gif)

## Installation

1. Install **Raylib** (with `pkg-config` support):
https://github.com/raysan5/raylib?tab=readme-ov-file#build-and-installation

2. Clone this repository:

```bash
git clone https://github.com/manursutil/snake-c.git
cd snake-c
```

3. Ensure the build script is executable:

```bash
chmod +x build.sh
```

`raymath.h` is included in this repository.

## Usage

Build the refactored version from `src/`:

```bash
./build.sh
```

Run the game:

```bash
./build/snake
```

Notes:
- The current build flow compiles C sources under `src/` only.

## Controls

- ⬆️ Up Arrow: move up
- ⬇️ Down Arrow: move down
- ⬅️ Left Arrow: move left
- ➡️ Right Arrow: move right

## Project Structure 

```text
.
├── build.sh                 # Build script for refactored sources
├── raymath.h                # Raymath header
├── src
│   └── c_engine
│       ├── main.c           # Rendering loop + input
│       ├── engine.c         # Game logic/state updates
│       ├── engine.h         # Engine API and shared types/constants
│       ├── env_api.c        # Placeholder for environment API
│       └── env_api.h        # Placeholder for environment API
└── README.md
```
