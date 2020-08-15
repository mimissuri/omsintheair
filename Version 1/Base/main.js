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

server.listen(3000);

io.on("connect", function (socket) {
    connected.push(socket.id);

    // ANCHOR Login 
    socket.on("login", function (data) {
        console.log(data);
        socket.emit("login", data);
    });

    socket.on("disconnect", function () {
        var i = connected.indexOf(socket.id);
        connected.pop(i);
    });
});