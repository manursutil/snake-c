#include "raylib.h"
#include "raymath.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 900
#define HEIGHT 600
#define FPS 60

#define CELL 20
#define MAX_LENGTH 500

typedef struct {
	int vx, vy;
} Velocity;

// typedef struct {
// 	int x, y, w, h;
// 	Velocity v;
// } Snake;

typedef struct {
	Vector2 pos;
} SnakeSegment;

typedef struct {
	SnakeSegment body[MAX_LENGTH];
	int length;
	Velocity v;
} Snake;

typedef struct {
	int x, y, w, h;
} Apple;

void checkDirection(Snake *s) {
	if (IsKeyDown(KEY_RIGHT)) s->v = (Velocity){1,0};
    if (IsKeyDown(KEY_LEFT)) s->v = (Velocity){-1,0};
    if (IsKeyDown(KEY_UP)) s->v = (Velocity){0,-1};
    if (IsKeyDown(KEY_DOWN)) s->v = (Velocity){0,1};
}

void updateSnake(Snake* s, int* gameOver) {
// 	if (!*gameOver) {
// 		s->x += s->v.vx * CELL;
// 		s->y += s->v.vy * CELL;
// 	}
// 	
// 	if (s->x + s->w > WIDTH) *gameOver = 1;
// 	if (s->x + s->w < 0) *gameOver = 1;
// 	if (s->y + s->h > HEIGHT) *gameOver = 1;
// 	if (s->y - s->h < 0) *gameOver = 1;

	for (int i = s->length -1; i > 0; i--) {
		s->body[i].pos = s->body[i - 1].pos;
	}
	
	s->body[0].pos.x += s->v.vx * CELL;
	s->body[0].pos.y += s->v.vy * CELL;
	
    Vector2 head = s->body[0].pos;
    if (head.x < 0 || head.x >= WIDTH || head.y < 0 || head.y >= HEIGHT) *gameOver = 1;
	
	for (int i = 1; i < s->length; i++) {
		if (Vector2Equals(head, s->body[i].pos)) {
			*gameOver = 1;
		}
	}
}

int checkSnakeEats(Snake *s, Apple *a) {
	Vector2 head = s->body[0].pos;
	
	if (head.x == a->x && head.y == a->y) {
		if (s->length < MAX_LENGTH) {
			s->body[s->length].pos = s->body[s->length - 1].pos;
			s->length++;
		}
		return 1;
	}
	return 0;
}

void DrawGrid2D(int cellSize, Color color) {
	for (int x = 0; x <= WIDTH; x+=cellSize) DrawLine(x, 0, x, HEIGHT, color);
	for (int y = 0; y <= HEIGHT; y+=cellSize) DrawLine(0, y, WIDTH, y, color);
}

Snake initSnake() {
	Snake snake = {0};
	snake.length = 1;
	snake.body[0].pos = (Vector2) {(WIDTH/2 / CELL) * CELL, (HEIGHT/2 / CELL) * CELL};
	snake.v = (Velocity){1,0};
	return snake;
}

void DrawSnake(Snake *s) {
	for (int i = 0; i < s->length; i++) {
		DrawRectangle(s->body[i].pos.x, s->body[i].pos.y, 20, 20, (i == 0) ? DARKBLUE : BLUE);
	}
}

void DrawApple(Apple *a) {
	DrawRectangle(a->x, a->y, a->w, a->h, RED);
}

Vector2 getApplePosition() {
	int posX = ((rand() % WIDTH + 1) / CELL) * CELL;
	int posY = ((rand() % HEIGHT + 1) / CELL) * CELL;
	return (Vector2){posX,posY};
}

int main(void) {
	srand(time(NULL));

    InitWindow(WIDTH, HEIGHT, "Snake");
    
    Snake snake = initSnake();
    Vector2 applePos = getApplePosition();
    Apple apple = {applePos.x, applePos.y, 20, 20};
    
    int movesPerSecond = 10;
	int moveEveryNFrames = FPS / movesPerSecond;
	int frameCounter = 0;
	
	int gameOver = 0;
	
	float plusOneTimer = 0.0f;
	float PLUS_ONE_DURATION = 1.0f;
	
	int score = 0;

	SetTargetFPS(FPS);
    while (!WindowShouldClose()) {
		frameCounter++;
		
		if (gameOver) DrawText("Game Over", WIDTH/2 - 100, HEIGHT/2 - 20, 50, RED);

		checkDirection(&snake);
		if (checkSnakeEats(&snake, &apple)) {
			applePos = getApplePosition();
			apple.x = applePos.x;
			apple.y = applePos.y;
			
			plusOneTimer = PLUS_ONE_DURATION;
			score++;
		}
		
		float deltaTime = GetFrameTime();
		if (plusOneTimer > 0.0f) plusOneTimer -= deltaTime;

		if (frameCounter >= moveEveryNFrames) {
    		updateSnake(&snake, &gameOver);
    		frameCounter = 0;
		}
        
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawGrid2D(CELL, BLACK);
            DrawSnake(&snake);
            DrawApple(&apple);
            if (plusOneTimer > 0.0f) DrawText("+1", WIDTH/2, HEIGHT/2 - 20, 50, LIME);
            DrawText(TextFormat("Score: %d", score), 10, 10, 30, LIME);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}