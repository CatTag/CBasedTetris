#include "raylib.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int SCR_WIDTH = 600;
const int SCR_HEIGHT = 1000;
const int BLOCK_SIZE = 40;

struct block {
  int grid[4][4];
  Vector2 pos;
  Color color;
};

int max(int a, int b) { return a > b ? a : b; }
int min(int a, int b) { return a < b ? a : b; }

int main(void) {
  InitWindow(SCR_WIDTH, SCR_HEIGHT, "c based tetris");
  float timedownpressed = 0.;
  float timeleftpressed = 0.;
  float timerightpressed = 0.;
  float movebuffer = 0.;
  float timesince = 0.;
  int grid[10][20];
  for (int i = 0; i < 10; i++) {
    for (int o = 0; o < 20; o++) {
      grid[i][o] = 0;
    }
  }
  struct block mainblock;
  for (int i = 0; i < 4; i++) {
    for (int o = 0; o < 4; o++) {
      mainblock.grid[i][o] = 0;
    }
  }

  struct block nextblock;
  for (int i = 0; i < 4; i++) {
    for (int o = 0; o < 4; o++) {
      nextblock.grid[i][o] = 0;
    }
  }
  int randnum = rand() % 7;
  switch (randnum) {
  case 0: // i
    nextblock.grid[0][1] = 1;
    nextblock.grid[1][1] = 1;
    nextblock.grid[2][1] = 1;
    nextblock.grid[3][1] = 1;
    break;
  case 1: // j
    nextblock.grid[0][1] = 1;
    nextblock.grid[1][1] = 1;
    nextblock.grid[2][1] = 1;
    nextblock.grid[2][2] = 1;
    break;
  case 2: // l
    nextblock.grid[0][2] = 1;
    nextblock.grid[0][1] = 1;
    nextblock.grid[1][1] = 1;
    nextblock.grid[2][1] = 1;
    break;
  case 3: // o
    nextblock.grid[1][1] = 1;
    nextblock.grid[1][2] = 1;
    nextblock.grid[2][1] = 1;
    nextblock.grid[2][2] = 1;
    break;
  case 4: // s
    nextblock.grid[0][2] = 1;
    nextblock.grid[1][1] = 1;
    nextblock.grid[1][2] = 1;
    nextblock.grid[2][1] = 1;
    break;
  case 5: // z
    nextblock.grid[0][1] = 1;
    nextblock.grid[1][1] = 1;
    nextblock.grid[1][2] = 1;
    nextblock.grid[2][2] = 1;
    break;
  case 6: // t
    nextblock.grid[0][1] = 1;
    nextblock.grid[1][1] = 1;
    nextblock.grid[2][1] = 1;
    nextblock.grid[1][2] = 1;
    break;
  default:
    break;
  }

  nextblock.color = BLUE;
  nextblock.pos.x = 0;
  nextblock.pos.y = 0;

  int temparr[4][4];
  for (int i = 0; i < 4; i++) {
    for (int o = 0; o < 4; o++) {
      temparr[i][o] = 0;
    }
  }
  int nocolcount = 0;
  bool newblockneeded = true;
  bool newblockneededcheck = false;
  int filledlines = 0;
  bool canmove = true;
  int linechecked = 0;
  int score = 0;
  int linescleared = 0;

  srand(time(0));

  while (!WindowShouldClose()) {

    if (newblockneeded) {

      for (int i = 0; i < 4; i++) {
        for (int o = 0; o < 4; o++) {
          if (mainblock.grid[i][o] == 1) {
            grid[i + (int)mainblock.pos.x][o + (int)mainblock.pos.y] =
                mainblock.grid[i][o];
            mainblock.grid[i][o] = 0;
          }
        }
      }

      mainblock.pos.x = 3;
      mainblock.pos.y = -1;
      mainblock.color = BLUE;

      for (int i = 0; i < 4; i++) {
        for (int o = 0; o < 4; o++) {
          mainblock.grid[i][o] = nextblock.grid[i][o];
          nextblock.grid[i][o] = 0;
        }
      }

      randnum = rand() % 7;
      switch (randnum) {
      case 0: // i
        nextblock.grid[0][1] = 1;
        nextblock.grid[1][1] = 1;
        nextblock.grid[2][1] = 1;
        nextblock.grid[3][1] = 1;
        break;
      case 1: // j
        nextblock.grid[0][1] = 1;
        nextblock.grid[1][1] = 1;
        nextblock.grid[2][1] = 1;
        nextblock.grid[2][2] = 1;
        break;
      case 2: // l
        nextblock.grid[0][2] = 1;
        nextblock.grid[0][1] = 1;
        nextblock.grid[1][1] = 1;
        nextblock.grid[2][1] = 1;
        break;
      case 3: // o
        nextblock.grid[1][1] = 1;
        nextblock.grid[1][2] = 1;
        nextblock.grid[2][1] = 1;
        nextblock.grid[2][2] = 1;
        break;
      case 4: // s
        nextblock.grid[0][2] = 1;
        nextblock.grid[1][1] = 1;
        nextblock.grid[1][2] = 1;
        nextblock.grid[2][1] = 1;
        break;
      case 5: // z
        nextblock.grid[0][1] = 1;
        nextblock.grid[1][1] = 1;
        nextblock.grid[1][2] = 1;
        nextblock.grid[2][2] = 1;
        break;
      case 6: // t
        nextblock.grid[0][1] = 1;
        nextblock.grid[1][1] = 1;
        nextblock.grid[2][1] = 1;
        nextblock.grid[1][2] = 1;
        break;
      default:
        break;
      }
      newblockneeded = false;
    }
    if (IsKeyDown(KEY_LEFT)) {
      timeleftpressed += GetFrameTime();
    } else {
      timeleftpressed = 0.;
    }
    if (IsKeyPressed(KEY_LEFT) ||
        (timeleftpressed > 0.3) && movebuffer >= 0.05) {
      movebuffer = 0.;
      canmove = true;
      for (int i = 0; i < 4; i++) {
        for (int o = 0; o < 4; o++) {
          if ((mainblock.grid[i][o] == 1 &&
               grid[max(i + (int)mainblock.pos.x - 1, 0)]
                   [o + (int)mainblock.pos.y] != 0) ||
              (mainblock.grid[i][o] == 1 && i + mainblock.pos.x <= 0)) {
            canmove = false;
          }
        }
      }
      if (canmove) {
        mainblock.pos.x--;
      }
    }
    if (IsKeyDown(KEY_RIGHT)) {
      timerightpressed += GetFrameTime();
    } else {
      timerightpressed = 0.;
    }
    if (IsKeyPressed(KEY_RIGHT) ||
        (timerightpressed > 0.3) && movebuffer >= 0.05) {
      movebuffer = 0.;
      canmove = true;
      for (int i = 0; i < 4; i++) {
        for (int o = 0; o < 4; o++) {
          if ((mainblock.grid[i][o] == 1 &&
               grid[min(i + (int)mainblock.pos.x + 1, 9)]
                   [o + (int)mainblock.pos.y] != 0) ||
              (mainblock.grid[i][o] == 1 && i + mainblock.pos.x >= 9)) {
            canmove = false;
          }
        }
      }
      if (canmove) {
        mainblock.pos.x++;
      }
    }
    if (IsKeyDown(KEY_DOWN)) {
      timedownpressed += GetFrameTime();
    } else {
      timedownpressed = 0.;
    }
    movebuffer += GetFrameTime();
    if (IsKeyPressed(KEY_DOWN) ||
        (timedownpressed > 0.3) && movebuffer >= 0.05) {
      movebuffer = 0.;
      timesince = 0.;
      canmove = true;
      for (int i = 0; i < 4; i++) {
        for (int o = 0; o < 4; o++) {
          if ((mainblock.grid[i][o] == 1 &&
               grid[i + (int)mainblock.pos.x][o + (int)mainblock.pos.y + 1] !=
                   0) ||
              (mainblock.grid[i][o] == 1 && o + mainblock.pos.y >= 19)) {
            canmove = false;
            newblockneeded = true;
          }
        }
      }
      if (canmove) {
        mainblock.pos.y++;
      }
    }
    if (IsKeyPressed(KEY_Z)) {
      for (int i = 0; i < 4; i++) {
        for (int o = 0; o < 4; o++) {
          temparr[3 - o][i] = mainblock.grid[i][o];
        }
      }
      nocolcount = 0;
      for (int i = 0; i < 4; i++) {
        for (int o = 0; o < 4; o++) {
          if ((temparr[i][o] == 1 &&
               grid[i + (int)mainblock.pos.x][o + (int)mainblock.pos.y] == 0) &&
              (i + mainblock.pos.x <= 9 && i + mainblock.pos.x >= 0) &&
              o + mainblock.pos.y <= 19) {
            nocolcount++;
          }
        }
      }
      if (nocolcount >= 4) {
        for (int i = 0; i < 4; i++) {
          for (int o = 0; o < 4; o++) {
            mainblock.grid[i][o] = temparr[i][o];
          }
        }
      }
    }
    timesince += GetFrameTime();
    if (timesince > 1.) {
      timesince = 0.;
      canmove = true;

      for (int i = 0; i < 4; i++) {
        for (int o = 0; o < 4; o++) {
          if ((mainblock.grid[i][o] == 1 &&
               grid[i + (int)mainblock.pos.x][o + (int)mainblock.pos.y + 1] !=
                   0) ||
              (mainblock.grid[i][o] == 1 && o + mainblock.pos.y >= 19)) {
            canmove = false;
            if (newblockneededcheck) {
              newblockneeded = true;
              newblockneededcheck = false;
            } else {
              newblockneededcheck = true;
            }
          }
        }
      }
      if (canmove) {
        mainblock.pos.y++;
      }
    }
    linescleared = 0;
    for (int y = 0; y < 20; y++) {
      linechecked = 0;
      for (int x = 0; x < 10; x++) {
        if (grid[x][y] != 0) {
          linechecked++;
        }
      }
      if (linechecked >= 10) {
        linescleared++;
        for (int x = 0; x < 10; x++) {
          grid[x][y] = 0;
        }
        for (int y1 = y; y1 > 0; y1--) {
          for (int x = 0; x < 10; x++) {
            grid[x][y1] = grid[x][y1 - 1];
          }
        }
      }
    }
    score += pow(linescleared, 2) * 100;

    BeginDrawing();
    ClearBackground(BLACK);
    for (int i = 0; i < 10; i++) {
      for (int o = 0; o < 20; o++) {
        if (grid[i][o] == 1) {

          DrawRectangle(i * BLOCK_SIZE, o * BLOCK_SIZE + 100, BLOCK_SIZE,
                        BLOCK_SIZE, BLUE);
        } else if (grid[i][o] == 2) {
          DrawRectangle(i * BLOCK_SIZE, o * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE,
                        WHITE);
        } else if (grid[i][o] == 3) {
          DrawRectangle(i * BLOCK_SIZE, o * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE,
                        RED);
        }
      }
    }
    for (int i = 0; i < 4; i++) {
      for (int o = 0; o < 4; o++) {
        if (mainblock.grid[i][o] == 1) {
          DrawRectangle((i + mainblock.pos.x) * BLOCK_SIZE,
                        (o + mainblock.pos.y) * BLOCK_SIZE + 100, BLOCK_SIZE,
                        BLOCK_SIZE, mainblock.color);
        }
      }
    }
    DrawRectangle(0, 0, 600, 100, GRAY);
    DrawRectangle(400, 100, 200, 800, GRAY);
    DrawRectangle(0, 900, 600, 100, GRAY);
    DrawText("C Based Tetris!", 20, 20, 60, WHITE);
    DrawText("Next Block:", 425, 400, 20, WHITE);
    DrawRectangle(425, 425, 150, 150, BLACK);
    for (int i = 0; i < 4; i++) {
      for (int o = 0; o < 4; o++) {
        if (nextblock.grid[i][o] == 1) {
          DrawRectangle(i * 30 + 455, o * 30 + 455, 30, 30, nextblock.color);
        }
      }
    }
    DrawText(TextFormat("Score: %i", score), 20, 950, 40, WHITE);

    EndDrawing();
  }
  CloseWindow();
  return 0;
}
