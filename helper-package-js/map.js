const { MOVE } = require('./constants');

const { Bee } = require('./bee');
const { Cell } = require('./cell');
const { Position } = require('./position');
const { QueenBee } = require('./queen-bee');

const JSON_INDICES = Object.freeze({
    HIVE: Object.freeze({
        X: 0,
        Y: 1,
        OWNER_ID: 2,
    }),
    INSECT: Object.freeze({
        X: 0,
        Y: 1,
        ID: 2,
        BOT_ID: 3,
        FACE: 4,
        POLLEN: 5,
        COUNT: 6,
    }),
    STATE: Object.freeze({
        BEES: 0,
        QUEENS: 1,
        HIVES: 2,
    }),
});

const Path = class Path {
    constructor(distance, move) {
        this.distance = distance || 0;
        this.move = move || MOVE.UP;
    }

    // Less than overrided??
};

exports.Path = Path;

exports.Map = class Map {
    constructor(pollenMap) {
        const pMap = JSON.parse(pollenMap);

        this.height = pMap.length;
        this.width = pMap[0].length;
        this.map = [];
        for (let y = 0; y < this.height; ++y) {
            this.map.push([]);
            for (let x = 0; x < this.width; ++x) {
                this.map[y].push(new Cell(new Position(x, y), pMap[y][x]));
            }
        }

        this.queenBees = [];
    }

    updateMap(newState) {
        const pState = JSON.parse(newState);
        const pBees = pState[JSON_INDICES.STATE.BEES];
        const pQueens = pState[JSON_INDICES.STATE.QUEENS];
        const pHives = pState[JSON_INDICES.STATE.HIVES];

        // Clear all bees and hives.
        for (let row of this.map) {
            for (let item of row) {
                item.clear();
            }
        }
        this.queenBees.length = 0;

        // Set bees.
        for (let pBee of pBees) {
            const bee = new Bee(
                pBee[JSON_INDICES.INSECT.ID],
                pBee[JSON_INDICES.INSECT.BOT_ID],
                new Position(pBee[JSON_INDICES.INSECT.X], pBee[JSON_INDICES.INSECT.Y]),
                pBee[JSON_INDICES.INSECT.FACE],
                pBee[JSON_INDICES.INSECT.POLLEN],
                pBee[JSON_INDICES.INSECT.COUNT],
            );
            this.map[bee.pos.y][bee.pos.x].bee = bee;
        }

        // Set queen bees.
        for (let pQueen of pQueens) {
            this.queenBees.push(new QueenBee(
                pQueen[JSON_INDICES.INSECT.ID],
                pQueen[JSON_INDICES.INSECT.BOT_ID],
                new Position(pQueen[JSON_INDICES.INSECT.X], pQueen[JSON_INDICES.INSECT.Y]),
                pQueen[JSON_INDICES.INSECT.FACE],
                pQueen[JSON_INDICES.INSECT.POLLEN],
            ));
        }

        // Set hives.
        for (let pHive of pHives) {
            this.map[pHive[JSON_INDICES.HIVE.Y]][pHive[JSON_INDICES.HIVE.X]].ownerId = pHive[JSON_INDICES.HIVE.OWNER_ID];
        }
    }

    getPath(from, to) {
        const path = new Path();
        const downDist = (this.height + to.y - from.y) % this.height;
        const upDist = (this.height + from.y - to.y) % this.height;
        const rightDist = (this.width + to.x - from.x) % this.width;
        const leftDist = (this.width + from.x - to.x) % this.width;

        let move1 = MOVE.STAY;
        let move2 = MOVE.STAY;
        if (to.y != from.y) {
            if (downDist > upDist) {
                move1 = MOVE.UP;
            } else {
                move1 = MOVE.DOWN;
            }
            path.move = move1;
        }
        if (to.x != from.x) {
            if (rightDist > leftDist) {
                move2 = MOVE.LEFT;
            } else {
                move2 = MOVE.RIGHT;
            }
            path.move = move2;
        }

        if (move1 != MOVE.STAY && move2 != MOVE.STAY) {
            path.move = Math.floor((Math.random() * 2)) == 0 ? move1 : move2;
        }

        path.distance = Math.min(rightDist, leftDist) + Math.min(downDist, upDist);

        return path;
    }
};