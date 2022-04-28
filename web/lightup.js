var g;
var nb_rows;
var nb_cols;
var wrapping;
Module.onRuntimeInitialized = () => { initGame(); }

const BLANK = 0;
const LIGHTBULB = 1;
const MARK = 2;
const MAX_SIZE = 10;
const MIN_SIZE = 3;

var canvas = document.getElementById("mycanvas");
var ctx = canvas.getContext('2d');

var div = document.getElementById("game");

var buttonsMenu = document.getElementById("buttons");

var mouseI = -1;
var mouseJ = -1;

function initGame() {
    console.log("Init game");
    nb_rows = Math.round((Math.random() * (MAX_SIZE - MIN_SIZE + 1) + MIN_SIZE));
    nb_cols = Math.round((Math.random() * (MAX_SIZE - MIN_SIZE + 1) + MIN_SIZE));
    wrapping = Math.round(Math.random());
    let nb_walls = Math.round(Math.random() * ((nb_cols * nb_rows) - (nb_cols * nb_rows) / 5));
    if (nb_walls < (nb_cols * nb_rows) / 5)
        nb_walls = (nb_cols * nb_rows) / 5;
    g = Module._new_random(nb_rows, nb_cols, wrapping ? true : false, nb_walls, false);
    printGame();
}

function drawWall(ctx, x, y, size, nb, err) {
    ctx.fillStyle = "#000000";
    ctx.fillRect(x, y, size, size);
    ctx.textAlign = "center";
    ctx.textBaseline = 'middle';
    ctx.font = size.toString(10) + 'px sans-serif';
    if (err)
        ctx.fillStyle = "#ff3737";
    else
        ctx.fillStyle = "#ffffff";
    switch (nb) {
        // 1.18 to compensate the cap height of numbers
        case 0:
            ctx.fillText("0", x + size / 2, y + size / 12 + size / 2);
            break;
        case 1:
            ctx.fillText("1", x + size / 2, y + size / 12 + size / 2);
            break;
        case 2:
            ctx.fillText("2", x + size / 2, y + size / 12 + size / 2);
            break;
        case 3:
            ctx.fillText("3", x + size / 2, y + size / 12 + size / 2);
            break;
        case 4:
            ctx.fillText("4", x + size / 2, y + size / 12 + size / 2);
            break;
        default:
            break;
    }
}

function solveGame() {
    Module._solve(g);
    printGame();
}

function undoMove() {
    Module._undo(g);
    printGame(g);
}

function redoMove() {
    Module._redo(g);
    printGame();
}

function restartGame() {
    Module._restart(g);
    printGame();
}

function newGame() {
    Module._delete(g);
    initGame();
    printGame(g);
    let button = document.getElementById("toEdit");
    button.setAttribute("value", "Nb Sol");
}

function nbsol() {
    let button = document.getElementById("toEdit");
    button.setAttribute("value", Module._nb_solutions(g));
}

function drawBlank(ctx, x, y, win, wrap, lighted, size, hover) {
    ctx.beginPath();
    let colour = [0, 0, 0];
    if (win)
        colour = [55, 150, 55];
    else if (lighted) {
        if (wrap)
            colour = [255, 190, 55];
        else
            colour = [255, 255, 55];
    }
    else
        colour = [255, 255, 255];
    if (hover) {
        for (let i = 0; i < 3; i++) {
            colour[i] -= 55;
        }
    }
    ctx.fillStyle = `rgb(${colour[0]},${colour[1]},${colour[2]})`;
    ctx.fillRect(x, y, size, size);
    ctx.fillStyle = "rgb(200,200,200)";
    ctx.strokeRect(x, y, size, size);
}

function drawLightBulb(ctx, x, y, win, wrap, size, err, hover) {
    drawBlank(ctx, x, y, win, wrap, true, size, hover);
    let lightbulb_d = size * 0.8;
    let lightbulb_r = lightbulb_d / 2;
    let lightbulb_x = x + size / 2;
    let lightbulb_y = y + size / 2;
    let lg = ctx.createRadialGradient(lightbulb_x, lightbulb_y, lightbulb_r * 0.1, lightbulb_x, lightbulb_y, lightbulb_r);
    if (!err) {
        lg.addColorStop(0, 'cyan');
        lg.addColorStop(1, 'purple');
    }
    else {
        lg.addColorStop(0, 'orange');
        lg.addColorStop(1, 'red');
    }
    ctx.fillStyle = lg;
    ctx.beginPath();
    ctx.arc(lightbulb_x, lightbulb_y, lightbulb_r, 0, 2 * Math.PI, false);
    ctx.fill();
}

function drawMark(ctx, x, y, win, wrap, lighted, size, hover) {
    drawBlank(ctx, x, y, win, wrap, lighted, size, hover);
    let mark_w = size * 0.2;
    let mark_h = size * 0.2;
    let mark_x = x + size / 2 - mark_w / 2;
    let mark_y = y + size / 2 - mark_h / 2;
    let lg = ctx.createLinearGradient(mark_x, mark_y, mark_x + mark_w, mark_y + mark_h);
    lg.addColorStop(0, 'yellow');
    lg.addColorStop(1, 'red');
    ctx.fillStyle = lg;
    ctx.beginPath();
    ctx.fillRect(mark_x, mark_y, mark_w, mark_h);
}

canvas.addEventListener("click", function (e) {
    console.log("click !");
    let i = Math.floor(e.offsetY / canvas.clientHeight * nb_rows);
    let j = Math.floor(e.offsetX / canvas.clientWidth * nb_cols);
    if (i < 0)
        i = 0;
    else if (i >= nb_rows)
        i = nb_rows - 1;
    if (j < 0)
        j = 0;
    else if (j >= nb_cols)
        j = nb_cols - 1;
    playLightbulb(g, i, j);
});

canvas.addEventListener("contextmenu", function (e) {
    e.preventDefault();
    console.log("rclick !");
    let i = Math.floor(e.offsetY / canvas.clientHeight * nb_rows);
    let j = Math.floor(e.offsetX / canvas.clientWidth * nb_cols);
    if (i < 0)
        i = 0;
    else if (i >= nb_rows)
        i = nb_rows - 1;
    if (j < 0)
        j = 0;
    else if (j >= nb_cols)
        j = nb_cols - 1;
    playMark(g, i, j);
});

function playLightbulb(g, i, j) {
    if (i >= 0 && i < nb_rows && j >= 0 && j < nb_cols) {
        if (Module._is_lightbulb(g, i, j))
            Module._play_move(g, i, j, BLANK);
        else if (Module._is_blank(g, i, j))
            Module._play_move(g, i, j, LIGHTBULB);
        printGame();
    }
}

function playMark(g, i, j) {
    if (i >= 0 && i < nb_rows && j >= 0 && j < nb_cols) {
        if (Module._is_marked(g, i, j))
            Module._play_move(g, i, j, BLANK);
        else if (Module._is_blank(g, i, j) || Module._is_lightbulb(g, i, j))
            Module._play_move(g, i, j, MARK);
        printGame();
    }
}

canvas.addEventListener("mousemove", function (e) {
    mouseI = Math.floor(e.offsetY / canvas.clientHeight * nb_rows);
    mouseJ = Math.floor(e.offsetX / canvas.clientWidth * nb_cols);
    if (mouseI < 0)
        mouseI = 0;
    else if (mouseI >= nb_rows)
        mouseI = nb_rows - 1;
    if (mouseJ < 0)
        mouseJ = 0;
    else if (mouseJ >= nb_cols)
        mouseJ = nb_cols - 1;
    printGame();
})

document.addEventListener("keydown", function (e) {
    console.log(e.key == "ArrowRight");
    switch (e.key) {
        case "ArrowRight":
            mouseJ++;
            printGame();
            break;
        case "ArrowLeft":
            mouseJ--;
            printGame();
            break;
        case "ArrowUp":
            mouseI--;
            printGame();
            break;
        case "ArrowDown":
            mouseI++;
            printGame();
            break;
        case "r":
            restartGame();
            break;
        case "s":
            solveGame();
            break;
        case "b":
            playLightbulb(g, mouseI, mouseJ);
            break;
        case "m":
            playMark(g, mouseI, mouseJ);
            break;
        case "z":
            undoMove();
            break;
        case "y":
            redoMove();
            break;
        case "n":
            newGame();
            break;
    }
});

window.addEventListener('resize', function () {
    printGame();
});

function printGame() {
    let width = div.clientWidth;
    let height = div.clientHeight;
    let size_case = height / nb_rows;
    if (size_case * nb_cols > width)
        size_case = width / nb_cols;
    let rec_x = 0;
    let rec_y = 0;
    console.log(`w: ${width}, h: ${height}, sc: ${size_case}, r: ${nb_rows}, c:${nb_cols}`);
    let win = Module._is_over(g);
    ctx.canvas.width = nb_cols * size_case;
    ctx.canvas.height = nb_rows * size_case;
    for (let i = 0; i < nb_rows; i++) {
        for (let j = 0; j < nb_cols; j++) {
            if (Module._is_black(g, i, j))
                drawWall(ctx, rec_x, rec_y, size_case, Module._get_black_number(g, i, j), Module._has_error(g, i, j));
            else if (Module._is_lightbulb(g, i, j))
                drawLightBulb(ctx, rec_x, rec_y, win, Module._is_wrapping(g), size_case, Module._has_error(g, i, j), (mouseI == i && mouseJ == j));
            else if (Module._is_marked(g, i, j))
                drawMark(ctx, rec_x, rec_y, win, Module._is_wrapping(g), Module._is_lighted(g, i, j), size_case, (mouseI == i && mouseJ == j));
            else if (Module._is_blank(g, i, j))
                drawBlank(ctx, rec_x, rec_y, win, Module._is_wrapping(g), Module._is_lighted(g, i, j), size_case, (mouseI == i && mouseJ == j));
            rec_x += size_case;
        }
        rec_x = 0;
        rec_y += size_case;
    }
}