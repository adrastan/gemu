class Noise {
  constructor(ctx) {
    this.ctx = ctx;
    this.cycles = 0;
    
    this.buffer1 = new Float32Array(32766);
    this.buffer2 = new Float32Array(128);
    let LSFR = 0x7FFF;

    for (let i = 0; i < this.buffer1.length; ++i) {
      let xor = getBit(LSFR, 0) ^ getBit(LSFR, 1);
      LSFR >>= 1;
      LSFR = xor ? setBit(LSFR, 14) : clearBit(LSFR, 14);
      this.buffer1[i] = getBit(LSFR, 0) ? -0.5 : 0.5;
    }
    LSFR = 0x7FFF;
    for (let i = 0; i < this.buffer2.length; ++i) {
      let xor = getBit(LSFR, 0) ^ getBit(LSFR, 1);
      LSFR >>= 1;
      LSFR = xor ? setBit(LSFR, 14) : clearBit(LSFR, 14);
      LSFR = xor ? setBit(LSFR, 6) : clearBit(LSFR, 6);
      this.buffer2[i] = getBit(LSFR, 0) ? -0.5 : 0.5;
    }
  }

  get NR41() { return this._NR41 }
  get NR42() { return this._NR42 }
  get NR43() { return this._NR43 }
  get NR44() { return this._NR44 }

  set NR41(value) { 
    this._NR41 = value;
    this.soundLength = value & 0x3F;
    this.soundDuration = (64 - this.soundLength) * (1 / 256);
  }

  set NR42(value) { 
    this._NR42 = value;
    this.envelopeVolume = value >> 4;
    this.increaseVolume = isSet(value, 3);
    this.envelopePeriod = value & 0x07;
    this.envelopeDuration = this.envelopePeriod * (1 / 64);
    if ((value >> 3) == 0) {
      this.dacEnabled = false;
      this.disable();
    } else {
      this.dacEnabled = true;
    }
  }

  set NR43(value) { 
    //0000 0000
    this._NR43 = value;
    this.shiftFrequency = value >> 4;
    this.stepWidth = isSet(value, 3) ? 7 : 15;
    this.dividingRatio = value & 0x07 || 0.5;
    this.frequency = 524288 / this.dividingRatio / Math.pow(2, this.shiftFrequency + 1);
  }

  set NR44(value) { 
    this._NR44 = value;
    this.lengthEnabled = isSet(value, 6);
    if (isSet(value, 7) && this.frequency != null && this.dacEnabled) {
      this.trigger();
    }
  }

  endFrame() {

  }

  updateCycles(cycles) {
    this.cycles += cycles;
    if (this.cycles >= this.period) {
      this.clock();
      this.cycles -= this.period;
    }
  }

  clock() {

  }

  trigger() {
    if (!this.soundDuration) {
      this.soundDuration = 1;
    }
    this.start();

    if (this.envelopePeriod) {
      if (this.increaseVolume) {
        this.envelopeIncrease();
      } else {
        this.envelopeDecrease();
      }
    }

    if (this.lengthEnabled) {
      this.source.stop(this.ctx.currentTime + this.soundDuration);
    }
  }

  start() {
    if (this.source) {
      this.source.stop();
    }
    this.gain = this.ctx.createGain();
    this.gain.connect(this.ctx.destination);
    this.gain.gain.setValueAtTime((this.envelopeVolume || 0) / 15, this.ctx.currentTime);
    this.source = this.ctx.createBufferSource();
    let buf, buffer;

    if (isSet(this.NR43, 3)) {
      buf = this.buffer2;
      buffer = this.ctx.createBuffer(1, buf.length, this.ctx.sampleRate);
    } else {
      buf = this.buffer1;
      buffer = this.ctx.createBuffer(1, buf.length, this.ctx.sampleRate);
    }

    buffer.copyToChannel(buf, 0, 0);
    this.source.buffer = buffer;
    this.source.loop = true;
    this.source.connect(this.gain);
    this.source.playbackRate.value = (buf.length) * this.frequency / this.ctx.sampleRate / 32;
    this.source.start();
  }

  envelopeDecrease() {
    let time = this.ctx.currentTime;

    while (this.envelopeVolume > 0) {
      this.envelopeVolume--;
      time = time + this.envelopeDuration;
      this.gain.gain.linearRampToValueAtTime(this.envelopeVolume / 15, time);
    }
  }

  envelopeIncrease() {
    let time = this.ctx.currentTime;

    while (this.envelopeVolume < 15) {
      this.envelopeVolume++;
      time = time + this.envelopeDuration;
      this.gain.gain.linearRampToValueAtTime(this.envelopeVolume / 15, time);
    }
  }

  disable() {
    this.stop();
  }

  clear() {
    this.NR41 = 0;
    this.NR42 = 0;
    this.NR43 = 0;
    this.NR44 = 0;
  }

  stop() {
    if (!this.source) return;
    this.gain.gain.value = 0;
    this.source.stop();
    this.source = null;
  }
}