window.$ = window.jQuery = require('jquery');
// ANCHOR MAPAAA THIIINGS
var latitud = 41.716379;
var longitude = 1.8220914;
$.get("http://ip-api.com/json", function (data, status) {
    console.log(data);
    latitud = data["lat"];
    longitude = data["lon"];
});
//Map
var ico_l = L.icon({
    iconUrl: 'map-plane.png',
    iconSize: [30, 30],
});
var mymap = L.map('map_id').setView([latitud, longitude], 14);
L.tileLayer('https://{s}.basemaps.cartocdn.com/light_all/{z}/{x}/{y}{r}.png', {
    attribution: 'OmsAndMoscas',
    maxZoom: 18,
    id: 'mapbox.streets',
    style: "grayscale"
}).addTo(mymap);

marker = L.marker([latitud, longitude], {
    icon: ico_l
}).addTo(mymap);
var marker;
//https://github.com/bbecquet/Leaflet.RotatedMarker INSTALAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAR
setInterval(() => {
    mymap.removeLayer(marker);
    latitud = latitud + 0.0001;
    longitude = longitude + 0.0001;
    marker = L.marker([latitud, longitude], {
        rotationAngle: 180,
        icon: ico_l
    }).addTo(mymap);
}, 1000);