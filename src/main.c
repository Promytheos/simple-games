#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

float calculateTrajectory(int distanceFromCentre);

const int SCREEN_WIDTH = 980;
const int SCREEN_HEIGHT = 640;
const int GAME_TOP = 20;
const int GAME_BOTTOM = SCREEN_HEIGHT;
const int GAME_LEFT = 0;
const int GAME_RIGHT = SCREEN_WIDTH;
const int GAME_HEIGHT = GAME_BOTTOM - GAME_TOP;
const int GAME_WIDTH = GAME_RIGHT - GAME_LEFT;
const Vector2 GAME_CENTRE = {GAME_LEFT + ((float)GAME_WIDTH / 2),
                             GAME_TOP + ((float)GAME_HEIGHT / 2)};

const int MAX_BALL_SPEED = 10;

struct Ball {
  float radius;
  int x;
  int y;
  Vector2 velocity;
};

struct Player {
  int x;
  int y;
  float width;
  float height;
  float velocity;
  float acceleration;
};

int get_ball_top(struct Ball ball) { return ball.y - ball.radius; }
int get_ball_bot(struct Ball ball) { return ball.y + ball.radius; }
int get_ball_left(struct Ball ball) { return ball.x - ball.radius; }
int get_ball_right(struct Ball ball) { return ball.x + ball.radius; }
int get_paddle_top(struct Player rec) { return rec.y; }
int get_paddle_bot(struct Player rec) { return rec.y + rec.height; }
int get_paddle_left(struct Player rec) { return rec.x; }
int get_paddle_right(struct Player rec) { return rec.x + rec.width; }

void reset_ball(struct Ball *ball) {
  ball->x = GAME_CENTRE.x;
  ball->y = GAME_CENTRE.y;
  ball->velocity.x = rand() % 5 + 5;
  ball->velocity.y = rand() % 5;
}

int main(int argcount, char **argvariables) {
  char *game = argcount > 1 ? argvariables[1] : "pong";
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Simple Games");

  const char *player_name = "Player";
  const char *ai_name = "AI";
  const int rectangle_width = 10;
  const int rectangle_height = 90;
  const int ball_radius = 15;

  const float player_velocity = 2.0;
  const float ai_velocity = 1.0;

  char score[4];

  int player_score = 0;
  int ai_score = 0;

  struct Player player = {GAME_LEFT,        GAME_CENTRE.y, rectangle_width,
                          rectangle_height, 0.0,           0.0};

  struct Player ai = {GAME_WIDTH - rectangle_width,
                      GAME_CENTRE.y,
                      rectangle_width,
                      rectangle_height,
                      0.0,
                      0.0};

  struct Ball ball = {ball_radius, GAME_CENTRE.x, GAME_CENTRE.y, {5.0, 1.0}};

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    player.y += player.velocity;
    ai.y += ai.velocity;
    ball.x += ball.velocity.x;
    ball.y += ball.velocity.y;

    if (IsKeyDown(KEY_W)) {
      player.velocity = -player_velocity;
      if (get_paddle_top(player) < GAME_TOP) {
        player.y = GAME_TOP;
        player.velocity = 0.0;
      }
    } else if (IsKeyDown(KEY_S)) {
      player.velocity = player_velocity;
      if (get_paddle_bot(player) > GAME_BOTTOM) {
        player.y = GAME_BOTTOM - player.height;
        player.velocity = 0.0;
      }
    } else {
      player.velocity = 0.0;
    }

    if (ball.velocity.x > 0) {
      if (get_paddle_bot(ai) - 10 < get_ball_top(ball)) {
        ai.velocity = ai_velocity;
        if (get_paddle_bot(ai) > GAME_BOTTOM) {
          ai.velocity = 0;
          ai.y = GAME_BOTTOM - ai.height;
        }
      } else if (get_paddle_top(ai) + 10 > get_ball_bot(ball)) {
        ai.velocity = -ai_velocity;
        if (get_paddle_top(ai) < GAME_TOP) {
          ai.velocity = 0;
          ai.y = GAME_TOP;
        }
      } else {
        ai.velocity = 0;
      }
    } else {
      ai.velocity = 0;
    }

    bool ai_collision = get_ball_right(ball) >= get_paddle_left(ai) &&
                        get_ball_bot(ball) > get_paddle_top(ai) &&
                        get_ball_top(ball) < get_paddle_bot(ai);

    bool player_collision = get_ball_left(ball) <= get_paddle_right(player) &&
                            get_ball_bot(ball) >= get_paddle_top(player) &&
                            get_ball_top(ball) <= get_paddle_bot(player) &&
                            get_ball_right(ball) >= get_paddle_left(player);

    if (get_ball_top(ball) < GAME_TOP || get_ball_bot(ball) > GAME_BOTTOM) {
      ball.velocity.y = -ball.velocity.y;
    }

    if (ai_collision || player_collision) {
      ball.velocity.x = -ball.velocity.x;
    }

    if (get_ball_left(ball) > GAME_RIGHT) {
      player_score++;
      reset_ball(&ball);
    }

    if (get_ball_right(ball) < GAME_LEFT) {
      ai_score++;
      reset_ball(&ball);
    }

    sprintf(score, "%d:%d", player_score, ai_score);

    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawLine(0, GAME_TOP, SCREEN_WIDTH, GAME_TOP, LIGHTGRAY);
    DrawLine(SCREEN_WIDTH / 2, GAME_TOP, SCREEN_WIDTH / 2, GAME_BOTTOM,
             LIGHTGRAY);
    DrawText(game, GAME_CENTRE.x + 10, GAME_TOP, 20, LIGHTGRAY);
    int score_width = MeasureText(score, 20);
    DrawText(score, SCREEN_WIDTH / 2 - (score_width / 2), 0, 20, BLACK);
    int player_width = MeasureText(player_name, 20);
    DrawText(player_name, SCREEN_WIDTH / 4 - (player_width / 2), 0, 20, BLACK);
    int ai_width = MeasureText(ai_name, 20);
    DrawText(ai_name, SCREEN_WIDTH - SCREEN_WIDTH / 4 - (ai_width / 2), 0, 20,
             BLACK);

    DrawRectangle(player.x, player.y, player.width, player.height, BLACK);
    DrawRectangle(ai.x, ai.y, ai.width, ai.height, BLACK);
    DrawCircle(ball.x, ball.y, ball.radius, BLACK);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
