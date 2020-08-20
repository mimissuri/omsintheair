var caliblx = 0;
var calibly = 0;
var calibrx = 0;
var calibry = 0;
var lx = 0;
var ly = 0;
var rx = 0;
var ry = 0;
window.addEventListener('gamepadconnected', (event) => {
    const update = () => {
        var gamepads = navigator.getGamepads();
        var gp = gamepads[0];
        var b_x = gp.buttons[0];
        var b_r = gp.buttons[1];
        var b_c = gp.buttons[2];
        var b_t = gp.buttons[3];
        var l1 = gp.buttons[4];
        var l2 = gp.buttons[6];
        var r1 = gp.buttons[5];
        var r2 = gp.buttons[7];
        var up = gp.buttons[12];
        var down = gp.buttons[13];
        var left = gp.buttons[14];
        var right = gp.buttons[15];
        var start = gp.buttons[8];
        var select = gp.buttons[9];

        var aLX = (gp.axes[0] - caliblx).toFixed(1)
        var aLY = (gp.axes[1] - calibly).toFixed(1)

        console.log(aLY);
        var aRX = gp.axes[2].toFixed(2) - calibrx
        var aRY = gp.axes[3].toFixed(2) - calibry
        rx += +aRX;
        //Right buttons
        if (b_x.pressed) {
            console.log("X pressed!");
        }
        if (b_c.pressed) {
            console.log("c pressed!");
        }
        if (b_t.pressed) {
            console.log("t pressed!");
        }
        if (b_r.pressed) {
            console.log("r pressed!");
        }
        //Els de adal
        if (r1.pressed) {
            ly += +aLY * -5;
            if (ly < 0) {
                ly = 0
            }
            if (ly > 100) {
                ly = 100
            }
            aLX = 0
        }
        if (r2.pressed) {
            aRY = 0;
        }
        if (l1.pressed) {
            ly += +aLY * -5;
            if (ly < 0) {
                ly = 0
            }
            if (ly > 100) {
                ly = 100
            }
        }
        if (l2.pressed) {
            aRX = 0;
        }
        //Left buttons

        if (up.pressed) {
            console.log("up pressed!");
        }
        if (down.pressed) {
            console.log("down pressed!");
        }
        if (left.pressed) {
            console.log("left pressed!");
        }
        if (right.pressed) {
            console.log("right pressed!");
        }
        //Center buttons
        if (b_t.pressed) {
            console.log("Calibrating");
            caliblx += +aLX;
            calibly += +aLY;
            calibrx += +aRX;
            calibry += +aRY;
        }
        if (select.pressed) {
            console.log("select pressed!");
        }
        if (aRY > 0) {
            aRX = aRX * -1
        }
        var frontspeedh = document.getElementById("palo").style.height;
        document.getElementById("rightmotor").style.height = ly + "%";
        document.getElementById("leftmotor").style.height = ly + "%";

        document.getElementById("waybar").style.width = Math.abs(aLX) * 50 + "%";
        document.getElementById("waybar").style.marginLeft = (aLX) * 50 + "%";

        document.getElementById("brujula").style.transform = "Rotate(" + aRX * 90 + "deg)";
        document.getElementById("frontspeed").style.height = Math.abs(aRY) * 50 + "%";
        document.getElementById("frontspeed").style.marginBottom = (aRY) * -0.5 * 7 + "vw";
        requestAnimationFrame(update);
    };
    update();
});