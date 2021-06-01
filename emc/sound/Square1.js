
class Square1 extends Square {
  constructor(ctx) {
    super(ctx);
    this.channelName = "channel1"
  }

  get NR10() { return this._NR10; };
  get NR11() { return this._NR11; };
  get NR12() { return this._NR12; };
  get NR13() { return this._NR13; };
  get NR14() { return this._NR14; };

  set NR10(value) {
    this._NR10 = value;
    this.sweepTime = (value >> 4) && 0x07;
    this.sweepIncrease = isSet(value, 3);
    this.sweepShift = value & 0x7;
    switch (this.sweepTime) {
      case 0: this.sweepDuration = 0; break;
      case 1: this.sweepDuration = 0.0078; break;
      case 2: this.sweepDuration = 0.0156; break;
      case 3: this.sweepDuration = 0.0234; break;
      case 4: this.sweepDuration = 0.0313; break;
      case 5: this.sweepDuration = 0.0391; break;
      case 6: this.sweepDuration = 0.0469; break;
      case 7: this.sweepDuration = 0.0547; break;
    }
  }

  set NR11(value) {
    this._NR11 = value;
    this.waveDuty = this.getWaveDuty((value >> 6));
    this.soundDuration = (64 - (value & 0x3F)) * (1 / 256); //seconds
    this.soundLength = (value & 0x3F);
  }

  set NR12(value) {
    this._NR12 = value;
    this.envelopeVolume = value >> 4;
    this.increaseVolume = isSet(value, 3);
    this.envelopeDuration = (value & 0x07) * (1 / 64); //seconds
    this.envelopePeriod = (value & 0x07);
    if ((value >> 3) == 0) {
      this.dacEnabled = false;
      this.disable();
    } else {
      this.dacEnabled = true;
    }
    this.updateGain();
  }

  set NR13(value) {
    this._NR13 = value;
    this.frequency = this.getFrequency(value, this._NR14);
    this.updateFrequency();
  }

  set NR14(value) {
    this._NR14 = value;
    this.lengthEnabled = isSet(value, 6);
    if (isSet(value, 7) && this.dacEnabled) {
      this.trigger();
    }
    this.frequency = this.getFrequency(this._NR13, value);
    this.updateFrequency();
  }

  clear() {
    this.NR10 = 0;
    this.NR11 = 0;
    this.NR12 = 0;
    this.NR13 = 0;
    this.NR14 = 0;
  }

  clockSweep() {

  }
}