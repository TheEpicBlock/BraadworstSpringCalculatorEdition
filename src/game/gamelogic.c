#include <tice.h>
#include <keypadc.h>

#include "game/gamelogic.h"
#include "rendering/renderer.h"
#include "gfx/generated/gfx.h"

const int OBSTACLE_SPACING = LCD_WIDTH * 0.9f;
const int OBSTACLE_MAX_OFFSET = 5;
const int OBSTACLE_SPEED = 80;

const int SECOND = 32768;
const int TIMER = 3;

static float Max(float a, float b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

int GetNewPositionOffset(void) {
    return OBSTACLE_SPACING + 0;
}

void game_InitState(struct GameState *state) {
    // Initialize obstacles
    float pos = GetNewPositionOffset();
    for (int i = 0; i < OBSTACLE_COUNT; i += 1) {
        state->obstacles[i].position = pos;
        pos += GetNewPositionOffset();
    }
    state->lastObstacle = OBSTACLE_COUNT-1;

    // Initialize timers    
    timer_Set(TIMER, 0);
    timer_Enable(TIMER, TIMER_32K, TIMER_NOINT, TIMER_UP);
}

bool game_Tick(struct GameState *state) {
    kb_Scan();
    
    // Check exit
    if (kb_IsDown(kb_KeyDel) || kb_IsDown(kb_KeyClear)) {
        return true;
    }
    
    // Calculate time
    int deltaTime = timer_Get(TIMER);
    timer_Set(TIMER, 0);
    
    float delta = (float)deltaTime / (float)SECOND;
    
    // Move obstacles
    for (int i = 0; i < OBSTACLE_COUNT; i += 1) {
        state->obstacles[i].position -= OBSTACLE_SPEED * delta;
        if (state->obstacles[i].position < -genericobstacle_width) {
            state->obstacles[i].position = state->obstacles[state->lastObstacle].position + GetNewPositionOffset();
            state->lastObstacle = i;
            state->score += 3;
        }
    }
    
    // Jumping
    state->timeJumped += delta;
    
    float x = state->timeJumped;
    float playerY = Max(-(x*(x-3))/2.25, 0);
    
    if (playerY == 0) {
        if (kb_IsDown(kb_KeyEnter) || kb_IsDown(kb_KeyUp)) {
            state->timeJumped = 0;
        }
    }
    
    // Render game
    rend_RenderGame(state, playerY);
    
    // Don't go too fast
    while (timer_Get(TIMER) < SECOND/60);
    
    return false;
}
