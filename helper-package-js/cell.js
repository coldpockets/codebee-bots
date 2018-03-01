const { NEUTRAL_ID } = require('./constants');

exports.Cell = class Cell {
    constructor(pos, potency) {
        this.pos = pos;
        this.potency = potency;
        this.ownerId = NEUTRAL_ID;
        this.bee = null;
    }

    clear() {
        this.ownerId = NEUTRAL_ID;
        this.bee = null;
    }

    getBeeBotId() {
        return bee ? bee.botId : NEUTRAL_ID;
    }
};