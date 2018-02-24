#include <time.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <queue>
#include <set>
#include <algorithm>

#include "helper-package/bot.h"
#include "helper-package/constants.h"

using namespace std;

fstream fs;

class ExpandingBot : public Bot {
public:
    ExpandingBot(int minPollen, float ratio) : Bot("ExpandingBot"), minPollen(minPollen), ratio(ratio) { }

protected:
    vector<Action> getMoves(int id, const Map& curMap) {
        vector<Action> moves;

        // Move to closest flower or queen bee if bee has pollen.
        for (auto beeCell = beeCells.begin(); beeCell != beeCells.end(); ++beeCell) {
            shared_ptr<Bee> bee = (*beeCell)->getBee();
            Position pos = (*beeCell)->getPosition();
            if (bee->pollen >= minPollen * bee->count) {
                if (!isBesideHiveOrQueen(bee->pos)) {
                    Map::Path minPath = curMap.getPath(pos, queenBee->pos);
                    for (auto hiveCell = hiveCells.begin(); hiveCell != hiveCells.end(); ++hiveCell) {
                        Map::Path path = curMap.getPath(pos, (*hiveCell)->getPosition());
                        if (path.distance < minPath.distance) {
                            minPath = path;
                        }
                    }
                    moves.push_back(
                            Action(actionType::MOVE, pos, minPath.move, minPath.move - 1 /* Faces move direction. */));
                }
            } else {
                Cell* firstFlower = flowerCells[0];
                Map::Path minPath = curMap.getPath(pos, firstFlower->getPosition());
                for (auto flowerCell = flowerCells.begin() + 1; flowerCell != flowerCells.end(); ++flowerCell) {
                    Map::Path path = curMap.getPath(pos, (*flowerCell)->getPosition());
                    if (path.distance < minPath.distance) {
                        minPath = path;
                    }
                }
                moves.push_back(
                        Action(actionType::MOVE, pos, minPath.move, minPath.move - 1 /* Faces move direction. */));
            }
        }

        int pollenNotUsed = queenBee->pollen;
        Position queenPos = queenBee->pos;
        int posPotency = curMap.map[queenPos.y][queenPos.x]->getPotency();
        // Create hive on spot
        if (totalBees > ratio * hiveCells.size() && posPotency == 0) {
            pollenNotUsed -= HIVE_POLLEN_AMOUNT;
            moves.push_back(Action(actionType::CREATE_HIVE));
        }

        // If queen is on hive or flower, move to an open spot.
        if (posPotency > 0 || curMap.map[queenPos.y][queenPos.x]->getOwnerId() == id) {
            if (isEmpty(getBoundedPos(queenPos.x + 1, queenPos.y))) {
                moves.push_back(Action(actionType::MOVE_QUEEN, move::RIGHT));
            } else if (isEmpty(getBoundedPos(queenPos.x, queenPos.y + 1))) {
                moves.push_back(Action(actionType::MOVE_QUEEN, move::DOWN));
            } else if (isEmpty(getBoundedPos(queenPos.x - 1, queenPos.y))) {
                moves.push_back(Action(actionType::MOVE_QUEEN, move::LEFT));
            } else if (isEmpty(getBoundedPos(queenPos.x, queenPos.y - 1))) {
                moves.push_back(Action(actionType::MOVE_QUEEN, move::UP));
            } else {
                // Everywhere is blocked so just move to a random movement that is not STAY.
                moves.push_back(Action(actionType::MOVE_QUEEN, (rand() % 4) + 1));
            }
        }

        if (hiveCells.size() > 0) {
            int totalBeesPerHive = (pollenNotUsed / BEE_POLLEN_AMOUNT) / hiveCells.size();
            for (auto &hiveCell : hiveCells) {
                if (totalBeesPerHive > 100) {
                    moves.push_back(
                            Action(
                                    actionType::SPAWN,
                                    hiveCell->getPosition(),
                                    MAX_BEES
                            )
                    );
                } else {
                    moves.push_back(
                            Action(
                                    actionType::SPAWN,
                                    hiveCell->getPosition(),
                                    totalBeesPerHive
                            )
                    );
                }
            }
        }

        return moves;
    }

private:
    const int minPollen;
    const float ratio;

    // Detects when a spot has no hive or flower on it.
    bool isEmpty(Position pos) {
        return curMap->map[pos.y][pos.x]->getPotency() == 0 && curMap->map[pos.y][pos.x]->getOwnerId() == NEUTRAL_ID;
    }
};

int main() {
    srand(time(0));

    fs.open("expanding_bot_log.txt", fstream::out | fstream::trunc);

    cout.sync_with_stdio(false);

    ExpandingBot expandingBot = ExpandingBot(1, 30.0f);

    expandingBot.run();

    fs.close();
}
