const {
    ipcRenderer
} = require('electron')

ipcRenderer.on('pingdata', (event, arg) => {
    document.getElementById("logs").innerHTML = "<div>" + arg + "</div>" + document.getElementById("logs")
        .innerHTML;
});
setInterval(function () {
    ipcRenderer.send("pingdata", "xd");
}, 50);

function commbutf(value) {
    ipcRenderer.send('commbuts', value);
}
var input = document.getElementById("input1");
input.addEventListener('keyup', function (e) {
    if (e.key === 'Enter' || e.keyCode === 13) {
        ipcRenderer.send('command', input.value);
        input.value = "";
    }
});