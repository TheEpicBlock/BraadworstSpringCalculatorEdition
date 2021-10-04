#include <tice.h>
#include <graphx.h>
#include <keypadc.h>

#include "game/gamelogic.h"
#include "rendering/renderer.h"
#include "gfx/generated/gfx.h"

const int OBSTACLE_SPACING = LCD_WIDTH * 0.9f;
const int OBSTACLE_MAX_OFFSET = 5;
const int OBSTACLE_SPEED = 80;

const int SECOND = 32768;
const int TIMER = 1;

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

float game_PlayerYFromTime(float time) {
    return Max(-(time*(time-3))/2.25, 0);
}

void game_InitState(struct GameState *state) {
    
    // Initialize obstacles
    float pos = GetNewPositionOffset();
    for (int i = 0; i < OBSTACLE_COUNT; i += 1) {
        state->obstacles[i].position = pos;
        pos += GetNewPositionOffset();
        state->obstacles[i].hasScored = false;
    }
    state->lastObstacle = OBSTACLE_COUNT-1;

    // Misc
    state->score = 0;
    state->status = PLAYING;
    state->timeJumped = 10;
    
    // Render game
    rend_RenderGame(state, 0);
    
    // Give some time for the user to let go of enter
    delay(500);
    
    // Initialize timers    
    timer_Set(TIMER, 0);
    timer_Enable(TIMER, TIMER_32K, TIMER_NOINT, TIMER_UP);
    
    kb_Scan();
    kb_Scan();
}

static void InGameTick(struct GameState *state) {
    // Calculate time
    int deltaTime = timer_Get(TIMER);
    timer_Set(TIMER, 0);
    
    float delta = (float)deltaTime / (float)SECOND;
    
    // Jump tick
    state->timeJumped += delta;
    
    float playerY = game_PlayerYFromTime(state->timeJumped);
    
    // Tick question
    if (state->questionInfo.timeToNext < 0) {
        state->status = QUESTIONED;
        timer_Disable(TIMER);
        rend_RenderQuestion(state);
        return;
    }
    
    // Obstacles
    for (int i = 0; i < OBSTACLE_COUNT; i += 1) {
        // Move
        state->obstacles[i].position -= OBSTACLE_SPEED * delta;
        if (state->obstacles[i].position < -genericobstacle_width) {
            state->obstacles[i].position = state->obstacles[state->lastObstacle].position + GetNewPositionOffset();
            state->lastObstacle = i;
            state->obstacles[i].hasScored = false;
        }
        
        // Score
        if (state->obstacles[i].position < PLAYER_X && !state->obstacles[i].hasScored) {
            state->score += 3;
            state->obstacles[i].hasScored = true;
        }
        
        // Kill
        if (gfx_CheckRectangleHotspot(PLAYER_X, playerY*PLAYER_MAX_JUMP, braadworst0_width, braadworst0_height, state->obstacles[i].position, 0, genericobstacle_width, genericobstacle_height)) {
            state->status = DEAD;
            timer_Disable(TIMER);
            rend_RenderDeath(state);
            delay(500); // Just in case the player presses enter right on this moments
            return;
        }
    }
    
    // Jump check
    if (playerY == 0) {
        if (kb_IsDown(kb_KeyEnter) || kb_IsDown(kb_KeyUp)) {
            state->timeJumped = 0;
        }
    }
    
    // Render game
    rend_RenderGame(state, playerY);
    
    // Don't go too fast
    while (timer_Get(TIMER) < SECOND/60);
}

static void TickQuestion(struct GameState *state) {

}

bool game_Tick(struct GameState *state) {
    kb_Scan();
    
    // Check exit
    if (kb_IsDown(kb_KeyDel) || kb_IsDown(kb_KeyClear)) {
        return true;
    }
    
    if (state->status == DEAD) {
        // Restart game
        if (os_GetCSC()) {
            game_InitState(state);
        }
    } else {
        if (state->status == PLAYING) {    
            InGameTick(state);
        } else {
            TickQuestion(state);
        }
    }
    
    return false;
}
