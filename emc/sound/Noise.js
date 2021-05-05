class Noise {
  constructor() {
    this.regs = {
      get NR41() { return this._NR41 | 0xFF },
      get NR42() { return this._NR42 | 0x00 },
      get NR43() { return this._NR43 | 0x00 },
      get NR44() { return this._NR44 | 0xBF },
      set NR41(value) { this._NR41 = value },
      set NR42(value) { this._NR42 = value },
      set NR43(value) { this._NR43 = value },
      set NR44(value) { this._NR44 = value },
    }
  }

  disable() {

  }

  update(address, byte) {

  }

  clear() {
    this.NR41 = 0;
    this.NR42 = 0;
    this.NR43 = 0;
    this.NR44 = 0;
  }

  stop() {
    
  }
}