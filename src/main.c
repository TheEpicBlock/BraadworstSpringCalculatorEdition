#include <tice.h>
#include <graphx.h>
#include <stdlib.h>

#include "gfx/gfx.h"
#include "colors.h"

void PrintCentered(const char *str, int y, int c);
void PrintMainMenu(void);

// Main function, called first
int main(void)
{
    // Initialize graphics drawing
    gfx_Begin();
    gfx_SetPalette(global_palette, sizeof_global_palette, 0); // Set global palette to the auto generated one
    gfx_SetTransparentColor(0); // Set the transparent color to be the background color
    
    // Print the main menu
    PrintMainMenu();
    
    // Wait for keypress
    while (!os_GetCSC());

    // End graphics drawing
    gfx_End();

    /* Return 0 for success */
    return 0;
}

void PrintMainMenu(void) {
    // Fill screen with background color
    gfx_ZeroScreen();
    
    PrintCentered("Braadworst Spring", 16, color_red);
    PrintCentered("TI-84 edition", LCD_HEIGHT-32, color_white);
}

void PrintCentered(const char *str, int y, int c)
{
    gfx_SetTextFGColor(c);
    gfx_PrintStringXY(str,
                      (LCD_WIDTH - gfx_GetStringWidth(str)) / 2,
                      y);
}
