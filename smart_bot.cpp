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

class SmartBot : public Bot {
public:
    SmartBot(int minPollen, float ratio) : Bot("SmartBot"), minPollen(minPollen), ratio(ratio) { }

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
                    random_shuffle(hiveCells.begin(), hiveCells.end());
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
                random_shuffle(flowerCells.begin(), flowerCells.end());
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

        // Check if queen is beside an enemy, and move away from enemies if so.
        set<Move> availableMoves = { move::RIGHT, move::DOWN, move::LEFT, move::UP };
        if (hasEnemy(getBoundedPos(queenPos.x + 1, queenPos.y))) {
            availableMoves.erase(move::RIGHT);
            availableMoves.erase(move::DOWN);
            availableMoves.erase(move::UP);
        }
        if (hasEnemy(getBoundedPos(queenPos.x + 1, queenPos.y + 1))) {
            availableMoves.erase(move::RIGHT);
            availableMoves.erase(move::DOWN);
        }
        if (hasEnemy(getBoundedPos(queenPos.x + 1, queenPos.y - 1))) {
            availableMoves.erase(move::RIGHT);
            availableMoves.erase(move::UP);
        }
        if (hasEnemy(getBoundedPos(queenPos.x - 1, queenPos.y))) {
            availableMoves.erase(move::LEFT);
            availableMoves.erase(move::DOWN);
            availableMoves.erase(move::UP);
        }
        if (hasEnemy(getBoundedPos(queenPos.x - 1, queenPos.y + 1))) {
            availableMoves.erase(move::LEFT);
            availableMoves.erase(move::DOWN);
        }
        if (hasEnemy(getBoundedPos(queenPos.x - 1, queenPos.y - 1))) {
            availableMoves.erase(move::LEFT);
            availableMoves.erase(move::UP);
        }
        if (hasEnemy(getBoundedPos(queenPos.x, queenPos.y + 1))) {
            availableMoves.erase(move::LEFT);
            availableMoves.erase(move::RIGHT);
            availableMoves.erase(move::UP);
        }
        if (hasEnemy(getBoundedPos(queenPos.x, queenPos.y - 1))) {
            availableMoves.erase(move::LEFT);
            availableMoves.erase(move::RIGHT);
            availableMoves.erase(move::DOWN);
        }
        if (availableMoves.size() < 4) {
            // Case when you won't escape an enemy no matter where you move.
            if (availableMoves.size() == 0) {
                moves.push_back(Action(actionType::MOVE_QUEEN, (rand() % 4) + 1));
            } else {
                vector<Move> shuffledMoves(availableMoves.begin(), availableMoves.end());
                random_shuffle(shuffledMoves.begin(), shuffledMoves.end());
                moves.push_back(Action(actionType::MOVE_QUEEN, *shuffledMoves.begin()));
            }
        } else {
            int posPotency = curMap.map[queenPos.y][queenPos.x]->getPotency();
            // Create hive on spot
            if (totalBees > ratio * hiveCells.size() && posPotency == 0) {
                pollenNotUsed -= HIVE_POLLEN_AMOUNT;
                moves.push_back(Action(actionType::CREATE_HIVE));
            }

            // If queen is on hive or flower, move to an open spot.
            if (posPotency > 0 || curMap.map[queenPos.y][queenPos.x]->getOwnerId() == id) {
                Move movements[] = {move::RIGHT, move::DOWN, move::LEFT, move::UP};
                random_shuffle(begin(movements), end(movements));

                bool moved = false;
                for (int i = 0; i < 4; ++i) {
                    if (movements[i] == move::RIGHT &&
                        isEmpty(getBoundedPos(queenPos.x + 1, queenPos.y)) &&
                        !hasEnemy(getBoundedPos(queenPos.x + 1, queenPos.y))) {
                        moved = true;
                        moves.push_back(Action(actionType::MOVE_QUEEN, move::RIGHT));
                        break;
                    } else if (movements[i] == move::DOWN &&
                               isEmpty(getBoundedPos(queenPos.x, queenPos.y + 1)) &&
                               !hasEnemy(getBoundedPos(queenPos.x, queenPos.y + 1))) {
                        moved = true;
                        moves.push_back(Action(actionType::MOVE_QUEEN, move::DOWN));
                        break;
                    } else if (movements[i] == move::LEFT &&
                               isEmpty(getBoundedPos(queenPos.x - 1, queenPos.y)) &&
                               !hasEnemy(getBoundedPos(queenPos.x - 1, queenPos.y))) {
                        moved = true;
                        moves.push_back(Action(actionType::MOVE_QUEEN, move::LEFT));
                        break;
                    } else if (movements[i] == move::UP &&
                               isEmpty(getBoundedPos(queenPos.x, queenPos.y - 1)) &&
                               !hasEnemy(getBoundedPos(queenPos.x, queenPos.y - 1))) {
                        moved = true;
                        moves.push_back(Action(actionType::MOVE_QUEEN, move::UP));
                        break;
                    }
                }
                // Everywhere is blocked so just move to a random movement that is not STAY.
                if (!moved) {
                    moves.push_back(Action(actionType::MOVE_QUEEN, (rand() % 4) + 1));
                }
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

    fs.open("smart_bot_log.txt", fstream::out | fstream::trunc);

    cout.sync_with_stdio(false);

    SmartBot smartBot = SmartBot(1, 30.0f);

    smartBot.run();

    fs.close();
}
