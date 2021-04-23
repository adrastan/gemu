class SoundController {
  constructor() {
    this.ctx = new (window.AudioContext || window.webkitAudioContext)();
    this.channel1 = new Channel1(this.ctx);
    this.channel2 = new Channel2(this.ctx);
    this.channel3 = new Channel3(this.ctx);
    this.channel4 = new Channel4(this.ctx);
    this.frame = 0;
    this.cycles = 0;
  }

  update(cycles) {
    this.cycles += cycles;
    if (this.cycles >= 8192) {
      this.clockFrame();
    }
  }

  clockFrame() {
    this.cycles -= 8192;
    this.frame++;
    if (this.frame % 2 == 1) {
      this.channel2.clockLength();
    }
    if (this.frame == 3 || this.frame == 7) {
      // this.channel2.clockSweep();
    }
    if (this.frame == 8) {
      this.channel2.clockEnvelope();
      this.frame = 0;
    }
  }

  write(address, byte) {
    if (address >= 0xff10 && address <= 0xff14) {
      this.channel1.update(address, byte);
    } else if (address >= 0xff16 && address <= 0xff19) {
      this.channel2.update(address, byte);
    } else if ((address >= 0xff1a && address <= 0xff1e) || (address >= 0xff30 && address <= 0xff3f)) {
      this.channel3.update(address, byte);
    } else if (address >= 0xff20 && address <= 0xff23) {
      this.channel4.update(address, byte);
    } else if (address >= 0xff24 && address <= 0xff26) {
      this.updateController(address, byte);
    }
  }

  updateController(address, byte) {
    if (address === 0xff24) {
      this.SO1Volume = byte & 0x07;
      this.SO2Volume = (byte >> 4) & 0x07;
    } else if (address === 0xff25) {
      this.updateChannelOutput(byte);
    } else if (address === 0xff26) {
      this.updateMasterControl(byte);
    }
  }

  updateMasterControl(byte) {
    if (!isSet(byte, 7)) {
      this.enabled = false;
      this.clearRegisters();
    } else {
      this.enabled = true;
    }
  }

  updateChannelOutput(byte) {
    this.channel1.SO1 = isSet(byte, 0);
    this.channel2.SO1 = isSet(byte, 1);
    this.channel3.SO1 = isSet(byte, 2);
    this.channel4.SO1 = isSet(byte, 3);
    this.channel1.SO2 = isSet(byte, 4);
    this.channel2.SO2 = isSet(byte, 5);
    this.channel3.SO2 = isSet(byte, 6);
    this.channel4.SO2 = isSet(byte, 7);
  }

  clearRegisters() {
    for (let i = 0xff10; i <= 0xf3f; ++i) {
      memory[i] = 0;
    }
  }

  endFrame() {
    
  }
}

class Channel1 {
  update(address, byte) {
    if (address == 0xff10) {

    }
  }
}

class Channel2 {
  constructor(ctx) {
    this.ctx = ctx;
    this.enabled = false;
    this.lengthEnabled = false;
    this.soundLength = 0;
  }

  // 00000000
  update(address, byte) {
    if (address == 0xff16) {
      this.waveDuty = (byte >> 6) & 3;
      this.soundLength = byte & 0x3f;
    }
    if (address == 0xff17) {
      this.volume = (byte >> 4) & 0x0f;
      this.increaseVolume = isSet(byte, 3);
      this.envelopePeriod = byte & 7;
      // this.envelopeDuration = (byte & 7) * (1 / 64) //seconds
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
      this.restart();
    }
    this.lengthEnabled = isSet(byte, 6);
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
    if (this.increaseVolume && this.envelopeVolume < 15) {
      this.envelopeVolume++;
    } else if (!this.increaseVolume && this.envelopeVolume > 0) {
      this.envelopeVolume--;
    }
  }

  disable() {
    this.enabled = false;
    this.stop();
  }

  play() {
    // let gain = this.ctx.createGain();
    if (this.osc) {
      this.osc.stop();
    }
    this.osc = this.ctx.createOscillator();
    this.osc.frequency.setValueAtTime(this.frequency, this.ctx.currentTime);
    this.osc.type = 'square';
    // gain.gain.value = this.envelopeVolume / 15;
    // gain.connect(this.ctx.destination);
    this.osc.connect(this.ctx.destination);

    this.osc.start();
  }

  stop() {
    console.log("stopping audio");
    this.osc.stop();
  }
  
  restart() {
    console.log("restart channel 2");
    this.enabled = true;
    if (this.soundLength == 0) {
      this.soundLength = 64;
    }
    this.frequency = this.getFrequency();
    this.envelopeTimer = (memory[0xff17] & 7) * (1 / 64);
    this.envelopeVolume = (memory[0xff17] >> 4) & 0x0F;
    console.log(this.frequency)
    this.play();

    // this.osc = this.ctx.createOscillator();
    // this.osc.frequency.setValueAtTime(this.frequency, this.ctx.currentTime);
    // this.osc.type = "square";
    // this.osc.connect(this.ctx.destination);
    // this.osc.start();
  }

  getFrequency() {
    let lowerFreq = memory[0xff18];
    let higherFreq = memory[0xff19];

    higherFreq = (0x07 & higherFreq) << 8;
    let frequency = higherFreq | lowerFreq;

    return 131071 / (2048 - frequency);
  }
}

class Channel3 {
  update(address, byte) {

  }
}

class Channel4 {
  update(address, byte) {

  }
}