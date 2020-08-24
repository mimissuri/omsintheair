// Importem les llibreries que necessitarem per crear la GUI
const {
  app,
  BrowserWindow
} = require('electron')

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

// En el moment que esta preparat creara la finestre que sera visible per l'usuari
app.whenReady().then(() => {
  createWindow()
  app.on('activate', function () {
    if (BrowserWindow.getAllWindows().length === 0) createWindow()
  })
})

// Quan detecta que totes les finestres s'han tencat para tot el programa amb el servidor inclos
app.on('window-all-closed', function () {
  if (process.platform !== 'darwin') {
    app.quit()
  }
})


// Importem les llibreries necessaries per crear el servidor
const server = require('http').createServer();
const io = require('socket.io')(server, {
  path: '/',
});

// Creem les variables que necessitarem de forma global, és a dir que no nomes una funció la fara servir
var last_data = ""; // Conte les ultimes dades que s'han rebut
var ndata = false; // Ens diu si hi han arribat noves dades
var plane = ""; // Conte informació d'on esta conectat l'avió amb el seu identificador
var nfoto = false;
var stream_foto = "";


// Iniciem el servidor en el port 3000 ja que en linux del port 0 fins al 1024 necessita acces d'administrador
server.listen(3000, function () {
  // Imprimim que el servidor s'ha creat correctament
  console.log("Server started");
});


// Esperem per connexions al servidor, quan es conecta algú es cridara a la funció que hi ha a continuació
io.on("connect", function (socket) {
  // Actualitzem les variables globals
  last_data = "Connected";
  ndata = true;
  // Imprimim l'identificador del dispositiu conectat
  console.log(socket.id);

  // Per saber quin dispositiu es l'avió aquest ha d'iniciar sessió
  socket.on("login", function (data) {
    console.log("xd")
    if (data == "plane") {
      console.log("logged")
      plane = socket.id;
    }
  });

  // la següent funció es la que s'encarrega de rebre l'informació de l'avió i l'envia a l'usuari
  socket.on("data", function (data) {
    console.log(data);
    last_data = data;
    ndata = true;
  });
  socket.on("data", function (data) {
    console.log(data);
    last_data = data;
    ndata = true;
  });
  socket.on("video_stream", function (data) {
    nfoto = true;
    stream_foto = data;
  });

  // Detecta quan un dispositiu es conecta
  socket.on("disconnect", function () {
    console.log("Disconnected");
    last_data = "Disconnected";
    ndata = true;
  });
});

// Importem la llibreria per la comunicació IPC que ens permet comunicar el backend amb el frontend
const {
  ipcMain
} = require('electron');

// Funció periodica que envia l'ultima informació de l'avio
ipcMain.on('pingdata', (event, arg) => {
  if (ndata) {
    event.reply('pingdata', last_data);
    ndata = false;
  }
  if (nfoto) {
    nfoto = false
    event.reply('videostream', stream_foto);
  }
})

// Quan desde la GUI es clica un boto arriba fins aquí el qual s'encarrega d'enviar-ho a l'avió
ipcMain.on('commbuts', (event, arg) => {
  console.log(arg);
  if (plane.length > 0) {
    io.to(plane).emit('phase', arg);
  }
})

// Quan desde la GUI s'escriu una ordre
ipcMain.on('command', (event, arg) => {
  console.log(arg);
  if (plane.length > 0) {
    io.to(plane).emit('command', arg);
  }
})