class Wave {
  constructor(ctx) {
    this.ctx = ctx;
    this.waveRam = new ArrayBuffer(16);
    this.sampleBuffer = new Float32Array(32);
    this.position = 0;
    this.cycles = 0;
  }

  get NR30() { return this._NR30 }
  get NR31() { return this._NR31 }
  get NR32() { return this._NR32 }
  get NR33() { return this._NR33 }
  get NR34() { return this._NR34 }

  set NR30(value) {
    this._NR30 = value;
    this.enabled = isSet(value, 7);
    if (!this.enabled) {
      this.disable();
    }
  }

  set NR31(value) {
    this._NR31 = value;
    this.soundDuration = (256 - value) * (1 / 256); // seconds
  }

  set NR32(value) {
    this._NR32 = value;
    this.outputLevel = (value >> 5) & 0x03;
    this.updateGain();
  }

  set NR33(value) {
    this._NR33 = value;
    this.frequency = this.getFrequency(value, this._NR34);
    this.updateFrequency();
  }

  set NR34(value) {
    this._NR34 = value;
    this.lengthEnabled = isSet(value, 6);

    if (isSet(value, 7) && this.enabled) {
      this.trigger();
    }

    this.frequency = this.getFrequency(this._NR33, value);
    this.updateFrequency();
  }

  init() {
    if (this.source) {
      this.stop();
    }
    this.buffer = this.ctx.createBuffer(1, 32, 65536);
    this.gain = this.ctx.createGain();
    this.updateGain();
    this.source = this.ctx.createBufferSource();
    this.source.buffer = this.buffer;
    this.source.loop = true;
    this.source.connect(this.gain);
    this.gain.connect(this.ctx.destination);
    this.source.start();
  }

  updateFrequency() {
    if (this.source) {
      this.source.playbackRate.value = 32 * this.frequency / 65536 ;
      //this.source.playbackRate.value = this.frequency / 65536;
    }
  }

  updateGain() {
    if (!this.gain) return;
    if (this.outputLevel == 0) this.gain.gain.value = 0;
    else if (this.outputLevel == 1) this.gain.gain.value = 1;
    else if (this.outputLevel == 2) this.gain.gain.value = 0.5;
    else this.gain.gain.value = 0.25;
  }

  convertRange( value, r1, r2 ) { 
    return ( value - r1[ 0 ] ) * ( r2[ 1 ] - r2[ 0 ] ) / ( r1[ 1 ] - r1[ 0 ] ) + r2[ 0 ];
  }

  trigger() {
    this.init();
    if (!this.soundDuration) {
      this.soundDuration = 1;
    }
    this.position = 0;
    this.period = (2048 - this.frequencyRaw) * 2;
    let count = 0;
    for (let i = 0; i < 16; ++i) {
      this.sampleBuffer[count] = this.convertRange(((this.waveRam[i] >> 4)), [0, 15], [-1, 1]);
      this.sampleBuffer[count + 1] = this.convertRange(((this.waveRam[i] & 0x0f)), [0, 15], [-1, 1]);
      count += 2;
    }
 
    this.buffer.copyToChannel(this.sampleBuffer, 0, 0);
    if (this.lengthEnabled) {
      this.source.stop(this.ctx.currentTime + this.soundDuration);
    }
  }

  getFrequency(lowerFreq, higherFreq) {
    higherFreq = (0x07 & higherFreq) << 8;
    let frequency = higherFreq | lowerFreq;

    this.frequencyRaw = frequency;
    return 65536 / (2048 - frequency);
  }

  clear() {
    this.NR30 = 0;
    this.NR31 = 0;
    this.NR32 = 0;
    this.NR33 = 0;
    this.NR34 = 0;
    for (let i = 0; i < 32; ++i) {
      this.sampleBuffer[i] = 0;
    }
  }

  mute() {
    if (this.gain) {
      this.gain.gain.setValueAtTime(0, this.ctx.currentTime);
    }
  }

  disable() {
    this.stop();
  }

  stop() {
    if (!this.source) return;
    this.source.stop();
    this.gain.disconnect();
    this.source.disconnect();
    this.gain = null;
    this.source = null;
  }
}