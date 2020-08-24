// Importem la llibreria IPC per la comunicació entre el frontend i el backend
const {
    ipcRenderer
} = require('electron')

// Creem un interval que demanara dades al backend cada 50ms
setInterval(function () {
    ipcRenderer.send("pingdata", "demanding_data");
}, 50);

// Quan revem la resposta les mostrem a l'usuari
ipcRenderer.on('pingdata', (event, arg) => {
    document.getElementById("logs").innerHTML = "<div>" + arg + "</div>" + document.getElementById("logs")
        .innerHTML;
});
ipcRenderer.on('videostream', (event, arg) => {
    document.getElementById("logs").style.display = "none";
    document.getElementById("imgstream").style.display = "block";
    document.getElementById("imgstream").src = "data:image/jpg;base64," + arg;
    console.log(arg)
});

//La següent funció s'encarrega de que quan es clica intro en el input envia l'ordre a el backend
var input = document.getElementById("input1");
input.addEventListener('keyup', function (e) {
    if (e.key === 'Enter' || e.keyCode === 13) {
        ipcRenderer.send('command', input.value);
        if (input.value == "stop_stream") {
            document.getElementById("logs").style.display = "block";
            document.getElementById("imgstream").style.display = "none";

        }
        input.value = "";
    }
});

function commbutf(value) {
    ipcRenderer.send('commbuts', value);
}