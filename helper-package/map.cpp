#include <string>

#include "map.h"
#include "json.hpp"
#include "bee.h"
#include "face.h"
#include "flower.h"

using namespace std;
using namespace nlohmann;

Map::Map(string pollenMap) {
    json pMap = json::parse(pollenMap);

    height = pMap.size();
    width = pMap.at(0).size();
    map = new Cell**[height];
    for (int y = 0; y < height; ++y) {
        json row = pMap.at(y);
        map[y] = new Cell*[width];
        for (int x = 0; x < width; ++x) {
            map[y][x] = new Cell(Position(x, y), row.at(x).get<Flower>());
        }
    }
}

Map::~Map() {
    if (map != nullptr) {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                delete map[y][x];
            }
            delete[] map[y];
        }
        delete[] map;
    }
}

void Map::updateMap(std::string newState) {
    json pState = json::parse(newState);
    json pBees = pState.at(STATE_BEES_INDEX);
    json pQueens = pState.at(STATE_QUEENS_INDEX);
    json pHives = pState.at(STATE_HIVES_INDEX);

    // Clear all bees and hives.
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            map[y][x]->clear();
        }
    }
    queenBees.clear();

    // Set bees.
    for (auto pBee : pBees) {
        shared_ptr<Bee> bee = make_shared<Bee>(
                pBee.at(INSECT_ID_INDEX).get<int>(),
                pBee.at(INSECT_BOT_ID_INDEX).get<int>(),
                Position(pBee.at(INSECT_X_INDEX).get<int>(),
                         pBee.at(INSECT_Y_INDEX).get<int>()),
                pBee.at(INSECT_FACE_INDEX).get<Face>(),
                pBee.at(INSECT_POLLEN_INDEX).get<int>(),
                pBee.at(INSECT_COUNT_INDEX).get<int>()
        );
        Position pos = bee->pos;
        map[pos.y][pos.x]->setBee(bee);
    }

    // Set queen bees.
    for (auto pQueen : pQueens) {
        queenBees.push_back(make_shared<QueenBee>(
                pQueen.at(INSECT_ID_INDEX).get<int>(),
                pQueen.at(INSECT_BOT_ID_INDEX).get<int>(),
                Position(pQueen.at(INSECT_X_INDEX).get<int>(),
                         pQueen.at(INSECT_Y_INDEX).get<int>()),
                pQueen.at(INSECT_FACE_INDEX).get<Face>(),
                pQueen.at(INSECT_POLLEN_INDEX).get<int>()
        ));
    }

    // Set hives.
    for (auto pHive : pHives) {
        map[pHive.at(HIVE_Y_INDEX).get<int>()][pHive.at(HIVE_X_INDEX).get<int>()]
                ->setOwnerId(pHive.at(HIVE_OWNER_ID_INDEX).get<int>());
    }
}


Map::Path Map::getPath(Position from, Position to) const {
    Path path;

    int downDist = (height + to.y - from.y) % height;
    int upDist = (height + from.y - to.y) % height;
    int rightDist = (width + to.x - from.x) % width;
    int leftDist = (width + from.x - to.x) % width;

    Move move1 = move::STAY;
    Move move2 = move::STAY;
    if (to.y != from.y) {
        if (downDist > upDist) {
            move1 = move::UP;
        } else {
            move1 = move::DOWN;
        }
        path.move = move1;
    }
    if (to.x != from.x) {
        if (rightDist > leftDist) {
            move2 = move::LEFT;
        } else {
            move2 = move::RIGHT;
        }
        path.move = move2;
    }

    if (move1 != move::STAY && move2 != move::STAY) {
        path.move = rand() % 2 == 0 ? move1 : move2;
    }

    path.distance = min(rightDist, leftDist) + min(downDist, upDist);

    return path;
}

