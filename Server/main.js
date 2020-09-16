var express = require("express");
var app = express();
const request = require('request');

const options = {
    url: 'https://jsonplaceholder.typicode.com/posts',
    method: 'GET',
    headers: {
        'Accept': 'application/json',
        'Accept-Charset': 'utf-8',
    }
};

var airplane = {
    "packet_id": 0,
    "m1": 0,
    "m2": 0,
    "s1": 0,
    "s2": 0,
    "s3": 0,
    "s4": 0,
    "s5": 0,
    "s6": 0,
    "s7": 0,
    "lat": 0,
    "long": 0,
    "alt": 0,
};

var client = {
    "phase": 0,
    "m1": 0,
    "m2": 0,
    "s1": 0,
    "s2": 0,
    "s3": 0,
    "s4": 0,
    "s5": 0,
    "s6": 0,
    "s7": 0,
};

app.get("/plane", function (req, res) {
    res.json(client);
    res.end();
    airplane["packet_id"] = req.query.paquet_id;
    airplane["m1"] = req.query.m1;
    airplane["m2"] = req.query.m2;
    airplane["s1"] = req.query.s1;
    airplane["s2"] = req.query.s2;
    airplane["s3"] = req.query.s3;
    airplane["s4"] = req.query.s4;
    airplane["s5"] = req.query.s5;
    airplane["s6"] = req.query.s6;
    airplane["s7"] = req.query.s7;
    airplane["lat"] = req.query.lat;
    airplane["long"] = req.query.long;
    airplane["alt"] = req.query.alt;
    console.log(airplane);
    io.emit("planedata", airplane);
});
app.get("*", function (req, res) {
    res.json({
        "ERROR": 404
    });
    res.end();
    console.log("???????????");
});

app.listen(8000, function () {
    console.log("My API is running...");
});




const socket_server = require('http').createServer();
const io = require('socket.io')(socket_server, {
    path: '/',
});


socket_server.listen(3000, function () {
    console.log("Servers started");
});

io.on("connect", function (socket) {
    console.log("Someone connected pog");
    socket.on("data", function (data) {

    });

    socket.on("disconnect", function () {
        console.log("Disconnected");
    });
});