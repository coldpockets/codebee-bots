exports.NEUTRAL_ID = -1;
exports.BEE_POLLEN_AMOUNT = 5;
exports.HIVE_POLLEN_AMOUNT = 100;
exports.MAX_BEES = 100;
exports.MAX_POLLEN_FACTOR = 3;

exports.ACTION = Object.freeze({
    MOVE: 0,
    SPAWN: 1,
    CREATE_HIVE: 2,
    MOVE_QUEEN: 3
});

exports.FACE = Object.freeze({
    UP: 0,
    RIGHT: 1,
    DOWN: 2,
    LEFT: 3
});

exports.FLOWER = Object.freeze({
    NONE: 0,
    REGULAR: 1,
    POTENT: 2
});

exports.MOVE = Object.freeze({
    STAY: 0,
    UP: 1,
    RIGHT: 2,
    DOWN: 3,
    LEFT: 4
});
