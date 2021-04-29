class Square2 {
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

  // 00000000
  update(address, byte) {
    if (address == 0xff16) {
      this.waveDuty = this.getWaveDuty((byte >> 6) & 3);
      this.soundLength = byte & 0x3f;
    }
    if (address == 0xff17) {
      this.volume = (byte >> 4) & 0x0f;
      this.increaseVolume = isSet(byte, 3);
      this.envelopePeriod = byte & 7;
    }
    if (address == 0xff18) {
      this.frequency = this.getFrequency();
    }
    if (address == 0xff19) {
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

  restart() {
    if (this.soundLength == 0) {
      this.soundLength = 64;
    }
    this.frequency = this.getFrequency();
    this.envelopeVolume = (memory[0xff17] >> 4) & 0x0F;
    this.playTone();
  }

  getFrequency() {
    let lowerFreq = memory[0xff18];
    let higherFreq = memory[0xff19];

    higherFreq = (0x07 & higherFreq) << 8;
    let frequency = higherFreq | lowerFreq;

    return 131071 / (2048 - frequency);
  }
}