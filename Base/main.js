const {
  app,
  BrowserWindow
} = require('electron')
const path = require('path')

var last_data = "Put to sleep";
var ndata = false

function createWindow() {
  const mainWindow = new BrowserWindow({
    width: 1280,
    height: 720,
    frame: false,
    webPreferences: {
      webviewTag: true,
      nodeIntegration: true,
    }
  })

  mainWindow.loadFile('app/index.html')
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
  last_data = "Connected";
  console.log(socket.id);
  connected.push(socket.id);
  ndata = true

  // ANCHOR Login 
  socket.on("login", function (data) {
    console.log(data);
    socket.emit("login", data);
    ndata = true
  });
  socket.on("data", function (data) {
    console.log(data);
    last_data = data;
    ndata = true
  });

  socket.on("disconnect", function () {
    var i = connected.indexOf(socket.id);
    console.log("Disconnected");
    connected.pop(i);
    last_data = "Disconnected";
    ndata = true
  });
});

const {
  ipcMain
} = require('electron')
ipcMain.on('pingdata', (event, arg) => {
  if (ndata) {
    ndata = false;
    event.reply('pingdata', last_data);
  }
})

ipcMain.on('commbuts', (event, arg) => {
  console.log(arg);
  if (connected.length > 0) {
    io.to(connected[0]).emit('phase', arg);
  }
})