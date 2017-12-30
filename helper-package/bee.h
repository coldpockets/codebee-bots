//
// Created by William on 29/12/2017.
//

#ifndef CODEBEE_BOTS_BEE_H
#define CODEBEE_BOTS_BEE_H

#include "face.h"
#include "insect.h"
#include "position.h"

struct Bee : public Insect {
    Bee(int id, int botId, Position pos, Face face, int pollen = 0, int count = 1) :
            Insect(id, botId, pos, face, pollen), count(count) { }

    const int count;
};

#endif //CODEBEE_BOTS_BEE_H
