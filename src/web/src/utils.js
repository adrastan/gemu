let getBit = function(byte, n) {
  return 0x01 & (byte >> n)
}
let isSet = function(byte, n) {
  return getBit(byte, n) === 1;
}
let setBit = function(byte, n) {
  byte |= 1 << n;
  return byte;
}
let clearBit = function(byte, n) {
  byte &= ~(1 << n);
  return byte;
}

export { 
  isSet,
  setBit,
  clearBit,
  getBit
}