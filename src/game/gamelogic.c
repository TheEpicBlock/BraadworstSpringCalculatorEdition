#include <tice.h>
#include <keypadc.h>

#include "game/gamelogic.h"
#include "rendering/renderer.h"


const int OBSTACLE_SPACING = LCD_WIDTH * 0.75f;
const int OBSTACLE_MAX_OFFSET = 5;
const int OBSTACLE_SPEED = 20;

const int SECOND = 32768;
const int TIMER = 1;

static double Max(double a, double b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

int GetNewPositionOffset(void) {
    return 0;
}

void game_InitState(struct GameState *state) {
    for (int i = 0; i < OBSTACLE_COUNT; i += 1) {
        state->obstacles[i].position = i*OBSTACLE_SPACING + GetNewPositionOffset();
    }
    
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
    
    double delta = deltaTime / SECOND;
    
    // Move obstacles
    for (int i = 0; i < OBSTACLE_COUNT; i += 1) {
        state->obstacles[i].position -= OBSTACLE_SPEED * delta;
    }
    
    // Jumping
    state->timeJumped += delta;
    
    double playerY = Max(-(state->timeJumped*(state->timeJumped-2)), 0);
    
    if (playerY == 0) {
        if (kb_IsDown(kb_KeyEnter) || kb_IsDown(kb_KeyUp)) {
            state->timeJumped = 0;
        }
    }
    
    // Render game
    rend_RenderGame(state, playerY);
    
    // Don't go too fast
    while (timer_Get(TIMER) < SECOND/50);
    
    return false;
}
