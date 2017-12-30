#include <time.h>
#include <cstdlib>
#include <iostream>
#include <fstream>

#include "helper-package/bot.h"

using namespace std;

fstream fs;

class RandomBot : public Bot {
public:
    RandomBot() : Bot("RandomBot") { }

protected:
    vector<Action> getMoves(int id, const Map& curMap) {
        vector<Action> moves;

        for (auto it = beeCells.begin(); it != beeCells.end(); ++it) {
            shared_ptr<Bee> bee = (*it)->getBee();
            int move = rand() % 5;
            moves.push_back(Action(actionType::MOVE, (*it)->getPosition(), move, move > 0 ? move - 1 : bee->face));
        }

        return moves;
    }
};

int main() {
    srand(time(0));

    fs.open("random_bot_log.txt", fstream::out | fstream::trunc);

    cout.sync_with_stdio(false);

    RandomBot randomBot = RandomBot();

    randomBot.run();

    fs.close();
}