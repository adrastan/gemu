
let db = new Dexie("games");
db.version(1).stores({
  game: 'name'
});
db.open();

document.getElementById("canvas").addEventListener("webglcontextlost", (e) => {
  alert('WebGL context lost. You will need to reload the page.'); 
  e.preventDefault(); 
})

var loadSave = async function() {
  let title = "";
  for (let i = 0x134; i <= 0x143; ++i) {
    title += String.fromCharCode(memory[i]);
  }
  window.game = await db.game.get(title);
  if (!window.game) {
    await db.game.put({ name: title, ram: new Uint8Array(32768) });
    window.game = await db.game.get(title);
  }
  console.log(title, game);
  for (let i = 0; i < 32768; ++i) {
    cartRam[i] = game.ram[i];
  }
}

var loadCart = async function(arr) {
  let buffer = new Uint8Array(arr);
  let j = 0;
  for (let i = 0; i < 0x10000; ++i, ++j) {
    memory[j] = buffer[i];
  }
  j = 0;
  for (let i = 0; i < arr.byteLength; ++i, ++j) {
    cartRom[j] = buffer[i];
  }
  await loadSave();
  audio = new SoundController();
  Module._startCpu();
  fn();
}

document.getElementById("fileInput").addEventListener("change", (e) => {
  e.target.files[0].arrayBuffer().then(arr => {
    return loadCart(arr);
  }).catch(e => {
    console.error(e);
  })
})

var Module = {
  preRun: [],
  postRun: [],
  print(text) {
    console.log(text);
  },
  printErr: function(text) {
    if (arguments.length > 1) text = Array.prototype.slice.call(arguments).join(' ');
    console.error(text);
  },
  canvas: document.createElement("canvas")
};

let setBit = function(byte, n) {
  byte |= 1 << n;
  return byte;
}

let clearBit = function(byte, n) {
  byte &= ~(1 << n);
  return byte;
}

let getBit = function(byte, n) {
  return 0x01 & (byte >> n)
}

let isSet = function(byte, n) {
  return getBit(byte, n) === 1;
}

let getImageData = function() {
  Module._getNextFrame();
  let buffer = new Uint8ClampedArray(Module.HEAPU8.buffer, Module._getScreenSurfacePtr(), 69120);
  let bufIdx = 0;
  let pixelBuffer = new ArrayBuffer(92160);
  let view = new Uint8ClampedArray(pixelBuffer);
  for (let i = 0; i < view.length; i += 4) {
    view[i] = buffer[bufIdx];
    view[i + 1] = buffer[bufIdx + 1];
    view[i + 2] = buffer[bufIdx + 2];
    view[i + 3] = 255;
    bufIdx += 3;
  }
  return new ImageData(view, 160);
}

const CANVAS_WIDTH = 160;
const CANVAS_HEIGHT = 144;

let mainCanvas = document.getElementById("canvas");
mainCanvas.width = CANVAS_WIDTH;
mainCanvas.height = CANVAS_HEIGHT;
let smallCanvas = document.createElement("canvas");
smallCanvas.width = 160;
smallCanvas.height = 144;
let mainCtx = mainCanvas.getContext("2d");
let smallCtx = smallCanvas.getContext("2d");

let ram_changed = function(address, byte) {
  if (window.game) {
    window.game.ram[address] = byte;
    db.game.put(window.game);
  }
}

let write_memory = function(address, byte) {
  if (address >= 0xff10 && address <= 0xff3f) {
    audio.write(address, byte);
  }
}

let update_sound = function(cycles) {
  audio.updateCycles(cycles);
}

let frames = [];

let draw = function() {
  if (!frames.length) {
    window.nextFrame = requestAnimationFrame(draw);
    return;
  }
  smallCtx.putImageData(frames.shift(), 0, 0);
  mainCtx.imageSmoothingEnabled = false;
  mainCtx.drawImage(smallCanvas, 0, 0, 160, 144, 0, 0, CANVAS_WIDTH, CANVAS_HEIGHT);
  
  // audio.endFrame();
  window.nextFrame = requestAnimationFrame(draw);
}

let memory;

let fn = function() {
  running = true;
  setInterval(() => {
    if (!audio.channel1.sources.length && !audio.channel1.currentlyPlaying.length) {
      frames.push(getImageData());
    } else if (!frames.length && audio.channel1.currentlyPlaying.length == 1) {
      let imageData = getImageData();
      frames.push(imageData)
    }
    audio.play();
  }, 1)
  window.nextFrame = requestAnimationFrame(draw);
}

Module.onRuntimeInitialized = function() {
  memory = new Uint8Array(Module.HEAPU8.buffer, Module._getMemoryPtr(), 0x10000);
  cartRom = new Uint8Array(Module.HEAPU8.buffer, Module._getCartPtr(), 4194304);
  cartRam = new Uint8Array(Module.HEAPU8.buffer, Module._getRamPtr(), 32768);
};

document.getElementById("startTetris").addEventListener("click", async () => {
  window.cancelAnimationFrame(window.nextFrame);
  if (typeof audio === "undefined") {
    audio = new SoundController();
  } else {
    audio.restart();
  }
  Module._startTetris();
  await loadSave();
  fn();
})

document.getElementById("startMK2").addEventListener("click", async () => {
  window.cancelAnimationFrame(window.nextFrame);
  if (typeof audio === "undefined") {
    audio = new SoundController();
  } else {
    audio.restart();
  }
  Module._startMK2();
  await loadSave();
  fn();
})

document.getElementById("startDK").addEventListener("click", async () => {
  window.cancelAnimationFrame(window.nextFrame);
  if (typeof audio === "undefined") {
    audio = new SoundController();
  } else {
    audio.restart();
  }
  Module._startDK();
  await loadSave();
  fn();
})

var isTouchEventWithElement = function(e, element) {
  const item = e.changedTouches.item(0);
  if (element === null || item === null) return false;
  return element.getBoundingClientRect().right > item.clientX &&
      element.getBoundingClientRect().left < item.clientX &&
      element.getBoundingClientRect().top < item.clientY &&
      element.getBoundingClientRect().bottom > item.clientY;
}

document.addEventListener("keydown", function(e) {
  if (e.keyCode === 13) { //START
    Module._keyPress(7);
  }
  if (e.keyCode === 38) { //UP
    Module._keyPress(2);
  }
  if (e.keyCode === 39) { //RIGHT
    Module._keyPress(0);
  }
  if (e.keyCode === 40) { //DOWN
    Module._keyPress(3);
  }
  if (e.keyCode === 37) { //LEFT
    Module._keyPress(1);
  }
  if (e.keyCode === 90) { //Z
    Module._keyPress(5);
  }
  if (e.keyCode === 88) { //X
    Module._keyPress(4);
  }
  if (e.keyCode === 16) { //RSHIFT
    Module._keyPress(6);
  }
  if (e.keyCode === 82) { //R
    audio.restart();
    Module._restartPress();
  }
})

document.addEventListener("keyup", function(e) {
  if (e.keyCode === 13) {
    Module._keyRelease(7);
  }
  if (e.keyCode === 38) {
    Module._keyRelease(2);
  }
  if (e.keyCode === 39) {
    Module._keyRelease(0);
  }
  if (e.keyCode === 40) {
    Module._keyRelease(3);
  }
  if (e.keyCode === 37) {
    Module._keyRelease(1);
  }
  if (e.keyCode === 90) {
    Module._keyRelease(5);
  }
  if (e.keyCode === 88) {
    Module._keyRelease(4);
  }
  if (e.keyCode === 16) {
    Module._keyRelease(6);
  }
})