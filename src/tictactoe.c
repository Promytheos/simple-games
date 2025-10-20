#include "screen.h"
#include <raylib.h>
#include <rlgl.h>
#include <stdlib.h>

#define CELL_WIDTH 100
#define CELL_HEIGHT 100
#define CENTER_X (SCREEN_WIDTH / 2)
#define CENTER_Y (SCREEN_HEIGHT / 2)
#define NUM_CELLS 9
#define BOARD_X 100
#define BOARD_Y 100
#define CIRCLE_RAD 30
#define X_LEN 30
#define X_OFFSET 20
#define Y_OFFSET 20

struct Cell {
  int row;
  int column;
  char state;
  Vector2 position;
};

struct Cell *cells;
Vector2 boardPosition = {.x = BOARD_X, .y = BOARD_Y};
Vector2 boardScale = {.x = 1.0f, .y = 1.0f};

void init(void) {
  cells = malloc(sizeof(struct Cell) * NUM_CELLS);
  for (int i = 0; i < NUM_CELLS; i++) {
    struct Cell cell = {.row = i / 3, .column = i % 3, .state = ' '};
    cells[i] = cell;
  }
}

void update(void) {
  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    float mouse_x = GetMouseX();
    float mouse_y = GetMouseY();
  }
}

void draw(void) {
  ClearBackground(BLACK);
  rlPushMatrix();
  rlTranslatef(boardPosition.x, boardPosition.y, 0.0f);
  rlScalef(boardScale.x, boardScale.y, 1);
  DrawRectangleLines(0, 0, CELL_WIDTH * 3, CELL_HEIGHT * 3, WHITE);
  for (int i = 0; i < NUM_CELLS; i++) {
    struct Cell cell = cells[i];
    if (i % 3 == 0) {
      cell.state = 'x';
    }
    if (i % 2 == 0) {
      cell.state = 'o';
    }
    rlPushMatrix();
    rlTranslatef(CELL_WIDTH * cell.column + X_OFFSET,
                 CELL_HEIGHT * cell.row + Y_OFFSET, 0.0f);
    switch (cell.state) {
    case 'x':
      DrawLineV((Vector2){.x = 0, .y = 0},
                (Vector2){.x = CIRCLE_RAD * 2, .y = CIRCLE_RAD * 2}, RAYWHITE);
      DrawLineV((Vector2){.x = 0, .y = CIRCLE_RAD * 2},
                (Vector2){.x = CIRCLE_RAD * 2, .y = 0}, RAYWHITE);
      break;
    case 'o':
      DrawCircleLines(CIRCLE_RAD, CIRCLE_RAD, CIRCLE_RAD, RAYWHITE);
      break;
    case ' ':
      break;
    }
    rlPopMatrix();
  }

  DrawLineV((Vector2){.x = CELL_WIDTH, .y = 0},
            (Vector2){.x = CELL_WIDTH, .y = CELL_HEIGHT * 3}, WHITE);
  DrawLineV((Vector2){.x = CELL_WIDTH * 2, .y = 0},
            (Vector2){.x = CELL_WIDTH * 2, .y = CELL_HEIGHT * 3}, WHITE);

  DrawLineV((Vector2){.x = 0, .y = CELL_HEIGHT},
            (Vector2){.x = CELL_WIDTH * 3, .y = CELL_HEIGHT}, WHITE);
  DrawLineV((Vector2){.x = 0, .y = CELL_HEIGHT * 2},
            (Vector2){.x = CELL_WIDTH * 3, .y = CELL_HEIGHT * 2}, WHITE);

  rlPopMatrix();
}

void cleanup(void) { free(cells); }
