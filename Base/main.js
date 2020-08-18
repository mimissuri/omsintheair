const {
  app,
  BrowserWindow
} = require('electron')
const path = require('path')

function createWindow() {
  const mainWindow = new BrowserWindow({
    width: 800,
    height: 600,
    frame: false,
    webPreferences: {
      webviewTag: true,
      nodeIntegration: true,
    }
  })

  mainWindow.loadFile('app/index.html')
  mainWindow.setFullScreen(true);
}

app.whenReady().then(() => {
  createWindow()
  app.on('activate', function () {
    if (BrowserWindow.getAllWindows().length === 0) createWindow()
  })
})
app.on('window-all-closed', function () {
  if (process.platform !== 'darwin') app.quit()
})

const {
  emit
} = require('process');


// ANCHOR SERVER THINGS
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
const {
  NONAME
} = require('dns')

const rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout
});

rl.on('line', (input) => {
  io.to(connected[0]).emit('phase', input);
});