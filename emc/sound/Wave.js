class Wave {
  constructor() {
    this.waveRam = new Uint8Array(16);
    this.regs = {
      get NR30() { return this._NR30 | 0x7F },
      get NR31() { return this._NR31 | 0xFF },
      get NR32() { return this._NR32 | 0x9F },
      get NR33() { return this._NR33 | 0xFF },
      get NR34() { return this._NR34 | 0xBF },
      set NR30(value) { this._NR30 = value },
      set NR31(value) { this._NR31 = value },
      set NR32(value) { this._NR32 = value },
      set NR33(value) { this._NR33 = value },
      set NR34(value) { this._NR34 = value },
    }
  }

  clear() {
    this.NR30 = 0;
    this.NR31 = 0;
    this.NR32 = 0;
    this.NR33 = 0;
    this.NR34 = 0;
    this.waveRam = new Uint8Array(16);
  }

  disable() {
    
  }

  update(address, byte) {

  }

  stop() {
    
  }
}