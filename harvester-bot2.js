const CONSTANTS = require('./helper-package-js/constants');
const { ACTION, FACE, FLOWER, MOVE } = require('./helper-package-js/constants');

const { Action } = require('./helper-package-js/action');
const { Bot } = require('./helper-package-js/bot');
const { Position } = require('./helper-package-js/position');

class HarvesterBot2 extends Bot {
    constructor() {
        super("HarvesterBot2");

        this.spawnedHive = false;
    }

    getActions() {
        const actions = [];

        for (let beeCell of this.beeCells) {
            const bee = beeCell.bee;
            const pos = beeCell.pos;
            if (bee.pollen >= bee.count) {
                // If the bees have pollen, move to closest queen or hive.
                if (!this.isBesideHiveOrQueen(pos)) {
                    let minPath = this.getMinPath(pos, this.queenBee.pos);
                    for (let hiveCell of this.hiveCells) {
                        let path = this.getMinPath(pos, hiveCell.pos);
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

        const queenPos = this.queenBee.pos;

        if (!this.spawnedHive && this.queenBee.pollen >= CONSTANTS.HIVE_POLLEN_AMOUNT) {
            actions.push(new Action({
                type: ACTION.CREATE_HIVE,
            }));
            this.spawnedHive = true;
        } else if (this.hiveCells.length > 0 && this.queenBee.pollen >= CONSTANTS.BEE_POLLEN_AMOUNT) {
            actions.push(new Action({
                type: ACTION.SPAWN,
                pos: this.hiveCells[0].pos,
                amount: this.queenBee.pollen / CONSTANTS.BEE_POLLEN_AMOUNT,
            }));
        }

        return actions;
    }
}

(async function main() {
    const harvesterBot2 = new HarvesterBot2();
    await harvesterBot2.run();
})();
