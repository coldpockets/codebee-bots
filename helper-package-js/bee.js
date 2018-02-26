const { Insect } = require('./insect');

exports.Bee = class Bee extends Insect {
    constructor(id, botId, pos, face, pollen = 0, count = 1) {
        super(id, botId, pos, face, pollen);
        this.count = count;
    }
}