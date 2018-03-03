#include <algorithm>
#include <string>
#include <iostream>
#include <vector>

#include "bot.h"
#include "json.hpp"
#include "bee.h"
#include "constants.h"

using namespace std;
using namespace nlohmann;

Bot::Bot(string name) : name(name) {
    getInit(id, curMap);
    sendInit(name);
}

Bot::~Bot() {
    delete curMap;
}

void Bot::run() {
    vector<Action> moves;

    int turn = 1;
    while(true) {
        getTurn(*curMap);

        moves = getMoves(id, *curMap);

        sendTurn(moves);

        turn++;
    }
}

void Bot::getInit(int &id, Map* &initialMap) {
    id = std::stoi(getString());
    const std::string map = getString();
    initialMap = new Map(map);

    for (int y = 0; y < initialMap->height; ++y) {
        for (int x = 0; x < initialMap->width; ++x) {
            if (initialMap->map[y][x]->getPotency() > 0) {
                flowerCells.push_back(initialMap->map[y][x]);
            }
        }
    }
}

void Bot::sendInit(std::string name) {
    sendString(name);
}

void Bot::getTurn(Map& curMap) {
    string currState = getString();
    curMap.updateMap(currState);

    int botId = id;
    queenBee = *find_if(curMap.queenBees.begin(), curMap.queenBees.end(),
                       [&botId](shared_ptr<QueenBee> &queenBee) { return queenBee->botId == botId; });
    beeCells.clear();
    hiveCells.clear();
    totalBees = 0;

    // Get all important cells.
    for (int y = 0; y < curMap.height; ++y) {
        for (int x = 0; x < curMap.width; ++x) {
            shared_ptr<Bee> bee = curMap.map[y][x]->getBee();
            if (bee && bee->botId == id) {
                beeCells.push_back(curMap.map[y][x]);
                totalBees += bee->count;
            }

            if (curMap.map[y][x]->getOwnerId() == id) {
                hiveCells.push_back(curMap.map[y][x]);
            }
        }
    }
}

void Bot::sendTurn(std::vector<Action> moves) {
    string strMoves = serializeActions(moves);
    sendString(strMoves);
}

void Bot::sendString(string send) {
    cout << send << endl; // Flushes stream
}

string Bot::getString() {
    string response;
    getline(cin, response);
    return response;
}

std::string Bot::serializeActions(std::vector<Action> actions) {
    json stringified;
    for (auto &action : actions) {
        json jsonAction;
        jsonAction["type"] = action.type;
        switch (action.type) {
            case actionType::MOVE:
                jsonAction["x"] = action.pos.x;
                jsonAction["y"] = action.pos.y;
                // Fallthrough.
            case actionType::MOVE_QUEEN:
                jsonAction["move"] = action.moveOrSpawnAmount;
                jsonAction["face"] = action.face;
                break;
            case actionType::SPAWN:
                jsonAction["x"] = action.pos.x;
                jsonAction["y"] = action.pos.y;
                jsonAction["amount"] = action.moveOrSpawnAmount;
                jsonAction["face"] = action.face;
                break;
            case actionType::CREATE_HIVE:
                break;
        }
        stringified.push_back(jsonAction);
    }
    return stringified.dump();
}

bool Bot::isBesideHiveOrQueen(Position pos) {
    return isBeside(pos, queenBee->pos) || isBesideHive(pos);
}

bool Bot::isBesideHive(Position pos) {
    for (auto &hiveCell : hiveCells) {
        if (isBeside(pos, hiveCell->getPosition())) {
            return true;
        }
    }
    return false;
}

bool Bot::isBeside(Position pos, Position target) {
    return pos == target ||
        getBoundedPos(pos.x, pos.y + 1) == target ||
        getBoundedPos(pos.x, pos.y - 1) == target ||
        getBoundedPos(pos.x + 1, pos.y + 1) == target ||
        getBoundedPos(pos.x + 1, pos.y - 1) == target ||
        getBoundedPos(pos.x - 1, pos.y + 1) == target ||
        getBoundedPos(pos.x - 1, pos.y - 1) == target ||
        getBoundedPos(pos.x + 1, pos.y) == target ||
        getBoundedPos(pos.x - 1, pos.y) == target;
}

bool Bot::isBesideEnemy(Position pos) {
    return hasEnemy(pos) ||
           hasEnemy(getBoundedPos(pos.x + 1, pos.y)) ||
           hasEnemy(getBoundedPos(pos.x + 1, pos.y + 1)) ||
           hasEnemy(getBoundedPos(pos.x + 1, pos.y - 1)) ||
           hasEnemy(getBoundedPos(pos.x - 1, pos.y)) ||
           hasEnemy(getBoundedPos(pos.x - 1, pos.y + 1)) ||
           hasEnemy(getBoundedPos(pos.x - 1, pos.y - 1)) ||
           hasEnemy(getBoundedPos(pos.x, pos.y + 1)) ||
           hasEnemy(getBoundedPos(pos.x, pos.y - 1));
}

bool Bot::hasEnemy(Position pos) {
    int cellBotId = curMap->map[pos.y][pos.x]->getBeeBotId();
    return cellBotId != NEUTRAL_ID && cellBotId != id;
}

Position Bot::getBoundedPos(int x, int y) {
    return Position((curMap->width + x) % curMap->width, (curMap->height + y) % curMap->height);
}

Map::Path Bot::getMinPath(Position from, Position to) {
    return curMap->getPath(from, to);
}