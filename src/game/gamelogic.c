#include <tice.h>
#include <graphx.h>
#include <keypadc.h>

#include "game/gamelogic.h"
#include "rendering/renderer.h"
#include "gfx/generated/gfx.h"

const int OBSTACLE_SPACING = LCD_WIDTH * 0.9f;
const int OBSTACLE_DEVIATION = LCD_WIDTH * 0.1f;
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

float GetNewPositionOffset(void) {
    return OBSTACLE_SPACING + randInt(-OBSTACLE_DEVIATION, OBSTACLE_DEVIATION);
}

float GetNewQuestionTime(void) {
    return (float)randInt(90, 120)/10.;
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
    
    // Question timer
    state->questionInfo.timeToNext = GetNewQuestionTime();
    
    // Misc
    state->score = 0;
    state->status = PLAYING;
    state->timeJumped = 10;
    state->playerAnim = 0;
    
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

static void OnDeath(struct GameState *state) {
    state->status = DEAD;
    timer_Disable(TIMER);
    rend_RenderDeath(state);
    delay(500); // Just in case the player presses enter right on this moments
}

static void InGameTick(struct GameState *state) {
    // Calculate time
    int deltaTime = timer_Get(TIMER);
    timer_Set(TIMER, 0);
    
    float delta = (float)deltaTime / (float)SECOND;
    
    // Player anim
    state->playerAnim += delta;
    if (state->playerAnim > 2) state->playerAnim = 0;
    
    // Jump tick
    state->timeJumped += delta;
    
    float playerY = game_PlayerYFromTime(state->timeJumped);
    
    // Tick question
    if (state->status == PLAYING && state->questionInfo.timeToNext < 0) {
        state->questionInfo.a = randInt(1,3);
        state->questionInfo.b = randInt(1,3);
        state->status = QUESTIONED;
        return;
    }
    state->questionInfo.timeToNext -= delta;
    
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
            OnDeath(state);
            return;
        }
    }
    
    // Jump check
    if (playerY == 0 && state->status == PLAYING) {
        if (kb_IsDown(kb_KeyEnter) || kb_IsDown(kb_KeyUp)) {
            state->timeJumped = 0;
        }
    }
    
    // Render game
    if (state->status == PLAYING) {
        rend_RenderGame(state, playerY);
    }
    
    // Don't go too fast
    while (timer_Get(TIMER) < SECOND/60);
}

static void Answer(struct GameState *state, int answer) {
    if (state->questionInfo.a + state->questionInfo.b == answer) {
        state->score += 8;
        
        state->questionInfo.timeToNext = GetNewQuestionTime();
        state->status = PLAYING;
    } else {
        OnDeath(state);
    }
}

static void TickQuestion(struct GameState *state) {
    if (state->status != QUESTIONED) {
        return; // Can happen if the player just died
    }
    
    rend_RenderQuestion(state);
    if (kb_IsDown(kb_Key0)) {
        Answer(state, 0);
    } else if (kb_IsDown(kb_Key1)) {
        Answer(state, 1);
    } else if (kb_IsDown(kb_Key2)) {
        Answer(state, 2);
    } else if (kb_IsDown(kb_Key3)) {
        Answer(state, 3);
    } else if (kb_IsDown(kb_Key4)) {
        Answer(state, 4);
    } else if (kb_IsDown(kb_Key5)) {
        Answer(state, 5);
    } else if (kb_IsDown(kb_Key6)) {
        Answer(state, 6);
    } else if (kb_IsDown(kb_Key7)) {
        Answer(state, 7);
    } else if (kb_IsDown(kb_Key8)) {
        Answer(state, 8);
    } else if (kb_IsDown(kb_Key9)) {
        Answer(state, 9);
    }
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
            InGameTick(state);
            TickQuestion(state);
        }
    }
    
    return false;
}
