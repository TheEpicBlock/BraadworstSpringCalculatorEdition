#ifndef RENDERER_H
#define RENDERER_H

#include "game/gamelogic.h"

void rend_InitRendering(void);
void rend_EndRendering(void);
void rend_PrintMainMenu(void);
void rend_RenderGame(struct GameState *state, double playerHeight);

#endif /* RENDERER_H */

