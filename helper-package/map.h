#ifndef CODEBEE_GAME_MAP_H
#define CODEBEE_GAME_MAP_H

#include <memory>
#include <string>
#include <vector>

#include "cell.h"
#include "position.h"
#include "move.h"
#include "queen_bee.h"

class Map {
public:
    struct Path {
        int distance;
        Move move;

        Path(int d = 0, Move m = move::UP) : distance(d), move(m) { }

        bool operator <(const Path &other) const {
            return distance < other.distance;
        }
    };


    Map(std::string pollenMap);
    ~Map();
    void updateMap(std::string newState);
    Path getPath(Position a, Position b) const;

    int height;
    int width;
    Cell*** map;
    std::vector<std::shared_ptr<QueenBee> > queenBees;
};

#endif //CODEBEE_GAME_MAP_H
