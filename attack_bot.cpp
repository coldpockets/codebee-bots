#include <time.h>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <algorithm>

#include "helper-package/bot.h"
#include "helper-package/constants.h"

using namespace std;

class AttackBot : public Bot {
public:
    AttackBot(int minPollen, int minBeesBeforeAttack, float percentage)
        : Bot("AttackBot"), minPollen(minPollen), minBees(minBeesBeforeAttack), percentage(percentage),
          spawnedHive(false) {
    }

protected:
    void init() {

    }

    vector<Action> getMoves(int id, const Map& curMap) {
        vector<Action> moves;
        vector<Cell*> enemyCells;

        for (int y = 0; y < curMap.height; ++y) {
            for (int x = 0; x < curMap.width; ++x) {
                shared_ptr<Bee> bee = curMap.map[y][x]->getBee();
                if (bee && bee->botId != id) {
                    enemyCells.push_back(curMap.map[y][x]);
                }
            }
        }

        if (totalBees >= minBees) {
	        // Calculate closest bee distances with enemy.
	        vector<Distances> orderedCells;
	        for (auto beeCell = beeCells.begin(); beeCell != beeCells.end(); ++beeCell) {
	            Position pos = (*beeCell)->getPosition();
	            Cell* firstEnemy = enemyCells[0];
	            Map::Path minPath = curMap.getPath(pos, (*firstEnemy).getPosition());
	            orderedCells.push_back(Distances(*beeCell, minPath));
	        }
            sort(orderedCells.begin(), orderedCells.end());

            int beesToAttack = (int) (totalBees * percentage);
	        for (auto group = orderedCells.begin(); group != orderedCells.end() && beesToAttack >= 0; ++group) {
	        	Distances d = *group;
                Position pos = d.cell->getPosition();
                int count = d.cell->getBee()->count;
                moves.push_back(Action(
                        actionType::MOVE,
                        pos,
                        d.path.move,
                        d.path.move - 1 /* Faces move direction. */
                ));
                beesToAttack -= count;
                // Remove this bee from cells, so we don't use it again.
                beeCells.erase(remove(beeCells.begin(), beeCells.end(), d.cell));
	        }
        }

        // Go to closest pollen or queen bee.
        for (auto beeCell = beeCells.begin(); beeCell != beeCells.end(); ++beeCell) {
            shared_ptr<Bee> bee = (*beeCell)->getBee();
            Position pos = (*beeCell)->getPosition();
            if (bee->pollen >= minPollen * bee->count) {
                Map::Path path = curMap.getPath(pos, queenBee->pos);
                moves.push_back(Action(actionType::MOVE, pos, path.move, path.move - 1 /* Faces move direction. */));
            } else {
                Cell* firstFlower = flowerCells[0];
                Map::Path minPath = curMap.getPath(pos, firstFlower->getPosition());
                for (auto flowerCell = flowerCells.begin() + 1; flowerCell != flowerCells.end(); ++flowerCell) {
                    Map::Path path = curMap.getPath(pos, (*flowerCell)->getPosition());
                    if (path.distance < minPath.distance) {
                        minPath = path;
                    }
                }
                moves.push_back(Action(actionType::MOVE, pos, minPath.move, minPath.move - 1 /* Faces move direction. */));
            }
        }

        if (!spawnedHive) {
            if (queenBee->pollen >= HIVE_POLLEN_AMOUNT) {
                moves.push_back(Action(actionType::CREATE_HIVE));
                spawnedHive = true;
            }
        }

        Position queenPos = queenBee->pos;
        if (!queenMoves.empty()) {
            moves.push_back(Action(actionType::MOVE_QUEEN, queenMoves.front(), rand() % 4));
            queenMoves.pop();
        } else {
            moves.push_back(Action(actionType::MOVE_QUEEN, move::STAY, rand() % 4));
        }

        if (hiveCells.size() > 0) {
            moves.push_back(Action(actionType::SPAWN, hiveCells.front()->getPosition(), queenBee->pollen / BEE_POLLEN_AMOUNT));
        }

        return moves;
    }

private:
    const int minPollen;
    const int minBees;
    const float percentage;
    queue<Move> queenMoves;
    bool spawnedHive;

    struct Distances {
        Cell* cell;
        Map::Path path;

        Distances() { }
        Distances(Cell* c, Map::Path p) : cell(c), path(p) { }
        bool operator< (const Distances &other) const {
            return path.distance < other.path.distance;
        }
    };
};

int main() {
    srand(time(0));

    cout.sync_with_stdio(false);

    AttackBot attackBot = AttackBot(1, 30, 0.20f);

    attackBot.run();

}
