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
            // If the bees have pollen, move to queen.
            if (bee.pollen >= bee.count) {
                if (!this.isBesideHiveOrQueen(pos)) {
                    const path = this.getMinPath(pos, this.queenBee.pos);
                    actions.push(new Action({
                        type: ACTION.MOVE,
                        pos: pos,
                        move: path.move,
                        face: path.move - 1 /* Faces move direction. */,
                    }));
                }
            } else {
                // Move to closest flower to collect pollen.
                const firstFlower = this.flowerCells[0];
                let minPath = this.getMinPath(pos, firstFlower.pos);
                for (let i = 1; i < this.flowerCells.length; ++i) {
                    const path = this.getMinPath(pos, this.flowerCells[i].pos);
                    if (path.distance < minPath.distance) {
                        minPath = path;
                    }
                }
                actions.push(new Action({
                    type: ACTION.MOVE,
                    pos: pos,
                    move: minPath.move,
                    face: minPath.move - 1 /* Faces move direction. */,
                }));
            }
        }

        return actions;
    }
}

(async function main() {
    const harvesterBot = new HarvesterBot();
    await harvesterBot.run();
})();
