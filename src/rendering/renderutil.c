#include <tice.h>
#include <graphx.h>
#include <stdlib.h>

// Prints a string that's horizontally centered, with the correct color and at the specified height.
void PrintHorizontalCentered(const char *str, int height, int color) {
    gfx_SetTextFGColor(color);
    int x = (LCD_WIDTH - gfx_GetStringWidth(str)) / 2;
    gfx_PrintStringXY(str, x, height);
}
