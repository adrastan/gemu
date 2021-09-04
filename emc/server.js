const express = require("express");
const http = require("http");
const app = express();

app.use(express.static(__dirname + "/", { index: "gemu.html" }));

http.createServer(app).listen(12345, "0.0.0.0");