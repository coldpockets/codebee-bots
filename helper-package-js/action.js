const { FACE } = require('./constants');
const { Position } = require('./position');

exports.Action = class Action {
    constructor(options) {
        this.type = options.type;
        this.moveOrSpawnAmount = options.move || options.amount || 0;
        this.face = options.face || FACE.UP;
        this.pos = options.pos || new Position();
    }
};
