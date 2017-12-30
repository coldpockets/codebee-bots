#ifndef CODEBEE_GAME_ACTION_H
#define CODEBEE_GAME_ACTION_H

#include "face.h"
#include "move.h"
#include "position.h"

typedef int ActionType;

namespace actionType {
    static const int MOVE = 0;
    static const int SPAWN = 1;
    static const int CREATE_HIVE = 2;
    static const int MOVE_QUEEN = 3;
}

struct Action {
    const ActionType type;
    Position pos;
    const int moveOrSpawnAmount;
    const Face face;

    Action(ActionType type, Position pos, int moveOrSpawnAmount = 0, Face face = face::UP) :
        type(type), pos(pos), moveOrSpawnAmount(moveOrSpawnAmount), face(face) { }

    Action(ActionType type, int moveOrSpawnAmount = 0, Face face = face::UP) :
        type(type), moveOrSpawnAmount(moveOrSpawnAmount), face(face) { }
};
#endif //CODEBEE_GAME_ACTION_H
