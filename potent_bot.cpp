#include <time.h>
#include <cstdlib>
#include <iostream>
#include <fstream>

#include "helper-package/bot.h"
#include "helper-package/constants.h"

using namespace std;

fstream fs;

class PotentBot : public Bot {
public:
    PotentBot() : Bot("PotentBot") { }

protected:
    vector<Action> getMoves(int id, const Map& curMap) {
        vector<Action> moves;

        for (auto beeCell = beeCells.begin(); beeCell != beeCells.end(); ++beeCell) {
            shared_ptr<Bee> bee = (*beeCell)->getBee();
            Position pos = (*beeCell)->getPosition();
            if (bee->pollen >= bee->count) {
                Map::Path path = curMap.getPath(pos, queenBee->pos);
                moves.push_back(
                        Action(actionType::MOVE, pos, path.move, path.move - 1 /* Faces move direction. */));
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
        moves.push_back(Action(actionType::MOVE, queenPos, move::STAY, face::LEFT));

        moves.push_back(Action(actionType::SPAWN, queenPos, queenBee->pollen / BEE_POLLEN_AMOUNT));

        return moves;
    }
};

int main() {
    srand(time(0));

    fs.open("potent_bot_log.txt", fstream::out | fstream::trunc);

    cout.sync_with_stdio(false);

    PotentBot randomBot = PotentBot();

    randomBot.run();

    fs.close();
}
