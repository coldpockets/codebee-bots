#include <time.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <queue>

#include "helper-package/bot.h"
#include "helper-package/constants.h"

using namespace std;

fstream fs;

class HarvesterBot : public Bot {
public:
    HarvesterBot(int minPollen) : Bot("HarvesterBot"), minPollen(minPollen) { }

protected:
    vector<Action> getMoves(int id, const Map& curMap) {
        vector<Action> moves;

        // Move to closest flower or queen bee if bee has pollen.
        for (auto beeCell = beeCells.begin(); beeCell != beeCells.end(); ++beeCell) {
            shared_ptr<Bee> bee = (*beeCell)->getBee();
            Position pos = (*beeCell)->getPosition();
            if (bee->pollen >= minPollen * bee->count) {
                if (!isBesideHiveOrQueen(bee->pos)) {
                    Map::Path path = curMap.getPath(pos, queenBee->pos);
                    moves.push_back(
                            Action(actionType::MOVE, pos, path.move, path.move - 1 /* Faces move direction. */));
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

        return moves;
    }

private:
    const int minPollen;
};

int main() {
    srand(time(0));

    fs.open("harvester_bot_log.txt", fstream::out | fstream::trunc);

    cout.sync_with_stdio(false);

    HarvesterBot harvesterBot = HarvesterBot(1);

    harvesterBot.run();

    fs.close();
}
