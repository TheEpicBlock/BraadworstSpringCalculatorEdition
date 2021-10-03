#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <stdbool.h> 

#define OBSTACLE_COUNT 2

enum GameStatus {
    PLAYING,
    DEAD
};

struct Obstacle {
    int         position;
};


struct GameState {
    enum GameStatus     status;
    int                 score;
    double              timeJumped;
    struct Obstacle     obstacles[OBSTACLE_COUNT];
};
void game_InitState(struct GameState *state);
bool game_Tick(struct GameState *state);

#endif /* GAMELOGIC_H */

