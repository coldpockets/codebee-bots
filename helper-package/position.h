#ifndef CODEBEE_GAME_POSITION_H
#define CODEBEE_GAME_POSITION_H

struct Position {
    int x;
    int y;
    Position(int x = 0, int y = 0) : x(x), y(y) { }

    Position(const Position &p) : x(p.x), y(p.y) { }

    Position& operator= (const Position &p) {
        x = p.x;
        y = p.y;
        return *this;
    }

    bool operator==(const Position& other) {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Position& other) {
        return !(*this == other);
    }
};

#endif //CODEBEE_GAME_POSITION_H
