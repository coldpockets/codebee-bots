#include <time.h>
#include <cstdlib>
#include <iostream>

#include "helper-package/bot.h"

using namespace std;

class RandomBot : public Bot {
public:
    RandomBot() : Bot("RandomBot") { }

protected:
    void init() {

    }

    vector<Action> getMoves(int id, const Map& curMap) {
        vector<Action> moves;

        for (auto it = beeCells.begin(); it != beeCells.end(); ++it) {
            shared_ptr<Bee> bee = (*it)->getBee();
            int move = rand() % 5;
            moves.push_back(Action(
                actionType::MOVE,
                (*it)->getPosition(),
                move, move > 0 ? move - 1 /* Faces move direction. */ : bee->face
            ));
        }

        return moves;
    }
};

int main() {
    srand(time(0));

    cout.sync_with_stdio(false);

    RandomBot randomBot = RandomBot();

    randomBot.run();
}
