const CONSTANTS = require('./helper-package-js/constants');
const { ACTION, FACE, FLOWER, MOVE } = require('./helper-package-js/constants');

const { Action } = require('./helper-package-js/action');
const { Bot } = require('./helper-package-js/bot');
const { Position } = require('./helper-package-js/position');

const BEES_TO_HIVE_RATIO = 30;

class ExpandingBot extends Bot {
    constructor() {
        super("ExpandingBot");
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
        let pollenNotUsed = this.queenBee.pollen;
        let posPotency = this.currMap.map[queenPos.y][queenPos.x].potency;

        // Create hive if the bees to hive ratio is higher than what we want.
        if (this.totalBees > BEES_TO_HIVE_RATIO * this.hiveCells.length && posPotency == FLOWER.NONE) {
            pollenNotUsed -= CONSTANTS.HIVE_POLLEN_AMOUNT;
            actions.push(new Action({
                type: ACTION.CREATE_HIVE,
            }));
        }

        // If queen is on a hive or flower, move to an open spot.
        if (!this.isEmpty(queenPos)) {
            if (this.isEmpty(this.getBoundedPos(queenPos.x + 1, queenPos.y))) {
                actions.push(new Action({
                    type: ACTION.MOVE_QUEEN,
                    move: MOVE.RIGHT,
                    face: FACE.RIGHT,
                }));
            } else if (this.isEmpty(this.getBoundedPos(queenPos.x, queenPos.y + 1))) {
                actions.push(new Action({
                    type: ACTION.MOVE_QUEEN,
                    move: MOVE.DOWN,
                    face: FACE.DOWN,
                }));
            } else if (this.isEmpty(this.getBoundedPos(queenPos.x - 1, queenPos.y))) {
                actions.push(new Action({
                    type: ACTION.MOVE_QUEEN,
                    move: MOVE.LEFT,
                    face: FACE.LEFT,
                }));
            } else if (this.isEmpty(this.getBoundedPos(queenPos.x, queenPos.y - 1))) {
                actions.push(new Action({
                    type: ACTION.MOVE_QUEEN,
                    move: MOVE.UP,
                    face: FACE.UP,
                }));
            } else {
                // Everywhere is blocked, so just move randomly.
                const move = Math.floor(Math.random() * 4) + 1;
                actions.push(new Action({
                    type: ACTION.MOVE_QUEEN,
                    move: move,
                    face: move - 1 /* Faces move direction. */,
                }));
            }
        }

        if (this.hiveCells.length > 0) {
            const totalBeesPerHive =
                Math.floor(Math.floor(pollenNotUsed / CONSTANTS.BEE_POLLEN_AMOUNT) / this.hiveCells.length);
            for (let hiveCell of this.hiveCells) {
                // Makes sure we don't overspawn bees.
                if (totalBeesPerHive > CONSTANTS.MAX_BEES) {
                    actions.push(new Action({
                        type: ACTION.SPAWN,
                        pos: hiveCell.pos,
                        amount: CONSTANTS.MAX_BEES,
                    }));
                } else {
                    actions.push(new Action({
                        type: ACTION.SPAWN,
                        pos: hiveCell.pos,
                        amount: totalBeesPerHive,
                    }));
                }
            }
        }

        return actions;
    }

    isEmpty(pos) {
        return this.currMap.map[pos.y][pos.x].potency == FLOWER.NONE &&
            this.currMap.map[pos.y][pos.x].ownerId == CONSTANTS.NEUTRAL_ID;
    }
}

(async function main() {
    const expandingBot = new ExpandingBot();
    await expandingBot.run();
})();
