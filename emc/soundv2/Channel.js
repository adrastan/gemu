class Channel {
  constructor(ctx) {
    this.ctx = ctx;
    this.buffer = [];
    this.start = null;
    this.sources = [];
    this.currentlyPlaying = [];
  }

  endFrame() {
    this.play();
  }

  updateDocument(message) {
    let el = document.getElementById(this.channelName);
    if (el) {
      el.innerHTML = this.channelName + " " + message;
    }
  }

  play() {
    if (!this.sources.length) {
      return;
    }

    let ct = this.ctx.currentTime;
    this.start = this.start || ct;
    
    var removeSource = () => {
      this.currentlyPlaying.shift();
      if (this.currentlyPlaying.length == 0) {
        this.playing = false;
        this.updateDocument("stopped")
      }
    }

    while (this.sources.length) {
      let source = this.sources.shift();
      source.start(this.start);
      this.start += source.buffer.duration / source.playbackRate.value;
      this.currentlyPlaying.push(source);
      this.updateDocument("playing")
      this.playing = true;
      source.onended = () => {
        removeSource();
      }
    }
  }

  createBuffer() {
    let source = this.ctx.createBufferSource();
    let buffer = this.ctx.createBuffer(1, this.buffer.length, this.ctx.sampleRate);
    buffer.copyToChannel(new Float32Array(this.buffer), 0, 0);
    source.buffer = buffer;
    source.playbackRate.value = SOURCE_SAMPLE_RATE;
    source.connect(this.ctx.destination);
    this.sources.push(source);
  }
}