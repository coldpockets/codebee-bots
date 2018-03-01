exports.Position = class Position {
    constructor(x = 0, y = 0) {
        this.x = x;
        this.y = y;
    }

    equals(pos) {
        return this.x == pos.x && this.y == pos.y;
    }
}
