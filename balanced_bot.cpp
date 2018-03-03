#include <time.h>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <set>

#include "helper-package/bot.h"
#include "helper-package/constants.h"

using namespace std;

class BalancedBot : public Bot {
public:
    BalancedBot(int minPollen) : Bot("BalancedBot"), minPollen(minPollen) { }

protected:
    vector<Action> getMoves(int id, const Map& curMap) {
        vector<Action> moves;

        // Move to closest flower or queen bee if bee has pollen.
        for (auto beeCell = beeCells.begin(); beeCell != beeCells.end(); ++beeCell) {
            shared_ptr<Bee> bee = (*beeCell)->getBee();
            Position pos = (*beeCell)->getPosition();
            if (bee->count >= minPollen * bee->count) {
                Map::Path minPath = curMap.getPath(pos, queenBee->pos);
                for (auto hiveCell = hiveCells.begin(); hiveCell != hiveCells.end(); ++hiveCell) {
                    Map::Path path = curMap.getPath(pos, (*hiveCell)->getPosition());
                    if (path.distance < minPath.distance) {
                        minPath = path;
                    }
                }
                moves.push_back(
                        Action(actionType::MOVE, pos, minPath.move, minPath.move - 1 /* Faces move direction. */));
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

        // Find closest flower that is not beside a hive.
        int pollenNotUsed = queenBee->pollen;
        Position queenPos = queenBee->pos;
        set<Map::Path> flowerPaths;
        bool hold = false;
        bool shouldCreateHive = totalBees >= 10 * hiveCells.size()
            && curMap.map[queenPos.y][queenPos.x]->getPotency() == 0;
        for (auto flowerCell = flowerCells.begin(); flowerCell != flowerCells.end(); ++flowerCell) {
            if (!nearHive((*flowerCell)->getPosition())) {
                Position flowerPos = (*flowerCell)->getPosition();
                if (abs(queenPos.x - flowerPos.x) <= 1 && abs(queenPos.y - flowerPos.y) <= 1) {
                    if (shouldCreateHive) {
                        pollenNotUsed -= HIVE_POLLEN_AMOUNT;
                        moves.push_back(Action(actionType::CREATE_HIVE));
                    }
                    hold = true;
                    break;
                } else {
                    flowerPaths.insert(curMap.getPath(queenPos, flowerPos));
                }
            }
        }

        // Move towards closest flower, or if on a flower, then move to a vacant spot if possible.
        if (!hold) {
            for (auto path = flowerPaths.begin(); path != flowerPaths.end(); ++path) {
                if (path->distance == 0) {
                    int top = (queenPos.y + curMap.height - 1) % curMap.height;
                    int bot = (queenPos.y + 1) % curMap.height;
                    int left = (queenPos.x + curMap.width - 1) % curMap.width;
                    int right = (queenPos.x + 1) % curMap.width;
                    if (curMap.map[queenPos.y][left]->getPotency() == 0) {
                        moves.push_back(Action(actionType::MOVE, queenPos, move::LEFT));
                        break;
                    } else if (curMap.map[top][queenPos.x]->getPotency() == 0) {
                        moves.push_back(Action(actionType::MOVE, queenPos, move::UP));
                        break;
                    } else if (curMap.map[queenPos.y][right]->getPotency() == 0) {
                        moves.push_back(Action(actionType::MOVE, queenPos, move::RIGHT));
                        break;
                    } else if (curMap.map[bot][queenPos.x]->getPotency() == 0) {
                        moves.push_back(Action(actionType::MOVE, queenPos, move::DOWN));
                        break;
                    }
                } else {
                    moves.push_back(Action(actionType::MOVE, queenPos, flowerPaths.begin()->move));
                    break;
                }
            }
        }

        if (hiveCells.size() > 0) {
            moves.push_back(
                    Action(
                            actionType::SPAWN,
                            hiveCells[rand() % hiveCells.size()]->getPosition(),
                            pollenNotUsed / BEE_POLLEN_AMOUNT
                    )
            );
        }

        return moves;
    }

private:
    const int minPollen;

    bool nearHive(Position pos) {
        int top = (pos.y + curMap->height - 1) % curMap->height;
        int bot= (pos.y + 1) % curMap->height;
        int left = (pos.x + curMap->width - 1) % curMap->width;
        int right = (pos.x + 1) % curMap->width;

        return curMap->map[top][left]->getOwnerId() == id ||
               curMap->map[top][pos.x]->getOwnerId() == id ||
               curMap->map[top][right]->getOwnerId() == id ||
               curMap->map[pos.y][left]->getOwnerId() == id ||
               curMap->map[pos.y][pos.x]->getOwnerId() == id ||
               curMap->map[pos.y][right]->getOwnerId() == id ||
               curMap->map[bot][left]->getOwnerId() == id ||
               curMap->map[bot][pos.x]->getOwnerId() == id ||
               curMap->map[bot][right]->getOwnerId() == id;
    }
};

int main() {
    srand(time(0));

    cout.sync_with_stdio(false);

    BalancedBot balancedBot = BalancedBot(1);

    balancedBot.run();
}
