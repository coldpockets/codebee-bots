#include "cell.h"
#include "constants.h"

Cell::Cell(Position pos, Flower p) : pos(pos), potency(p), ownerId(NEUTRAL_ID), bee(nullptr) { }

std::shared_ptr<Bee> Cell::getBee() const {
    return bee;
}

const int Cell::getBeeBotId() const {
    if (bee == nullptr) {
        return NEUTRAL_ID;
    } else {
        return bee->botId;
    }
}

const int Cell::getPotency() const {
    return potency;
}

const Position Cell::getPosition() const {
    return pos;
}

const int Cell::getOwnerId() const {
    return ownerId;
}

void Cell::clear() {
    ownerId = NEUTRAL_ID;
    bee = nullptr;
}

void Cell::setBee(std::shared_ptr <Bee> b) {
    bee = b;
}

void Cell::setOwnerId(int oId) {
    ownerId = oId;
}
