import logo from './logo.svg';
import './App.css';
import React from "react";

import * as Main from "./main"
import SoundController from './sound/SoundController';

function App() {
  function fileLoaded(e) {
    console.log(e.target.files[0])
    if (!window.audio) {
      window.audio = new SoundController();
    }
    e.target.files[0].arrayBuffer().then(data => {
      console.log(data);
      Main.loadCart(data);
    })
  }
  
  React.useEffect(() => {
    const script = document.createElement("script");
    script.src = 'gemu.js'; // whatever url you want here
    script.async = true;
    document.head.appendChild(script);
    console.log(Main)
    Main.initialiseCanvas();
    for (let prop in Main) {
      window[prop] = Main[prop]
    }
    document.addEventListener("keydown", function (e) {
      if (e.keyCode === 13) { //START
        window.Module._keyPress(7);
      }
      if (e.keyCode === 38) { //UP
        window.Module._keyPress(2);
      }
      if (e.keyCode === 39) { //RIGHT
        window.Module._keyPress(0);
      }
      if (e.keyCode === 40) { //DOWN
        window.Module._keyPress(3);
      }
      if (e.keyCode === 37) { //LEFT
        window.Module._keyPress(1);
      }
      if (e.keyCode === 90) { //Z
        window.Module._keyPress(5);
      }
      if (e.keyCode === 88) { //X
        window.Module._keyPress(4);
      }
      if (e.keyCode === 16) { //RSHIFT
        window.Module._keyPress(6);
      }
      if (e.keyCode === 82) { //R
        window.restart();
      }
    })

    document.addEventListener("keyup", function (e) {
      if (e.keyCode === 13) {
        window.Module._keyRelease(7);
      }
      if (e.keyCode === 38) {
        window.Module._keyRelease(2);
      }
      if (e.keyCode === 39) {
        window.Module._keyRelease(0);
      }
      if (e.keyCode === 40) {
        window.Module._keyRelease(3);
      }
      if (e.keyCode === 37) {
        window.Module._keyRelease(1);
      }
      if (e.keyCode === 90) {
        window.Module._keyRelease(5);
      }
      if (e.keyCode === 88) {
        window.Module._keyRelease(4);
      }
      if (e.keyCode === 16) {
        window.Module._keyRelease(6);
      }
    })
  },[])

  return (
    <div className="App">
      <input type="file" onChange={fileLoaded} />
      <canvas id="canvas" className="canvas"></canvas>
    </div>
  );
}

export default App;
