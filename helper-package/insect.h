#ifndef CODEBEE_BOTS_INSECT_H
#define CODEBEE_BOTS_INSECT_H

#include "face.h"
#include "position.h"

struct Insect {
    Insect(int id, int botId, Position pos, Face face, int pollen) :
            id(id), botId(botId), pos(pos), face(face), pollen(pollen) { }

    const int id;
    const int botId;
    const Position pos;
    const Face face;
    const int pollen;
};

#endif //CODEBEE_BOTS_INSECT_H
