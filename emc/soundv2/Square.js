class Square {
  constructor(ctx) {
    this.ctx = ctx;
    this.dutyTable = {
      0: [0,0,0,0,0,0,0,1],
      1: [1,0,0,0,0,0,0,1],
      2: [1,0,0,0,0,1,1,1],
      3: [0,1,1,1,1,1,1,0],
    }
    this.currentDuty = 0;
    this.volume = 0;
    this.enabled = false;
    this.buffer = [];
    this.sources = [];
  }

  play() {
    this.start = this.start || this.ctx.currentTime;
    while (this.sources.length) {
      let source = this.sources.shift();
      source.start(this.start);
      this.start += source.buffer.duration / source.playbackRate.value;
    }
  }

  createBuffer() {
    let source = this.ctx.createBufferSource();
    let buffer = this.ctx.createBuffer(1, this.buffer.length, this.ctx.sampleRate);
    buffer.copyToChannel(new Float32Array(this.buffer), 0, 0);
    source.buffer = buffer;
    source.playbackRate.value = 1;
    source.connect(this.ctx.destination);
    this.sources.push(source);
  }

  updateCycles(cycles) {
    this.dutyTimer -= cycles;
    if (this.dutyTimer <= 0) {
      this.advanceDuty();
      this.dutyTimer += (2048 - this.frequency) * 4;
    }
  }

  endFrame() {
    if (this.sources && this.sources.length) {
      this.play();
    }
  }

  readSample() {
    if (!this.dacEnabled || !this.enabled) {
      this.buffer.push(0);
    } else {
      let amp = this.dutyTable[this.waveDuty || 0][this.currentDuty || 0];
      amp = amp * (this.volume / 15);
      this.buffer.push(amp);
    }
    if (this.buffer.length > BUF_SIZE) {
      this.createBuffer();
      this.buffer = [];
    }
  }

  advanceDuty() {
    if (!this.dutyEnabled) {
      return;
    }
    this.currentDuty++;
    if (this.currentDuty === 8) {
      this.currentDuty = 0;
    }
  }

  clockLength() {
    if (this.lengthEnabled && this.soundLength) {
      this.soundLength--;
      if (this.soundLength === 0) {
        this.enabled = false;
      }
    }
  }

  clockSweep() {

  }

  clockEnvelope() {
    if (this.envelopePeriod && this.envelopeTimer > 0) {
      this.envelopeTimer--;
      if (this.envelopeTimer === 0) {
        this.envelopeTimer = this.envelopePeriod;
        if (this.increaseVolume && this.volume < 15) {
          this.volume++;
        } else if (this.volume > 0) {
          this.volume--;
        }
      }
    }
  }

  //4194304
  trigger() {
    this.enabled = true;
    this.dutyEnabled = true; // disable duty advance until first trigger
    this.dutyTimer = (2048 - this.frequency) * 4;
    if (!this.soundLength) {
      this.soundLength = 64;
    }
    this.envelopeTimer = this.envelopePeriod || 8;
    if (this.frame === 7) {
      this.envelopeTimer++;
    }
    this.volume = this.envelopeVolume;
  }

  stop() {
    this.enabled = false;
    this.volume = 0;
  }
}