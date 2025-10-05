#include "screen.h"
#include <math.h>
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define GAME_TOP 20
#define GAME_BOTTOM SCREEN_HEIGHT
#define GAME_LEFT 0
#define GAME_RIGHT SCREEN_WIDTH
#define GAME_HEIGHT (GAME_BOTTOM - GAME_TOP)
#define GAME_WIDTH (GAME_RIGHT - GAME_LEFT)
#define MAX_BALL_SPEED 10
#define MAX_SCORE 12
#define MAX_PADDLE_VELOCITY 5
#define PADDLE_WIDTH 10
#define PADDLE_HEIGHT 90
#define BALL_RADIUS 10
#define PADDLE_ACCELERATION 2.0f
#define AI_TRACKING_OFFSET 10
#define MIN_BALL_SPEED 5
#define MAX_BALL_SPEED_RANGE 3
#define GAME "Pong"
#define SCORE_DIVIDER ":"

const Vector2 GAME_CENTRE = {GAME_LEFT + ((float)GAME_WIDTH / 2),
                             GAME_TOP + ((float)GAME_HEIGHT / 2)};

struct Ball {
  float radius;
  float x, y;
  Vector2 velocity;
};

struct Player {
  float x, y;
  float width, height;
  float velocity;
  float acceleration;
};

Vector2 calculateTrajectory(float distanceFromCentre, Vector2 incomingVelocity);
void reset_ball(struct Ball *ball);

float get_ball_top(struct Ball ball) { return ball.y - ball.radius; }
float get_ball_bot(struct Ball ball) { return ball.y + ball.radius; }
float get_ball_left(struct Ball ball) { return ball.x - ball.radius; }
float get_ball_right(struct Ball ball) { return ball.x + ball.radius; }
float get_paddle_top(struct Player rec) { return rec.y; }
float get_paddle_bot(struct Player rec) { return rec.y + rec.height; }
float get_paddle_left(struct Player rec) { return rec.x; }
float get_paddle_right(struct Player rec) { return rec.x + rec.width; }

const char *player_name = "Player";
const char *ai_name = "AI";
int player_score = 0;
int ai_score = 0;
struct Player player;
struct Player ai;
struct Ball ball;

struct Player create_player(float x, float y) {
  struct Player player = {.x = x,
                          .y = y,
                          .width = PADDLE_WIDTH,
                          .height = PADDLE_HEIGHT,
                          .velocity = 0.0,
                          .acceleration = PADDLE_ACCELERATION};

  return player;
}

void init() {
  player = create_player(GAME_LEFT, GAME_CENTRE.y);
  ai = create_player(GAME_WIDTH - PADDLE_WIDTH, GAME_CENTRE.y);

  ball.radius = BALL_RADIUS;
  reset_ball(&ball);
}

void update() {
  if (player_score >= MAX_SCORE || ai_score >= MAX_SCORE) {
    player_score = 0;
    ai_score = 0;
    reset_ball(&ball);
  }

  player.y += player.velocity;
  ai.y += ai.velocity;
  ball.x += ball.velocity.x;
  ball.y += ball.velocity.y;

  if (IsKeyDown(KEY_W)) {
    if (player.velocity > 0) {
      player.velocity = 0;
    }
    player.velocity -= player.acceleration;
    if (get_paddle_top(player) < GAME_TOP) {
      player.y = GAME_TOP;
      player.velocity = 0.0;
    }
  } else if (IsKeyDown(KEY_S)) {
    if (player.velocity < 0) {
      player.velocity = 0;
    }
    player.velocity += player.acceleration;
    if (get_paddle_bot(player) > GAME_BOTTOM) {
      player.y = GAME_BOTTOM - player.height;
      player.velocity = 0.0;
    }
  } else {
    player.velocity = 0.0;
  }

  if (ball.velocity.x > 0) {
    if (get_paddle_bot(ai) - AI_TRACKING_OFFSET < get_ball_top(ball)) {
      if (ai.velocity < 0) {
        ai.velocity = 0;
      }
      ai.velocity += ai.acceleration;
      if (get_paddle_bot(ai) > GAME_BOTTOM) {
        ai.velocity = 0;
        ai.y = GAME_BOTTOM - ai.height;
      }
    } else if (get_paddle_top(ai) + AI_TRACKING_OFFSET > get_ball_bot(ball)) {
      if (ai.velocity > 0) {
        ai.velocity = 0;
      }
      ai.velocity -= ai.acceleration;
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

  if (fabs(ai.velocity) > MAX_PADDLE_VELOCITY) {
    ai.velocity = ai.velocity > 0 ? MAX_PADDLE_VELOCITY : -MAX_PADDLE_VELOCITY;
  }
  if (fabs(player.velocity) > MAX_PADDLE_VELOCITY) {
    player.velocity =
        player.velocity > 0 ? MAX_PADDLE_VELOCITY : -MAX_PADDLE_VELOCITY;
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
}

void draw() {
  ClearBackground(RAYWHITE);

  DrawLine(0, GAME_TOP, SCREEN_WIDTH, GAME_TOP, LIGHTGRAY);
  DrawLine(SCREEN_WIDTH / 2, GAME_TOP, SCREEN_WIDTH / 2, GAME_BOTTOM,
           LIGHTGRAY);
  DrawText(GAME, GAME_CENTRE.x + 10, GAME_TOP, 20, LIGHTGRAY);
  int divider_width = MeasureText(SCORE_DIVIDER, 20);
  DrawText(SCORE_DIVIDER, SCREEN_WIDTH / 2 - (divider_width / 2), 0, 20, BLACK);

  char player_score_text[4];
  sprintf(player_score_text, "%d", player_score);
  int player_score_width = MeasureText(player_score_text, 20);
  DrawText(player_score_text, SCREEN_WIDTH / 2 - divider_width - player_score_width, 0, 20, BLACK);

  char ai_score_text[4];
  sprintf(ai_score_text, "%d", ai_score);
  DrawText(ai_score_text, SCREEN_WIDTH / 2 + divider_width, 0, 20, BLACK);

  int player_width = MeasureText(player_name, 20);
  DrawText(player_name, SCREEN_WIDTH / 4 - (player_width / 2), 0, 20, BLACK);
  int ai_width = MeasureText(ai_name, 20);
  DrawText(ai_name, SCREEN_WIDTH - SCREEN_WIDTH / 4 - (ai_width / 2), 0, 20,
           BLACK);

  DrawRectangle(player.x, player.y, player.width, player.height, BLACK);
  DrawRectangle(ai.x, ai.y, ai.width, ai.height, BLACK);
  DrawCircle(ball.x, ball.y, ball.radius, BLACK);
}

void cleanup() {}

void reset_ball(struct Ball *ball) {
  ball->x = GAME_CENTRE.x;
  ball->y = GAME_CENTRE.y;
  ball->velocity.x = rand() % 5 + MIN_BALL_SPEED;
  ball->velocity.y = rand() % 5 + MAX_BALL_SPEED_RANGE;
}

Vector2 calculateTrajectory(float distanceFromCenter,
                            Vector2 incomingVelocity) {
  Vector2 speed = {0, 0};

  return speed;
}
