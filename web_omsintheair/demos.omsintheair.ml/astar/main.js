var cols = 100;
var rows = 100;

var grid;
var openSet = [];
var closedSet = [];
var startx = Math.floor(Math.random() * cols);
var starty = Math.floor(Math.random() * rows);
var start;
var endx = Math.floor(Math.random() * cols);
var endy = Math.floor(Math.random() * rows);
var end;
var path = [];

var done = false;

var white = "#fff"
var red = "#f9acac"
var green = "#6eff95"
var blue = "#6ed0ff"
var black = "#000"
var sred = "#f00"



function calcH(a, b) {
    return Math.sqrt(Math.pow((a - end.x), 2) + Math.pow((b - end.y), 2));
}

function removeFromArray(variable, object) {
    for (var i = variable.length - 1; i >= 0; i--) {
        if (variable[i] == object) {
            variable.splice(i, 1)
        }
    }
    return variable
}

function Spot(x, y, wall) {
    this.x = x;
    this.y = y;

    this.f = 0.0;
    this.g = 0.0;
    this.h = 0.0;

    this.previous = undefined;

    this.wall = wall;

    this.vesinos = []

    this.show = function (color) {
        ctx.beginPath()
        ctx.fillStyle = color;
        ctx.fillRect(this.x * sc, this.y * sr, sc, sr);
        ctx.stroke()
    }

    this.addVesinos = function () {
        if (this.previous != undefined) {

        } else {
            this.vesinos = []
            if (this.x > 0) {
                this.vesinos.push(grid[this.x - 1][this.y])
            }
            if (this.x < cols - 1) {
                this.vesinos.push(grid[this.x + 1][this.y])
            }
            if (this.y > 0) {
                this.vesinos.push(grid[this.x][this.y - 1])
            }
            if (this.y < rows - 1) {
                this.vesinos.push(grid[this.x][this.y + 1])
            }
            if (x > 0 && y > 0) {
                this.vesinos.push(grid[this.x - 1][this.y - 1]);
            }
            if (x < cols - 1 && y > 0) {
                this.vesinos.push(grid[this.x + 1][this.y - 1]);
            }
            if (x > 0 && y < rows - 1) {
                this.vesinos.push(grid[this.x - 1][this.y + 1]);
            }
            if (x < cols - 1 && y < rows - 1) {
                this.vesinos.push(grid[this.x + 1][this.y + 1]);
            }
        }
    }
}

function arraySetup() {
    grid = new Array(cols)
    for (var x = 0; x < cols; x++) {
        grid[x] = new Array(rows);
        for (var y = 0; y < rows; y++) {
            if (Math.random() < 0.2 && (y != starty || x != startx) && (y != endy || x != endx)) {
                var wall = true;
            } else {
                var wall = false;
            }
            grid[x][y] = new Spot(x, y, wall)
        }
    }

    start = grid[startx][starty];
    end = grid[endx][endy];


    for (var x = 0; x < cols; x++) {
        for (var y = 0; y < rows; y++) {
            grid[x][y].h = calcH(x, y);
            grid[x][y].f = grid[x][y].h;
            grid[x][y].addVesinos();
        }
    }
}

var c = document.getElementById("canvas");
var ctx = c.getContext("2d");

c.width = screen.width;
c.height = screen.height;

sc = c.width / cols;
sr = c.height / rows;

arraySetup();
console.log("Start: ", startx, starty, " End: ", endx, endy)

openSet.push(start);


function Step() {
    if (openSet.length > 0) {
        var lowest = 0
        for (var i = 0; i < openSet.length; i++) {
            if (openSet[i].f < openSet[lowest].f) {
                lowest = i
            }
        }

        var current = openSet[lowest]
        openSet = removeFromArray(openSet, current)
        closedSet.push(current);

        if (current == end) {
            console.log("Done");
            done = true;
        }

        var vesinos = current.vesinos;
        for (var i = 0; i < vesinos.length; i++) {
            var vesino = vesinos[i];
            if (!closedSet.includes(vesino) && !vesino.wall) {
                if (vesino.x == current.x || vesino.y == current.y) {
                    var tempg = current.g + 1;
                } else {
                    var tempg = current.g + 1.4142135;
                }
                var newPath = false
                if (openSet.includes(vesino)) {
                    if (tempg > vesino.g) {
                        vesino.g = tempg;
                        newPath = true;
                    }
                } else {
                    vesino.g = tempg;
                    newPath = true;
                    openSet.push(vesino);
                }
                if (newPath) {
                    vesino.f = vesino.g + vesino.h;
                    vesino.previous = current;
                }
            }
        }


    } else {
        // no solution
        done = true;
        console.log("No solution");
    }
    var temp = current;
    var path = []
    path.push(temp)
    while (temp.previous) {
        path.push(temp.previous)
        temp = temp.previous;
    }
    // refresh color
    for (var i = 0; i < cols; i++) {
        for (var j = 0; j < rows; j++) {
            if (grid[i][j].wall) {
                grid[i][j].show(black)
            } else {
                grid[i][j].show(white)
            }
        }
    }
    for (var i = 0; i < openSet.length; i++) {
        openSet[i].show(green)
    }
    for (var i = 0; i < closedSet.length; i++) {
        closedSet[i].show(red)
    }
    for (var i = 0; i < path.length; i++) {
        path[i].show(blue)
    }
    start.show(sred)
    end.show(sred)

    document.getElementById("start").innerHTML = "X: " + Math.floor(start.h) + " Y: " + Math.floor(start.y);
    document.getElementById("end").innerHTML = "X: " + Math.floor(end.h) + " Y: " + Math.floor(end.y);
    document.getElementById("OpenSet").innerHTML = openSet.length;
    document.getElementById("ClosedSet").innerHTML = closedSet.length;
    document.getElementById("path").innerHTML = path.length;
    document.getElementById("current").innerHTML = "X: " + Math.floor(current.h, 2) + " Y: " + current.y;
    document.getElementById("operations").innerHTML = operations;
    document.getElementById("time").innerHTML = Math.floor(Date.now() - starttime, 2) + "ms";
}
var operations = 0;
var starttime = Date.now()
setInterval(function () {
    if (!done) {
        Step()
        operations++;
    }
}, 0);

function reset() {
    operations = 0;
    starttime = Date.now();
    openSet = [];
    closedSet = [];
    startx = Math.floor(Math.random() * cols);
    starty = Math.floor(Math.random() * rows);
    start;
    endx = Math.floor(Math.random() * cols);
    endy = Math.floor(Math.random() * rows);
    end;
    arraySetup();
    console.log("Start: ", startx, starty, " End: ", endx, endy)
    openSet.push(start);
    done = false;

}