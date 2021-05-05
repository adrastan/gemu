
class SoundController {
  constructor() {
    this.ctx = new (window.AudioContext || window.webkitAudioContext)();
    this.channel1 = new Square1(this.ctx);
    this.channel2 = new Square2(this.ctx);
    this.channel3 = new Wave(this.ctx);
    this.channel4 = new Noise(this.ctx);
    this.frame = 0;
    this.cycles = 0;
    this.regs = {
      get NR50() { return this._NR50 | 0x00 },
      get NR51() { return this._NR51 | 0x00 },
      get NR52() { return this._NR52 | 0x70 },
      set NR50(value) { this._NR50 = value },
      set NR51(value) { this._NR51 = value },
      set NR52(value) { this._NR52 = value },
    }
  }

  endFrame() {
    // this.channel1.start();
    // this.channel2.start();
  }

  restart() {
    this.frame = 0;
    this.cycles = 0;
    this.channel1.disable();
    this.channel2.disable();
    this.channel3.disable();
    this.channel4.disable();
  }

  updateCycles(cycles) {
    this.cycles += cycles;
    if (this.cycles >= 8192) {
      this.clockFrame();
    }
  }

  clockFrame() {
    this.cycles -= 8192;
    this.frame++;

    // if (!this.channel1.playing && this.channel1.enabled) {
    //   this.channel1.start();
    // }
    // if (!this.channel2.playing && this.channel2.enabled) {
    //   this.channel2.start();
    // }
    // this.channel1.playTone();
    // this.channel2.playTone();
    // if (this.channel1.ready && this.channel2.ready) {
    //   this.channel1.start();
    //   this.channel2.start();
    // }

    if (this.frame % 2 == 1) {
      this.channel1.clockLength();
      this.channel2.clockLength();
    }
    if (this.frame == 3 || this.frame == 7) {
      this.channel1.clockSweep();
    }
    if (this.frame == 8) {
      this.channel1.clockEnvelope();
      this.channel2.clockEnvelope();
      this.frame = 0;
    }
  }

  write(address, byte) {
    if (address >= 0xff30 && address <= 0xff3f) {
      this.channel3.waveRam[address - 0xff30] = byte;
      this.channel3.update();
      return;
    }

    if (address == 0xff26 && !byte) {
      this.powerOff();
      return;
    } else if (address == 0xff26 && isSet(byte, 7)) {
      this.powerOn();
      return;
    } else if (!this.enabled()) {
      return;
    }

    if (address >= 0xff24 && address <= 0xff26) {
      if (address == 0xff24) this.NR50 = byte;
      if (address == 0xff25) this.NR51 = byte;
    } else if (address >= 0xff10 && address <= 0xff14) {
      if (address == 0xff10) this.channel1.NR10 = byte;
      if (address == 0xff11) this.channel1.NR11 = byte;
      if (address == 0xff12) this.channel1.NR12 = byte;
      if (address == 0xff13) this.channel1.NR13 = byte;
      if (address == 0xff14) this.channel1.NR14 = byte;
    } else if (address >= 0xff16 && address <= 0xff19) {
      if (address == 0xff16) this.channel2.NR21 = byte;
      if (address == 0xff17) this.channel2.NR22 = byte;
      if (address == 0xff18) this.channel2.NR23 = byte;
      if (address == 0xff19) this.channel2.NR24 = byte;
    } else if ((address >= 0xff1a && address <= 0xff1e) || (address >= 0xff30 && address <= 0xff3f)) {
      if (address == 0xff1a) this.channel3.NR30 = byte;
      if (address == 0xff1b) this.channel3.NR31 = byte;
      if (address == 0xff1c) this.channel3.NR32 = byte;
      if (address == 0xff1d) this.channel3.NR33 = byte;
    } else if (address >= 0xff20 && address <= 0xff23) {
      if (address == 0xff20) this.channel4.NR41 = byte;
      if (address == 0xff21) this.channel4.NR42 = byte;
      if (address == 0xff22) this.channel4.NR43 = byte;
      if (address == 0xff23) this.channel4.NR44 = byte;
    }
  }

  powerOn() {
    this.NR52 = 0xF0;
  }

  powerOff() {
    this.clearRegs();
    this.channel1.disable();
    this.channel2.disable();
    this.channel3.disable();
    this.channel4.disable();
  }

  enabled() { 
    return isSet(this.NR52, 7);
  }

  clearRegs() {
    this.NR50 = 0;
    this.NR51 = 0;
    this.NR52 = 0x70;
    this.channel1.clear();
    this.channel2.clear();
    this.channel3.clear();
    this.channel4.clear();
  }
}