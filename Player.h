#ifndef CHESS_PLAYER_H
#define CHESS_PLAYER_H

#include <string>

typedef struct Player {
    long id;
    std::string name;
    int score;
} Player;

#endif