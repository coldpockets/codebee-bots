#ifndef CODEBEE_GAME_BOT_H
#define CODEBEE_GAME_BOT_H

#include <string>
#include <vector>

#include "map.h"
#include "action.h"
#include "queen_bee.h"

class Bot {
public:
    Bot(std::string name);
    ~Bot();
    void run();

protected:
    std::string getString();
    void sendString(std::string send);

    void getInit(int &id, Map* &initialMap);
    void sendInit(std::string name);
    void getTurn(Map& curMap);
    void sendTurn(std::vector<Action> moves);

    bool isBesideHiveOrQueen(Position pos);
    bool isBeside(Position pos, Position target);

    Position getBoundedPos(int x, int y);

    virtual std::vector<Action> getMoves(int id, const Map& curMap) = 0;

    int id;
    std::string name;
    Map* curMap;
    std::vector<Cell*> flowerCells;
    std::vector<Cell*> beeCells;
    std::vector<Cell*> hiveCells;
    std::shared_ptr<QueenBee> queenBee;
    int totalBees;

private:
    std::string serializeActions(std::vector<Action> actions);
};

#endif //CODEBEE_GAME_BOT_H
