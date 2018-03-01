const CONSTANTS = require('./helper-package-js/constants');
const { ACTION, FACE, FLOWER, MOVE } = require('./helper-package-js/constants');

const { Action } = require('./helper-package-js/action');
const { Bot } = require('./helper-package-js/bot');
const { Position } = require('./helper-package-js/position');

class RandomBot extends Bot {
    constructor() {
        super("RandomBot");
    }

    getActions() {
        const actions = [];

        for (let beeCell of this.beeCells) {
            const move = Math.floor(Math.random() * 5);
            actions.push(new Action({
                type: ACTION.MOVE,
                pos: beeCell.pos,
                move: move,
                face: move > 0 ? move - 1 : beeCell.bee.face,
            }));
        }

        return actions;
    }
}

(async function main() {
    const randomBot = new RandomBot();
    await randomBot.run();
})();
