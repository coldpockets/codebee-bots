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
            json cell = row.at(x);
            map[y][x] = new Cell(Position(x, y), cell.at("p").get<Flower>());
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
    json pBees = pState.at("b");
    json pQueens = pState.at("q");
    json pHives = pState.at("h");

    // Clear all bees and hives.
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            map[y][x]->clear();
        }
    }
    queenBees.clear();

    // Set bees.
    for (auto pInsect : pBees) {
        shared_ptr<Bee> bee = make_shared<Bee>(
                pInsect.at("i").get<int>(),
                pInsect.at("b").get<int>(),
                Position(pInsect.at("x").get<int>(),
                         pInsect.at("y").get<int>()),
                pInsect.at("f").get<Face>(),
                pInsect.at("p").get<int>(),
                pInsect.at("c").get<int>()
        );
        Position pos = bee->pos;
        map[pos.y][pos.x]->setBee(bee);
    }

    // Set queen bees.
    for (auto pQueen : pQueens) {
        queenBees.push_back(make_shared<QueenBee>(
                pQueen.at("i").get<int>(),
                pQueen.at("b").get<int>(),
                Position(pQueen.at("x").get<int>(),
                         pQueen.at("y").get<int>()),
                pQueen.at("f").get<Face>(),
                pQueen.at("p").get<int>()
        ));
    }

    // Set hives.
    for (auto pHive : pHives) {
        map[pHive.at("y").get<int>()][pHive.at("x").get<int>()]->setOwnerId(pHive.at("o").get<int>());
    }
}


Map::Path Map::getPath(Position from, Position to) const {
    Path path;

    int downDist = (height + to.y - from.y) % height;
    int upDist = (height + from.y - to.y) % height;
    int rightDist = (width + to.x - from.x) % width;
    int leftDist = (width + from.x - to.x) % width;

    if (to.y != from.y) {
        if (downDist > upDist) {
            path.move = move::UP;
        } else {
            path.move = move::DOWN;
        }
    } else if (to.x != from.x) {
        if (rightDist > leftDist) {
            path.move = move::LEFT;
        } else {
            path.move = move::RIGHT;
        }
    }

    path.distance = min(rightDist, leftDist) + min(downDist, upDist);

    return path;
}

