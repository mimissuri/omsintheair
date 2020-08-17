const {
    emit
} = require('process');

const server = require('http').createServer();

const io = require('socket.io')(server, {
    path: '/',
    serveClient: false,
    pingInterval: 10000,
    pingTimeout: 5000,
    cookie: false
});

var connected = [];
var logged = [];

server.listen(3000, function () {
    console.log("Server started");
});

io.on("connect", function (socket) {
    var i = 0;
    console.log(socket.id);
    connected.push(socket.id);

    // ANCHOR Login 
    socket.on("login", function (data) {
        console.log(data);
        socket.emit("login", data);
    });
    socket.on("data", function (data) {
        i++
        console.log(data);
        if (i == 100) {
            socket.emit("phase", 2);
        }
    });

    socket.on("disconnect", function () {
        var i = connected.indexOf(socket.id);
        console.log("Disconnected");
        connected.pop(i);
    });
});

const readline = require('readline');

const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

rl.on('line', (input) => {
    io.to(connected[0]).emit('phase', input);
});