//
// Created by William on 29/12/2017.
//

#ifndef CODEBEE_BOTS_QUEEN_BEE_H
#define CODEBEE_BOTS_QUEEN_BEE_H

#include "face.h"
#include "insect.h"
#include "position.h"

struct QueenBee : public Insect {
    QueenBee(int id, int botId, Position pos, Face face, int pollen = 0) :
            Insect(id, botId, pos, face, pollen) { }
};

#endif //CODEBEE_BOTS_QUEEN_BEE_H
