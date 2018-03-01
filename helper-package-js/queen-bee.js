const { Insect } = require('./insect');

exports.QueenBee = class QueenBee extends Insect {
    constructor(id, botId, pos, face, pollen = 0) {
        super(id, botId, pos, face, pollen);
    }
};
