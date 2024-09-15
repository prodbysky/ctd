#include <raylib.h>

int main() {
  InitWindow(1280, 720, "Hello world!");
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(GetColor(0x181818ff));
    EndDrawing();
  }
  CloseWindow();
}
