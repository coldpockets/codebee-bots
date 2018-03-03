#include <time.h>
#include <cstdlib>
#include <iostream>
#include <queue>

#include "helper-package/bot.h"
#include "helper-package/constants.h"

using namespace std;

class HarvesterBot2 : public Bot {
public:
    HarvesterBot2(int minPollen) : Bot("HarvesterBot2"), minPollen(minPollen), spawnedHive(false) {
    }

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

        Position queenPos = queenBee->pos;

        if (!spawnedHive && queenBee->pollen >= HIVE_POLLEN_AMOUNT) {
            moves.push_back(Action(actionType::CREATE_HIVE));
            spawnedHive = true;
        } else if (hiveCells.size() > 0 && queenBee->pollen >= BEE_POLLEN_AMOUNT) {
            moves.push_back(Action(actionType::SPAWN, hiveCells.front()->getPosition(),
                                   queenBee->pollen / BEE_POLLEN_AMOUNT));
        }
        return moves;
    }

private:
    const int minPollen;
    bool spawnedHive;
};

int main() {
    srand(time(0));

    cout.sync_with_stdio(false);

    HarvesterBot2 harvesterBot2 = HarvesterBot2(1);

    harvesterBot2.run();
}
