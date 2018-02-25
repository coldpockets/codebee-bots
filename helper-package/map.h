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

private:
    static const int STATE_BEES_INDEX = 0;
    static const int STATE_QUEENS_INDEX = 1;
    static const int STATE_HIVES_INDEX = 2;

    static const int INSECT_X_INDEX = 0;
    static const int INSECT_Y_INDEX = 1;
    static const int INSECT_ID_INDEX = 2;
    static const int INSECT_BOT_ID_INDEX = 3;
    static const int INSECT_FACE_INDEX = 4;
    static const int INSECT_POLLEN_INDEX = 5;
    static const int INSECT_COUNT_INDEX = 6;

    static const int HIVE_X_INDEX = 0;
    static const int HIVE_Y_INDEX = 1;
    static const int HIVE_OWNER_ID_INDEX = 2;
};

#endif //CODEBEE_GAME_MAP_H
