var mtop = 0
var way = 0
var w = window.innerWidth;
var h = window.innerHeight;
var patata = setInterval(function () {
    if (way == 0) {
        mtop++;
        if (mtop == 30) { way = 1 }
    } else {
        mtop--
        if (mtop == 0) { way = 0 }
    }
    if (w > h) {
        document.getElementById("flecheta").style.marginTop = mtop / 30 + "vh";
    } else {
        document.getElementById("flecheta").style.marginTop = mtop / 20 + "vw";
    }
}, 20)

window.addEventListener('scroll', function () {
    clearInterval(patata);
});