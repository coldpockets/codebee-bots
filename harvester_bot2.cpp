#include <time.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <queue>

#include "helper-package/bot.h"
#include "helper-package/constants.h"

using namespace std;

fstream fs;

class HarvesterBot2 : public Bot {
public:
    HarvesterBot2(int minPollen) : Bot("HarvesterBot2"), minPollen(minPollen), spawnedHive(false) {
//        queenMoves.push(move::UP);
//        queenMoves.push(move::UP);
//        queenMoves.push(move::LEFT);
//        queenMoves.push(move::DOWN);
    }

protected:
    vector<Action> getMoves(int id, const Map& curMap) {
        vector<Action> moves;

        // Move to closest flower or queen bee if bee has pollen.
        for (auto beeCell = beeCells.begin(); beeCell != beeCells.end(); ++beeCell) {
            shared_ptr<Bee> bee = (*beeCell)->getBee();
            Position pos = (*beeCell)->getPosition();
            if (bee->pollen >= minPollen * bee->count) {
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

        Position queenPos = queenBee->pos;

        if (!spawnedHive) {
            if (queenBee->pollen >= HIVE_POLLEN_AMOUNT) {
                moves.push_back(Action(actionType::CREATE_HIVE));
                queenMoves.push(move::UP);
                queenMoves.push(move::LEFT);
                spawnedHive = true;
            }
        } else {
            if (hiveCells.size() > 0 && queenBee->pollen >= BEE_POLLEN_AMOUNT) {
                moves.push_back(Action(actionType::SPAWN, hiveCells.front()->getPosition(),
                                       queenBee->pollen / BEE_POLLEN_AMOUNT));
            }
            if (queenMoves.size() > 0) {
                moves.push_back(Action(actionType::MOVE_QUEEN, queenMoves.front(), rand() % 4));
                queenMoves.pop();
            } else {
                moves.push_back(Action(actionType::MOVE_QUEEN, move::STAY, rand() % 4));
            }
        }
        return moves;
    }

private:
    const int minPollen;
    bool spawnedHive;
    queue<Move> queenMoves;
};

int main() {
    srand(time(0));

    fs.open("harvester_bot2_log.txt", fstream::out | fstream::trunc);

    cout.sync_with_stdio(false);

    HarvesterBot2 harvesterBot2 = HarvesterBot2(1);

    harvesterBot2.run();

    fs.close();
}
