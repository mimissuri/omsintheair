var express = require('express');
var app = express();
var server = require('http').createServer(app);
var io = require('socket.io')(server);

var airplane = {
    "pi": 0,
    "local_time": 0,

    "m_l": 0,
    "m_r": 0,

    "s0": 0,
    "s1": 0,
    "s2": 0,
    "s3": 0,
    "s4": 0,
    "s_l": 0,
    "s_r": 0,

    "accx_m1": 0,
    "accy_m1": 0,
    "accz_m1": 0,
    "gyrox_m1": 0,
    "gyroy_m1": 0,
    "gyroz_m1": 0,
    "pitch_m1": 0,
    "roll_m1": 0,
    "yaw_m1": 0,

    "accx_m2": 0,
    "accy_m2": 0,
    "accz_m2": 0,
    "gyrox_m2": 0,
    "gyroy_m2": 0,
    "gyroz_m2": 0,
    "pitch_m2": 0,
    "roll_m2": 0,
    "yaw_m2": 0,

    "accx_m3": 0,
    "accy_m3": 0,
    "accz_m3": 0,
    "gyrox_m3": 0,
    "gyroy_m3": 0,
    "gyroz_m3": 0,
    "magx_m3": 0,
    "magy_m3": 0,
    "magz_m3": 0,
    "pitch_m3": 0,
    "roll_m3": 0,
    "yaw_m3": 0,

    "pitch_gi": 0,
    "roll_gi": 0,
    "yaw_gi": 0,

    "lat": 0,
    "long": 0,
    "alt": 0,
    "speed": 0,
    "sats_used": 0,
    "gps_state": 0,
};

var client = {
    "mode": 0,  //0
    "m_l": 0,   //1
    "m_r": 0,   //2
    "s0": 0,    //3
    "s1": 0,    //4
    "s2": 0,    //5
    "s3": 0,    //6
    "s4": 0,    //7
    "s_l": 0,   //8
    "s_r": 0,   //9
};

apps.get("/plane", function (req, res) {
    res.send(client.mode + "," +
        client.m_l + "," +
        client.m_r + "," +
        client.s0 + "," +
        client.s1 + "," +
        client.s2 + "," +
        client.s3 + "," +
        client.s4 + "," +
        client.s_l + "," +
        client.s_r);
    res.end();

    var data = req._parsedUrl.query.split(";");
    airplane["pi"] = data[0];

    airplane["m_l"] = data[8];
    airplane["m_r"] = data[9];

    airplane["s0"] = data[10];
    airplane["s1"] = data[11];
    airplane["s2"] = data[12];
    airplane["s3"] = data[13];
    airplane["s4"] = data[14];
    airplane["s_l"] = data[15];
    airplane["s_r"] = data[16];

    airplane["accx_m1"] = data[17];
    airplane["accy_m1"] = data[18];
    airplane["accz_m1"] = data[19];
    airplane["gyrox_m1"] = data[20];
    airplane["gyroy_m1"] = data[21];
    airplane["gyroz_m1"] = data[22];
    airplane["pitch_m1"] = data[23];
    airplane["roll_m1"] = data[24];
    airplane["yaw_m1"] = data[25];

    airplane["accx_m2"] = data[26];
    airplane["accy_m2"] = data[27];
    airplane["accz_m2"] = data[28];
    airplane["gyrox_m2"] = data[29];
    airplane["gyroy_m2"] = data[30];
    airplane["gyroz_m2"] = data[31];
    airplane["pitch_m2"] = data[32];
    airplane["roll_m2"] = data[33];
    airplane["yaw_m2"] = data[34];

    airplane["accx_m3"] = data[35];
    airplane["accy_m3"] = data[36];
    airplane["accz_m3"] = data[37];
    airplane["gyrox_m3"] = data[38];
    airplane["gyroy_m3"] = data[39];
    airplane["gyroz_m3"] = data[40];
    airplane["magx_m3"] = data[41];
    airplane["magy_m3"] = data[42];
    airplane["magz_m3"] = data[43];
    airplane["pitch_m3"] = data[44];
    airplane["roll_m3"] = data[45];
    airplane["yaw_m3"] = data[46];

    airplane["pitch_gi"] = data[47];
    airplane["roll_gi"] = data[48];
    airplane["yaw_gi"] = data[49];

    airplane["lat"] = data[2];
    airplane["long"] = data[3];
    airplane["alt"] = data[4];
    airplane["speed"] = data[5];
    airplane["sats_used"] = data[6];
    airplane["gps_state"] = data[7];

    console.log("Received packet: " + airplane["pi"]);
    io.emit("planedata", airplane);
});

app.use("/", express.static(__dirname + '/app'));

server.listen(process.env.PORT || 80);

io.on("connect", function (socket) {
    console.log("New socket connection: " + socket.id);
    socket.on("data", function (data) {
        client = data;
    });

    socket.on("disconnect", function () {
        console.log("Disconnected: " + socket.id);
    });
    socket.on("datatoplane", function (data) {
        client = data;
    });
    socket.on("commbuts", function (data) {
        client["mode"] = data;
    })
});