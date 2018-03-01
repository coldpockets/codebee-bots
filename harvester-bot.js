const CONSTANTS = require('./helper-package-js/constants');
const { ACTION, FACE, FLOWER, MOVE } = require('./helper-package-js/constants');

const { Action } = require('./helper-package-js/action');
const { Bot } = require('./helper-package-js/bot');
const { Position } = require('./helper-package-js/position');

class HarvesterBot extends Bot {
    constructor() {
        super("HarvesterBot");
    }

    getActions() {
        const actions = [];

        for (let beeCell of this.beeCells) {
            const bee = beeCell.bee;
            const pos = beeCell.pos;
            if (bee.pollen >= bee.count) {
                if (!this.isBesideHiveOrQueen(pos)) {
                    const path = this.getMinPath(pos, this.queenBee.pos);
                    actions.push(new Action(ACTION.MOVE, path.move, path.move - 1 /* Faces move direction. */, pos));
                }
            } else {
                const firstFlower = this.flowerCells[0];
                let minPath = this.getMinPath(pos, firstFlower.pos);
                for (let i = 1; i < this.flowerCells.length; ++i) {
                    const path = this.getMinPath(pos, this.flowerCells[i].pos);
                    if (path.distance < minPath.distance) {
                        minPath = path;
                    }
                }
                actions.push(new Action(ACTION.MOVE, minPath.move, minPath.move - 1 /* Faces move direction. */, pos));
            }
        }

        return actions;
    }
}

(async function main() {
    const harvesterBot = new HarvesterBot();
    await harvesterBot.run();
})();
