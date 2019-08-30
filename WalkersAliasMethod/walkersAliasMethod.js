class WalkersAliasMethod {
    constructor(ary = null) {
        this._list = [];
        this._aliasList = [];
        if (ary instanceof Array) this.update(ary);
    }
    /**
     * 
     * @param {Array.<number>} ary - todo
     */
    get value() {
        const r = Math.random() * this._list.length;
        const index = Math.floor(r);
        const value = r - index;
        if (value < this._list[index]) return index;
        else return this._aliasList[index];
    }
    /**
     * 
     * @param {Array.<number>} ary 
     */
    update(ary) {
        const len = ary.length;
        const sum = ary.reduce((sum, v) => sum + v, 0);
        ary = ary.map(v => v * len / sum);
        const underAry = ary.map((v, i) => {return {v,i}}).filter(({v}) => v < 1).map(({i}) => i);
        const overAry = ary.map((v, i) => {return {v,i}}).filter(({v}) => v >= 1).map(({i}) => i);
        while (underAry.length > 0) {
            const underIndex = underAry[0];
            const overIndex = overAry[0];
            this._list[underIndex] = ary[underIndex];
            this._aliasList[underIndex] = overIndex;
            ary[overIndex] -= 1 - ary[underIndex];
            if (ary[overIndex] < 1) underAry.push(overAry.shift());
            underAry.shift();
        }
        while (overAry.length > 0) {
            this._list[overAry[0]] = 1;
            overAry.pop();
        }
    }
};

module.exports = WalkersAliasMethod;
