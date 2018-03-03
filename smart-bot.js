const CONSTANTS = require('./helper-package-js/constants');
const { ACTION, FACE, FLOWER, MOVE } = require('./helper-package-js/constants');

const { Action } = require('./helper-package-js/action');
const { Bot } = require('./helper-package-js/bot');
const { Position } = require('./helper-package-js/position');

const BEES_TO_HIVE_RATIO = 30;

class SmartBot extends Bot {
    constructor() {
        super("SmartBot");
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

                    // Randomizes hive priorities if same distance.
                    this.shuffle(this.hiveCells);

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
                // Randomizes flower priorities if same distance.
                this.shuffle(this.flowerCells);

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

        // Check for an enemy at each position beside the queen.
        const topLeftEnemy = this.hasEnemy(this.getBoundedPos(queenPos.x - 1, queenPos.y - 1));
        const topEnemy = this.hasEnemy(this.getBoundedPos(queenPos.x, queenPos.y - 1));
        const topRightEnemy = this.hasEnemy(this.getBoundedPos(queenPos.x + 1, queenPos.y - 1));
        const leftEnemy = this.hasEnemy(this.getBoundedPos(queenPos.x - 1, queenPos.y));
        const rightEnemy = this.hasEnemy(this.getBoundedPos(queenPos.x + 1, queenPos.y));
        const botLeftEnemy = this.hasEnemy(this.getBoundedPos(queenPos.x - 1, queenPos.y + 1));
        const botEnemy = this.hasEnemy(this.getBoundedPos(queenPos.x, queenPos.y + 1));
        const botRightEnemy = this.hasEnemy(this.getBoundedPos(queenPos.x + 1, queenPos.y + 1));

        // Add moves that will move the queen away from potential threats.
        const availableMoves = [];
        if (!topEnemy && !botEnemy && !topLeftEnemy && !leftEnemy && !botLeftEnemy) {
            availableMoves.push(MOVE.LEFT);
        }
        if (!leftEnemy && !rightEnemy && !botLeftEnemy && !botEnemy && !botRightEnemy) {
            availableMoves.push(MOVE.DOWN);
        }
        if (!topEnemy && !botEnemy && !topRightEnemy && !rightEnemy && !botRightEnemy) {
            availableMoves.push(MOVE.RIGHT);
        }
        if (!leftEnemy && !rightEnemy && !topLeftEnemy && !topEnemy && !topRightEnemy) {
            availableMoves.push(MOVE.UP);
        }

        // If available moves is less than 4, then that means the queen is in threat of an enemy.
        // We should move it then.
        if (availableMoves.length < 4) {
            // Case when you won't escape an enemy no matter where you move.
            if (availableMoves.length == 0) {
                availableMoves.push(Math.floor(Math.random() % 4) + 1);
            }
            const move = availableMoves[Math.floor(Math.random() * availableMoves.length)];
            actions.push(new Action({
                type: ACTION.MOVE_QUEEN,
                move: move,
                face: move - 1,
            }));
        } else {
            // No threat, so proceed to expand normally.

            // Create hive if the bees to hive ratio is higher than what we want.
            if (this.totalBees > BEES_TO_HIVE_RATIO * this.hiveCells.length && posPotency == FLOWER.NONE) {
                pollenNotUsed -= CONSTANTS.HIVE_POLLEN_AMOUNT;
                actions.push(new Action({
                    type: ACTION.CREATE_HIVE,
                }));
            }

            // If queen is on a hive or flower, randomly move to an open spot.
            if (!this.isEmpty(queenPos)) {
                const movements = [MOVE.RIGHT, MOVE.DOWN, MOVE.LEFT, MOVE.UP];
                this.shuffle(movements);

                let moved = false;
                for (let i = 0; i < 4; ++i) {
                    if (movements[i] == MOVE.RIGHT &&
                        this.isEmpty(this.getBoundedPos(queenPos.x + 1, queenPos.y)) &&
                        !this.hasEnemy(this.getBoundedPos(queenPos.x + 1, queenPos.y))) {
                        moved = true;
                        actions.push(new Action({
                            type: ACTION.MOVE_QUEEN,
                            move: MOVE.RIGHT,
                            face: FACE.RIGHT,
                        }));
                        break;
                    } else if (movements[i] == MOVE.DOWN &&
                        this.isEmpty(this.getBoundedPos(queenPos.x, queenPos.y + 1)) &&
                        !this.hasEnemy(this.getBoundedPos(queenPos.x, queenPos.y + 1))) {
                        moved = true;
                        actions.push(new Action({
                            type: ACTION.MOVE_QUEEN,
                            move: MOVE.DOWN,
                            face: FACE.DOWN,
                        }));
                        break;
                    } else if (movements[i] == MOVE.LEFT &&
                        this.isEmpty(this.getBoundedPos(queenPos.x - 1, queenPos.y)) &&
                        !this.hasEnemy(this.getBoundedPos(queenPos.x - 1, queenPos.y))) {
                        moved = true;
                        actions.push(new Action({
                            type: ACTION.MOVE_QUEEN,
                            move: MOVE.LEFT,
                            face: FACE.LEFT,
                        }));
                        break;
                    } else if (movements[i] == MOVE.UP &&
                        this.isEmpty(this.getBoundedPos(queenPos.x, queenPos.y - 1)) &&
                        !this.hasEnemy(this.getBoundedPos(queenPos.x, queenPos.y - 1))) {
                        moved = true;
                        actions.push(new Action({
                            type: ACTION.MOVE_QUEEN,
                            move: MOVE.UP,
                            face: FACE.UP,
                        }));
                        break;
                    }
                }

                // Everywhere is blocked, so just move randomly.
                if (!moved) {
                    const move = Math.floor(Math.random() * 4) + 1;
                    actions.push(new Action({
                        type: ACTION.MOVE_QUEEN,
                        move: move,
                        face: move - 1,
                    }));
                }
            }
        }

        if (this.hiveCells.length > 0) {
            const totalBeesPerHive =
                Math.floor(Math.floor(pollenNotUsed / CONSTANTS.BEE_POLLEN_AMOUNT) / this.hiveCells.length);
            for (let hiveCell of this.hiveCells) {
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

    // Fisher-Yates shuffle algorithm.
    shuffle(array) {
        for (let i = array.length - 1; i > 0; --i) {
            const randomInd = Math.floor(Math.random() * (i + 1));
            const temp = array[i];
            array[i] = array[randomInd];
            array[randomInd] = temp;
        }
    }
}

(async function main() {
    const smartBot = new SmartBot();
    await smartBot.run();
})();
