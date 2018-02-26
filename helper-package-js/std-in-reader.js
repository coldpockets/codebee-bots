exports.StdInReader = class StdInReader {
    constructor() {
        this.data = '';
        this.lines = [];
        this.pending = [];

        process.stdin.resume();
        process.stdin.setEncoding('utf8');
        process.stdin.on('data', data => this._handleData(data));
    }

    _getPromise() {
        let resolve;
        const promise = new Promise(r => resolve = r);
        return [promise, resolve];
    }

    _handleData(data) {
        this.data += data;
        const lines = this.data.split('\n');
        if (lines.length > 1) {
            this.data = lines.pop();
            this.lines = [...this.lines, ...lines.map(s => s.trim())];
            this._returnLines();
        }
    }

    _returnLines() {
        while (this.pending.length && this.lines.length) {
            const resolve = this.pending.shift();
            resolve(this.lines.shift());
        }
    }

    async getLine() {
        const [promise, resolve] = this._getPromise();
        this.pending.push(resolve);
        this._returnLines();
        return promise;
    }
}