
class Square1 {
  constructor(ctx) {
    this.ctx = ctx;
    this.enabled = false;
    this.lengthEnabled = false;
    this.soundLength = 0;
  }

  getWaveDuty(value) {
    switch (value) {
      case 0: return -0.25;
      case 1: return -0.5;
      case 2: return 0;
      case 3: return 0.5;
    }
  }
  
  clockLength() {
    if (this.lengthEnabled && this.soundLength) {
      this.soundLength--;
      if (this.soundLength == 0) {
        this.disable();
      }
    }
  }

  clockEnvelope() {
    if (this.envelopePeriod) {
      if (this.increaseVolume && this.envelopeVolume < 15) {
        this.envelopeVolume++;
        this.rampGain();
      } else if (!this.increaseVolume && this.envelopeVolume > 0) {
        this.envelopeVolume--;
        this.rampGain();
      }
    }
  }

  // not used by tetris
  clockSweep() {
    // if (this.sweepEnabled && this.sweepTime) {
    //   let f = this.frequencyCalculation();
    //   if (f > 2047) {
    //     this.disable();
    //   } else if (f <= 2047 && this.sweepShift) {
    //     this.frequencyShadow = f;
    //     this.updateFrequencyRegs(f);
    //     f = this.frequencyCalculation();
    //     if (f > 2047) {
    //       this.disable();
    //     } else {
    //       this.osc.linearRampToValueAtTime(f, this.ctx.currentTime + this.getSweepTime(f))
    //     }
    //   }
    // }
  }

  rampGain() {
    let time = this.ctx.currentTime + (this.envelopePeriod * (1 / 64));
    this.gain.gain.linearRampToValueAtTime((this.envelopeVolume / 15), time);
  }

  disable() {
    this.enabled = false;
    this.stop();
  }

  enable() {
    this.enabled = true;
  }

  playTone() {
    if (!this.enabled) {
      return;
    }
    this.stop();

    this.gain = this.ctx.createGain();
    this.gain.gain.setValueAtTime(this.envelopeVolume / 15, this.ctx.currentTime);
    this.osc = this.ctx.createOscillator();
    this.osc.type = "triangle";
    this.osc.frequency.setValueAtTime(this.frequency, this.ctx.currentTime);
    this.waveShaper = this.ctx.createWaveShaper();
    this.waveShaper.curve = this.getCurve(val => val <= 0 ? -1 : 1);

    this.constantSource = this.ctx.createConstantSource();
    let g = this.ctx.createGain();
    g.gain.setValueAtTime(this.waveDuty || 0, this.ctx.currentTime);
    this.constantSource.connect(g);
    g.connect(this.waveShaper);
    this.constantSource.start();

    this.osc.connect(this.waveShaper);
    this.waveShaper.connect(this.gain);
    this.gain.connect(this.ctx.destination);

    this.osc.start();
  }

  getCurve(mapping, length = 1024) {
    const array = new Float32Array(length);
    for (let i = 0, len = length; i < len; i++) {
      const normalized = (i / (len - 1)) * 2 - 1;
      array[i] = mapping(normalized, i);
    }
    return array;
  }

  stop() {
    if (this.osc) {
      this.osc.stop();
    }
    if (this.constantSource) {
      this.constantSource.stop();
    }
  }

  getSweepTime(t) {
    switch (t) {
      case 1: return 0.0078;
      case 2: return 0.0156;
      case 3: return 0.0234;
      case 4: return 0.0313;
      case 5: return 0.0391;
      case 6: return 0.0469;
      case 7: return 0.0547;
    }
  }

  updateFrequencyRegs(f) {
    memory[0xff13] = f & 0xff;
    memory[0xff14] |= (f >> 8) & 7;
  }

  // 00000000
  update(address, byte) {
    if (address == 0xff10) {
      this.sweepTime = (byte >> 6) & 7;
      this.sweepIncrease = !isSet(byte, 3);
      this.sweepShift = byte & 7;
    }
    if (address == 0xff11) {
      this.waveDuty = this.getWaveDuty((byte >> 6) & 3);
      this.soundLength = byte & 0x3f;
    }
    if (address == 0xff12) {
      this.volume = (byte >> 4) & 0x0f;
      this.increaseVolume = isSet(byte, 3);
      this.envelopePeriod = byte & 7;
    }
    if (address == 0xff13) {
      this.frequency = this.getFrequency();
    }
    if (address == 0xff14) {
      this.updateChannelControl(byte);
    }
  }

  updateChannelControl(byte) {
    if (isSet(byte, 7)) {
      this.enable();
      this.restart();
    }
    this.lengthEnabled = isSet(byte, 6);
  }

  frequencyCalculation() {
    let f = this.frequencyShadow >> this.sweepShift;
    if (this.sweepIncrease) {
      f += this.frequencyShadow;
    } else {
      f -= this.frequencyShadow;
    }
    return f;
  }

  sweepTrigger() {
    this.sweepEnabled = !!(this.sweepTime || this.sweepShift);
    this.frequencyShadow = this.getFrequency();
    this.sweepTime = (memory[0xff10] >> 6) & 7;
    if (this.sweepShift) {
      let f = this.frequencyCalculation();
      if (f > 2047) {
        this.disable();
      }
    }
  }

  restart() {
    this.sweepTrigger();
    if (this.soundLength == 0) {
      this.soundLength = 64;
    }
    this.frequency = this.getFrequency();
    this.envelopeVolume = (memory[0xff12] >> 4) & 0x0F;
    this.playTone();
  }

  getFrequency() {
    let lowerFreq = memory[0xff13];
    let higherFreq = memory[0xff14];

    higherFreq = (0x07 & higherFreq) << 8;
    let frequency = higherFreq | lowerFreq;

    return 131071 / (2048 - frequency);
  }
}