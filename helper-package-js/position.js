exports.Position = class Position {
    constructor(x, y) {
        this.x = x;
        this.y = y;
    }

    equals(pos) {
        return this.x == pos.x && this.y == pos.y;
    }
}
