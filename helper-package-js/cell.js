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
        return this.bee ? this.bee.botId : NEUTRAL_ID;
    }
};