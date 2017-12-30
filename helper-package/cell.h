#ifndef CODEBEE_BOTS_CELL_H
#define CODEBEE_BOTS_CELL_H

#include <memory>

#include "bee.h"
#include "flower.h"
#include "position.h"

class Cell {
public:
    Cell(Position pos, Flower p);

    std::shared_ptr<Bee> getBee() const;
    const int getPotency() const;
    const Position getPosition() const;
    const int getOwnerId() const;

    void clear();

    void setBee(std::shared_ptr<Bee> b);
    void setOwnerId(int oId);
private:
    const Position pos;
    const int potency;
    int ownerId;
    std::shared_ptr<Bee> bee;
};

#endif //CODEBEE_BOTS_CELL_H
