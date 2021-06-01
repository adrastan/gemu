class Square {
  constructor(ctx) {
    this.ctx = ctx;
    this.enabled = false;
    this.lengthEnabled = false;
    this.soundLength = 0;
  }

  init() {
    if (this.osc) {
      this.stop();
    }
    this.gain = this.ctx.createGain();
    this.osc = this.ctx.createOscillator();
    this.osc.type = "triangle";
    this.waveShaper = this.ctx.createWaveShaper();
    this.waveShaper.curve = this.getCurve(val => val <= 0 ? -1 : 1);
    this.constantSource = this.ctx.createConstantSource();
    this.g = this.ctx.createGain();
    this.constantSource.connect(this.g);
    this.g.connect(this.waveShaper);
    this.osc.connect(this.waveShaper);
    this.waveShaper.connect(this.gain);
    this.gain.connect(this.ctx.destination);
    this.updateFrequency();
    this.updateGain();
    this.osc.start();
    this.constantSource.start();
  }

  getWaveDuty(value) {
    switch (value) {
      case 0: return -0.25;
      case 1: return -0.5;
      case 2: return 0;
      case 3: return 0.5;
    }
  }

  getFrequency(lowerFreq, higherFreq) {
    higherFreq = (0x07 & higherFreq) << 8;
    let frequency = higherFreq | lowerFreq;

    return 131071 / (2048 - frequency);
  }

  updateFrequency() {
    if (this.osc && this.frequency) {
      this.osc.frequency.setValueAtTime(this.frequency, this.ctx.currentTime);
    }
  }

  updateGain() {
    if (this.gain) {
      this.gain.gain.setValueAtTime((this.envelopeVolume || 0) / 15, this.ctx.currentTime);
      this.g.gain.setValueAtTime(this.waveDuty || 0, this.ctx.currentTime);
    }
  }

  trigger() {
    this.init();
    if (!this.soundDuration) {
      this.soundDuration = 1;
    }

    if (this.lengthEnabled) {
      this.osc.stop(this.ctx.currentTime + this.soundDuration);
    }

    if (this.envelopePeriod && this.gain) {
      this.envelope();
    }
    if (this.sweepDuration) {
      this.sweep();
    }
  }

  envelope() {
    if (this.increaseVolume) {
      this.envelopeIncrease();
    } else {
      this.envelopeDecrease();
    }
  }

  sweep() {
    let time = this.ctx.currentTime;
    let f = this.osc.frequency.value;
    let elapsed = 0;

    while (elapsed < this.soundDuration) {
      if (this.sweepIncrease) {
        f = f + f / Math.pow(2, this.sweepShift);
      } else {
        f = f - f / Math.pow(2, this.sweepShift);
      }
      elapsed += time + this.sweepDuration;
      time = time + this.sweepDuration;
      elapsed += this.sweepDuration;
      this.osc.frequency.linearRampToValueAtTime(f, time);
    }
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

  getCurve(mapping, length = 1024) {
    const array = new Float32Array(length);
    for (let i = 0, len = length; i < len; i++) {
      const normalized = (i / (len - 1)) * 2 - 1;
      array[i] = mapping(normalized, i);
    }
    return array;
  }

  stop() {
    if (!this.osc) {
      return;
    }
    this.gain.gain.cancelScheduledValues(this.ctx.currentTime);
    this.osc.frequency.cancelScheduledValues(this.ctx.currentTime);
    this.osc.stop();
    this.constantSource.stop();
    this.osc.disconnect();
    this.gain.disconnect();
    this.osc = null;
    this.constantSource.disconnect();
  }
}