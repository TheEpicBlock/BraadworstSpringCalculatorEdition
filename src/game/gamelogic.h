#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <stdbool.h> 

#define OBSTACLE_COUNT 2

const int PLAYER_MAX_JUMP = LCD_HEIGHT*0.5;
const int PLAYER_X = 10;

enum GameStatus {
    PLAYING,
    DEAD
};

struct Obstacle {
    float               position;
    bool                hasScored;
};


struct GameState {
    enum GameStatus     status;
    unsigned int        score;
    float               timeJumped;
    struct Obstacle     obstacles[OBSTACLE_COUNT];
    int                 lastObstacle;
    bool                isDead;
};

float game_PlayerYFromTime(float time);
void game_InitState(struct GameState *state);
bool game_Tick(struct GameState *state);

#endif /* GAMELOGIC_H */

