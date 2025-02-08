#include "../inc/common.h"

void input() {

  s_GameState *state = getGameState();
    state->hero->isMoving = false;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        state->hero->entityDest.x += MV_SPEED;
        state->hero->isMoving = true;
    }

    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        state->hero->entityDest.x -= MV_SPEED;
        state->hero->isMoving = true;
    }

    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
        state->hero->entityDest.y -= MV_SPEED;
        state->hero->isMoving = true;
    }

    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
        state->hero->entityDest.y += MV_SPEED;
        state->hero->isMoving = true;
    }
}
