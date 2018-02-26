const { FACE } = require('./constants');

exports.Action = class Action {
    constructor(type, moveOrSpawnAmount = 0, face = FACE.UP, pos) {
        this.type = type;
        this.moveOrSpawnAmount = moveOrSpawnAmount;
        this.face = face;
        this.pos = pos;
    }
}
