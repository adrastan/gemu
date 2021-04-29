class SoundController {
  constructor() {
    this.ctx = new (window.AudioContext || window.webkitAudioContext)();
    this.channel1 = new Square1(this.ctx);
    this.channel2 = new Square2(this.ctx);
    this.channel3 = new Wave(this.ctx);
    this.channel4 = new Noise(this.ctx);
    this.frame = 0;
    this.cycles = 0;
  }

  restart() {
    this.channel1.disable();
    this.channel2.disable();
    this.frame = 0;
    this.cycles = 0;
  }

  updateCycles(cycles) {
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
      this.channel1.clockSweep();
    }
    if (this.frame == 8) {
      this.channel2.clockEnvelope();
      this.frame = 0;
    }
  }

  write(address, byte) {
    if (address >= 0xff10 && address <= 0xff14) {
      if (this.enabled) {
        this.channel1.update(address, byte);
      }
    } else if (address >= 0xff16 && address <= 0xff19) {
      if (this.enabled) {
        this.channel2.update(address, byte);
      }
    } else if ((address >= 0xff1a && address <= 0xff1e) || (address >= 0xff30 && address <= 0xff3f)) {
      // this.channel3.update(address, byte);
    } else if (address >= 0xff20 && address <= 0xff23) {
      // this.channel4.update(address, byte);
    } else if (address >= 0xff24 && address <= 0xff26) {
      this.updateController(address, byte);
    }
  }

  updateController(address, byte) {
    if (address === 0xff24) {
      this.SO1Volume = byte & 0x07;
      this.SO2Volume = (byte >> 4) & 0x07;
      console.log("Terminal 1 volume: ", this.SO1Volume);
      console.log("Terminal 2 volume: ", this.SO2Volume);
    } else if (address === 0xff25) {
      this.updateChannelOutput(byte);
    } else if (address === 0xff26) {
      this.updateMasterControl(byte);
    }
  }

  updateMasterControl(byte) {
    if (!isSet(byte, 7)) {
      this.powerOff();
      this.clearRegisters();
      console.log("Disable all sound.");
    } else {
      this.powerOn();
      console.log("Enable all sound.");
    }
  }

  powerOn() {
    this.enabled = true;
    this.channel1.enable();
    this.channel2.enable();
    this.frame = 0;
    this.cycles = 0;
  }

  powerOff() {
    this.enabled = false;
    this.channel1.disable();
    this.channel2.disable();
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


class Wave {
  update(address, byte) {

  }
}

class Noise {
  update(address, byte) {

  }
}