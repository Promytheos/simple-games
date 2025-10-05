#include "screen.h"
#include <raylib.h>

int main(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Simple Games");
  SetTargetFPS(FPS);
  init();

  bool is_paused = false;
  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_R)) {
      cleanup();
      init();
    }
    if (IsKeyPressed(KEY_P)) {
      is_paused = !is_paused;
    }
    if (!is_paused) {
      update();
    }
    BeginDrawing();
    draw();
    EndDrawing();
  }

  CloseWindow();
  cleanup();

  return 0;
}
