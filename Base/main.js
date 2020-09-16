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