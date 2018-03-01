const { ACTION, NEUTRAL_ID } = require('./constants');
const { StdInReader } = require('./std-in-reader');

const { Cell } = require('./cell');
const { Map } = require('./map');
const { Position } = require('./position');

exports.Bot = class Bot {
    constructor(name) {
        this.name = name;
    }

    async run() {
        await this.getInit();
        this.sendInit(this.name);

        let turn = 1;
        while (true) {

            await this.getTurn();

            const actions = this.getActions();

            this.sendTurn(actions);

            turn++;
        }
    }

    async getInit() {
        this.StdInReader = new StdInReader();
        this.id = parseInt(await this.getString());
        const map = await this.getString();

        const pMap = JSON.parse(map);

        this.currMap = new Map(map);

        this.flowerCells = [];
        this.beeCells = [];
        this.hiveCells = [];

        for (let row of this.currMap.map) {
            for (let item of row) {
                if (item.potency > 0) {
                    this.flowerCells.push(item);
                }
            }
        }
    }

    sendInit(name) {
        this.sendString(name);
    }

    async getTurn() {
        const currState = await this.getString();
        this.currMap.updateMap(currState);

        this.queenBee = this.currMap.queenBees.find(bee => bee.botId == this.id);
        this.beeCells.length = 0;
        this.hiveCells.length = 0;
        this.totalBees = 0;

        // Get all important cells.
        for (let row of this.currMap.map) {
            for (let item of row) {
                const bee = item.bee;
                if (bee && bee.botId == this.id) {
                    this.beeCells.push(item);
                    this.totalBees += bee.count;
                }

                if (item.ownerId == this.id) {
                    this.hiveCells.push(item);
                }
            }
        }
    }

    sendTurn(actions) {
        this.sendString(this._serializeActions(actions));
    }

    sendString(send) {
        console.log(send);
    }

    async getString() {
        return await this.StdInReader.getLine();
    }

    _serializeActions(actions) {
        const mappedActions = [];
        for (let action of actions) {
            const mappedAction = {
                type: action.type
            };
            switch (action.type) {
                case ACTION.MOVE:
                    mappedAction.x = action.pos.x;
                    mappedAction.y = action.pos.y;
                    // Fallthrough.
                case ACTION.MOVE_QUEEN:
                    mappedAction.move = action.moveOrSpawnAmount;
                    mappedAction.face = action.face;
                    break;
                case ACTION.SPAWN:
                    mappedAction.x = action.pos.x;
                    mappedAction.y = action.pos.y;
                    mappedAction.amount = action.moveOrSpawnAmount;
                    mappedAction.face = action.face;
                    break;
                case ACTION.CREATE_HIVE:
                    break;
            }
            mappedActions.push(mappedAction);
        }
        return JSON.stringify(mappedActions);
    }

    isBesideHiveOrQueen(pos) {
        return this.isBeside(pos, this.queenBee.pos) || this.isBesideHive(pos);
    }

    isBesideHive(pos) {
        for (let hiveCell of this.hiveCells) {
            if (this.isBeside(pos, hiveCell.pos)) {
                return true;
            }
        }
        return false;
    }

    isBeside(pos, target) {
        return pos.equals(target) ||
            this.getBoundedPos(pos.x, pos.y + 1).equals(target) ||
            this.getBoundedPos(pos.x, pos.y - 1).equals(target) ||
            this.getBoundedPos(pos.x + 1, pos.y + 1).equals(target) ||
            this.getBoundedPos(pos.x + 1, pos.y - 1).equals(target) ||
            this.getBoundedPos(pos.x - 1, pos.y + 1).equals(target) ||
            this.getBoundedPos(pos.x - 1, pos.y - 1).equals(target) ||
            this.getBoundedPos(pos.x + 1, pos.y).equals(target) ||
            this.getBoundedPos(pos.x - 1, pos.y).equals(target);
    }

    isBesideEnemy(pos) {
        return this.hasEnemy(pos) ||
            this.hasEnemy(this.getBoundedPos(pos.x + 1, pos.y)) ||
            this.hasEnemy(this.getBoundedPos(pos.x + 1, pos.y + 1)) ||
            this.hasEnemy(this.getBoundedPos(pos.x + 1, pos.y - 1)) ||
            this.hasEnemy(this.getBoundedPos(pos.x - 1, pos.y)) ||
            this.hasEnemy(this.getBoundedPos(pos.x - 1, pos.y + 1)) ||
            this.hasEnemy(this.getBoundedPos(pos.x - 1, pos.y - 1)) ||
            this.hasEnemy(this.getBoundedPos(pos.x, pos.y + 1)) ||
            this.hasEnemy(this.getBoundedPos(pos.x, pos.y - 1));
    }

    hasEnemy(pos) {
        const cellBotId = this.currMap.map[pos.y][pos.x].getBeeBotId();
        return cellBotId != NEUTRAL_ID && cellBotId != this.id;
    }

    getBoundedPos(x, y) {
        return new Position((this.currMap.width + x) % this.currMap.width, (this.currMap.height + y) % this.currMap.height);
    }

    getMinPath(from, to) {
        return this.currMap.getPath(from, to);
    }
};
