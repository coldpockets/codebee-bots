#include <time.h>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <set>
#include <algorithm>

#include "helper-package/bot.h"
#include "helper-package/constants.h"

using namespace std;

class ExpandingBotOld : public Bot {
public:
    ExpandingBotOld(int minPollen, float ratio) : Bot("ExpandingBotOld"), minPollen(minPollen), ratio(ratio) { }

protected:
    void init() {

    }

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

        // Find closest flower that is not beside a hive if bees:hives ratio is greater than specified.
        if (totalBees > ratio * hiveCells.size()) {
            Position queenPos = queenBee->pos;
            set<Map::Path> flowerPaths;
            bool creatingHive = false;
            bool canCreateHive = curMap.map[queenPos.y][queenPos.x]->getPotency() == 0;
            for (auto flowerCell = flowerCells.begin(); flowerCell != flowerCells.end(); ++flowerCell) {
                if (!isBesideHive((*flowerCell)->getPosition())) {
                    Position flowerPos = (*flowerCell)->getPosition();
                    bool beside = isBeside(queenPos, flowerPos);
                    if (canCreateHive && beside) {
                        pollenNotUsed -= HIVE_POLLEN_AMOUNT;
                        moves.push_back(Action(actionType::CREATE_HIVE));
                        creatingHive = true;
                        break;
                    } else if (!canCreateHive || !beside) {
                        flowerPaths.insert(curMap.getPath(queenPos, flowerPos));
                    }
                }
            }

            // Move towards closest flower, or if on a flower, then move to a vacant spot if possible.
            if (!creatingHive) {
                for (auto path = flowerPaths.begin(); path != flowerPaths.end(); ++path) {
                    if (path->distance == 0) {
                        int top = (queenPos.y + curMap.height - 1) % curMap.height;
                        int bot = (queenPos.y + 1) % curMap.height;
                        int left = (queenPos.x + curMap.width - 1) % curMap.width;
                        int right = (queenPos.x + 1) % curMap.width;
                        if (curMap.map[queenPos.y][left]->getPotency() == 0) {
                            moves.push_back(Action(actionType::MOVE_QUEEN, move::LEFT));
                            break;
                        } else if (curMap.map[top][queenPos.x]->getPotency() == 0) {
                            moves.push_back(Action(actionType::MOVE_QUEEN, move::UP));
                            break;
                        } else if (curMap.map[queenPos.y][right]->getPotency() == 0) {
                            moves.push_back(Action(actionType::MOVE_QUEEN, move::RIGHT));
                            break;
                        } else if (curMap.map[bot][queenPos.x]->getPotency() == 0) {
                            moves.push_back(Action(actionType::MOVE_QUEEN, move::DOWN));
                            break;
                        }
                    } else {
                        moves.push_back(Action(actionType::MOVE_QUEEN, path->move));
                        break;
                    }
                }
            }
        }

        if (hiveCells.size() > 0) {
            moves.push_back(
                    Action(
                            actionType::SPAWN,
                            hiveCells[(rand() % hiveCells.size())]->getPosition(),
                            pollenNotUsed / BEE_POLLEN_AMOUNT
                    )
            );
        }

        return moves;
    }

private:
    const int minPollen;
    const float ratio;
};

int main() {
    srand(time(0));

    cout.sync_with_stdio(false);

    ExpandingBotOld expandingBotOld = ExpandingBotOld(1, 30.0f);

    expandingBotOld.run();
}
