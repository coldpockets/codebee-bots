#include <time.h>
#include <cstdlib>
#include <iostream>

#include "helper-package/bot.h"

using namespace std;

class MyBot : public Bot {
public:
    MyBot() : Bot("MyBot") { }

protected:
    vector<Action> getMoves(int id, const Map& curMap) {
        vector<Action> moves;

        /***************************************************************
        	REPLACE CODE HERE
        ****************************************************************/

        for (auto it = beeCells.begin(); it != beeCells.end(); ++it) {
            shared_ptr<Bee> bee = (*it)->getBee();
            int move = rand() % 5;
            moves.push_back(Action(actionType::MOVE, (*it)->getPosition(), move, move > 0 ? move - 1 : bee->face));
        }

        /*************************************************************/

        return moves;
    }
};

/* DO NOT TOUCH - unless you know what you're doing ;) */
int main() {
    srand(time(0));

    cout.sync_with_stdio(false);

    MyBot myBot = MyBot();

    myBot.run();
}