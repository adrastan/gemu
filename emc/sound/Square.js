class Square {
  constructor(ctx) {
    this.ctx = ctx;
    this.enabled = false;
    this.lengthEnabled = false;
    this.soundLength = 0;
    this.init();
    this.start();
  }
  
  init() {
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

  trigger() {
    if (this.soundLength == 0 || !this.lengthEnabled) {
      if (this.lengthClocked) {
        this.soundLength = 63;
      } else {
        this.soundLength = 64;
      }
    }
    if (!this.dacEnabled) return;
    this.enable();
    this.playTone();
  }

  clockLength() {
    if (this.lengthEnabled && this.soundLength) {
      this.soundLength--;
      if (this.soundLength == 0) {
        this.disable();
      }
    }
    this.lengthClocked = !this.lengthClocked;
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

  rampGain() {
    let time = this.ctx.currentTime + (this.envelopePeriod * (1 / 64));
    this.gain.gain.linearRampToValueAtTime((this.envelopeVolume / 15), time);
  }

  disable() {
    this.enabled = false;
    this.envelopeVolume = 0;
    this.mute();
  }

  enable() {
    this.enabled = true;
  }

  playTone() {
    if (this.frequency == null) return;
    this.gain.gain.setValueAtTime(this.envelopeVolume / 15, this.ctx.currentTime);
    this.osc.frequency.setValueAtTime(this.frequency, this.ctx.currentTime);
    this.g.gain.setValueAtTime(this.waveDuty || 0, this.ctx.currentTime);
  }

  getCurve(mapping, length = 1024) {
    const array = new Float32Array(length);
    for (let i = 0, len = length; i < len; i++) {
      const normalized = (i / (len - 1)) * 2 - 1;
      array[i] = mapping(normalized, i);
    }
    return array;
  }

  start() {
    if (!this.started) {
      this.osc.start();
      this.constantSource.start();
    }
    this.started = true;
  }

  mute() {
    this.gain.gain.setValueAtTime(0, this.ctx.currentTime);
    this.g.gain.setValueAtTime(0, this.ctx.currentTime);
  }

  stop() {
    if (this.started) {
      this.osc.stop();
      this.constantSource.stop();
    }
    this.started = false;
  }
}