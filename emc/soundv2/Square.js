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
  }

  updateCycles(cycles) {
    this.dutyTimer -= cycles;
    if (this.dutyTimer <= 0) {
      this.advanceDuty();
      this.dutyTimer += (2048 - this.frequency) * 4;
    }
  }

  endFrame() {
    
  }

  getAmplitude() {
    if (!this.dacEnabled) return 0;
    let amp = this.dutyTable[this.waveDuty || 0][this.currentDuty || 0];
    amp = amp * (this.volume / 15);
    return amp;
  }

  advanceDuty() {
    if (!this.dutyEnabled) return;
    this.currentDuty++;
    if (this.currentDuty === 8) {
      this.currentDuty = 0;
    }
  }

  clockLength() {

  }

  clockEnvelope() {
    if (this.envelopePeriod) {
      if (this.envelopeTimer > 0) {
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
  }

  //4194304
  trigger() {
    this.dutyEnabled = true;
    this.dutyTimer = (2048 - this.frequency) * 4;
    if (!this.soundLength) {
      this.soundLength = 64;
    }
    this.envelopeTimer = (this._NR12 & 0x07) || 8;
    if (this.frame === 7) {
      this.envelopeTimer++;
    }
    this.volume = this._NR12 >> 4;
  }

  stop() {
    
  }
}