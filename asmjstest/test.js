m = require('./asmjstest.js');
var testBuf = Uint8Array(1024);
var oBuf = Uint8Array(1024 * 8);
m["decompress_buffer"](m, testBuf, oBuf)
