#include <tice.h>
#include <graphx.h>
#include <stdlib.h>

#include "gfx/generated/gfx.h"
#include "rendering/colors.h"
#include "rendering/renderutil.h"
#include "game/gamelogic.h"

void rend_InitRendering(void) {
    gfx_Begin();
    gfx_SetDrawBuffer();
    gfx_SetPalette(global_palette, sizeof_global_palette, 0); // Set global palette to the auto generated one
    gfx_SetTransparentColor(0); // Set the transparent color to be the background color
}

void rend_EndRendering(void) {
    gfx_End();
}

void rend_PrintMainMenu(void) {
    // Fill screen with background color
    gfx_ZeroScreen();
    
    gfx_SetTextScale(2,2);
    PrintHorizontalCentered("Braadworst Spring", 16, COLOR_RED);
    gfx_SetTextScale(1,1);
    PrintHorizontalCentered("TI-84 edition", LCD_HEIGHT-32, COLOR_WHITE);
    gfx_SwapDraw();
}

const int GROUND_LAYER_HEIGHT = 15;
const int GROUND_LAYER_TOTAL_HEIGHT = GROUND_LAYER_HEIGHT*4;
const int BASE = LCD_HEIGHT-GROUND_LAYER_TOTAL_HEIGHT;

void RenderFloor(void) {
    gfx_SetColor(COLOR_FLOOR_1);
    gfx_FillRectangle_NoClip(0, LCD_HEIGHT-GROUND_LAYER_HEIGHT*1, LCD_WIDTH, GROUND_LAYER_HEIGHT);
    gfx_SetColor(COLOR_FLOOR_2);
    gfx_FillRectangle_NoClip(0, LCD_HEIGHT-GROUND_LAYER_HEIGHT*2, LCD_WIDTH, GROUND_LAYER_HEIGHT);
    gfx_SetColor(COLOR_FLOOR_1);
    gfx_FillRectangle_NoClip(0, LCD_HEIGHT-GROUND_LAYER_HEIGHT*3, LCD_WIDTH, GROUND_LAYER_HEIGHT);
    gfx_SetColor(COLOR_FLOOR_2);
    gfx_FillRectangle_NoClip(0, LCD_HEIGHT-GROUND_LAYER_HEIGHT*4, LCD_WIDTH, GROUND_LAYER_HEIGHT);
}

void RenderPlayer(float playerHeight) {
    gfx_Sprite(braadworst0, PLAYER_X, BASE-playerHeight*PLAYER_MAX_JUMP-braadworst0_height);
}

void RenderObstacles(struct GameState *state) {
    for (int i = 0; i < OBSTACLE_COUNT; i += 1) {
        gfx_Sprite(genericobstacle, (int)(state->obstacles[i].position), BASE-genericobstacle_height);
    }
}

void rend_RenderGame(struct GameState *state, float playerHeight) {
    gfx_ZeroScreen();
    
    RenderFloor();
    RenderPlayer(playerHeight);
    
    RenderObstacles(state);
    
    // Show score
    PrintUInt(0, 0, COLOR_WHITE, state->score);
    
    // Swap frame
    gfx_SwapDraw();
}

void rend_RenderDeath(struct GameState *state) {
    gfx_ZeroScreen();
    
    RenderFloor();
    float playerHeight = game_PlayerYFromTime(state->timeJumped);
    RenderPlayer(playerHeight);
    
    // TEXT
    gfx_SetTextFGColor(COLOR_BLACK);
    gfx_SetTextBGColor(COLOR_WHITE);
    gfx_SetTextScale(2,2);
    const char *gameover = "Game over";
    int textwidth = gfx_GetStringWidth(gameover);
    gfx_PrintStringXY(gameover, (LCD_WIDTH - textwidth) / 2, (LCD_HEIGHT - 16) / 2);
    
    gfx_SetTextScale(1,1);
    const char *score = "Score: ";
    gfx_SetTextXY((LCD_WIDTH - textwidth) / 2, gfx_GetTextY()+16);
    gfx_PrintString(score);
    gfx_PrintUInt(state->score, 0);
    
    gfx_SetTextBGColor(255);
    
    // Swap frame
    gfx_SwapDraw();
}
