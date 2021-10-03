#include <tice.h>
#include <graphx.h>
#include <stdlib.h>

#include "rendering/renderer.h"
#include "game/gamelogic.h"

// Main function, called first
int main(void)
{
    // Initialize graphics drawing
    rend_InitRendering();
    
    // Print the main menu and wait for a keypress
    rend_PrintMainMenu();
    while (!os_GetCSC());

    struct GameState gameState;
    game_InitState(&gameState);
    
    while (!game_Tick(&gameState));

    // End graphics drawing
    rend_EndRendering();
    return 0;
}
