Module.onRuntimeInitialized = () => { start(); }

function drawWall(ctx, x, y, size, nb, err) {
    ctx.fillStyle = "#000000";
    ctx.fillRect(x, y, size, size);
    ctx.textAlign = "center";
    ctx.textBaseline = 'middle';
    ctx.font = size.toString(10) + 'pt Roboto-Regular';
    if (err)
        ctx.fillStyle = "#ff3737";
    else
        ctx.fillStyle = "#ffffff";
    switch (nb) {
        // 1.18 to compensate the cap height of numbers
        case 0:
            ctx.fillText("0", x + size / 2, (y + size / 2) * 1);
            break;
        case 1:
            ctx.fillText("1", x + size / 2, (y + size / 2) * 1);
            break;
        case 2:
            ctx.fillText("2", x + size / 2, (y + size / 2) * 1);
            break;
        case 3:
            ctx.fillText("3", x + size / 2, (y + size / 2) * 1);
            break;
        case 4:
            ctx.fillText("4", x + size / 2, (y + size / 2) * 1);
            break;
        default:
            break;
    }
}

function drawBlank(ctx, x, y, win, wrap, lighted, size) {
    ctx.beginPath();
    if (win)
        ctx.fillStyle = '#379637';
    else if (lighted)
        if (wrap)
            ctx.fillStyle = '#ffc037';
        else
            ctx.fillStyle = '#ffff37';
    else
        ctx.fillStyle = '#ffffff'
    ctx.fillRect(x, y, size, size);
}

function drawLightBulb(ctx, x, y, win, wrap, size, err) {
    drawBlank(ctx, x, y, win, wrap, true, size);
    let lightbulb_d = size * 0.8;
    let lightbulb_r = lightbulb_d / 2;
    let lightbulb_x = x + size / 2;
    let lightbulb_y = y + size / 2;
    let lg = ctx.createRadialGradient(lightbulb_x, lightbulb_y, lightbulb_r * 0.1, lightbulb_x, lightbulb_y, lightbulb_r);
    lg.addColorStop(0, 'cyan');
    lg.addColorStop(1, 'purple');
    ctx.fillStyle = lg;
    ctx.beginPath();
    ctx.arc(lightbulb_x, lightbulb_y, lightbulb_r, 0, 2 * Math.PI, false);
    ctx.fill();
    ctx.closePath();
}

function drawMark(ctx, x, y, win, wrap, lighted, size) {
    drawBlank(ctx, x, y, win, wrap, lighted, size);
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
    ctx.closePath();
}

function start() {
    let keyboard = {};
    document.addEventListener("keydown", function (e) {
        keyboard[e.key] = true;
        console.log(e.key);
    });
    document.addEventListener("keyup", function (e) {
        keyboard[e.key] = false;
        console.log(e.key);
    });
    document.addEventListener("click", function (e) {
        console.log("click !");
    })
    document.addEventListener("contextmenu", function (e) {
        e.preventDefault();
        console.log("rclick !");
    })
    console.log("call start routine");
    let canvas = document.getElementById("mycanvas");
    let ctx = canvas.getContext('2d');
    let width = document.documentElement.clientWidth * 0.8;
    let height = document.documentElement.clientHeight * 0.8;
    let max_size = Math.min(width, height);
    // drawWall(ctx, 0, 0, width, 3, false);
    const MAX_SIZE = 10;
    const MIN_SIZE = 3;
    let nb_rows = Math.round((Math.random() * (MAX_SIZE - MIN_SIZE + 1) + MIN_SIZE));
    let nb_cols = Math.round((Math.random() * (MAX_SIZE - MIN_SIZE + 1) + MIN_SIZE));
    let wrapping = Math.round(Math.random());
    let nb_walls = Math.round(Math.random() * ((nb_cols * nb_rows) - (nb_cols * nb_rows) / 5));
    if (nb_walls < (nb_cols * nb_rows) / 5)
        nb_walls = (nb_cols * nb_rows) / 5;
    var g = Module._new_random(nb_rows, nb_cols, wrapping ? true : false, nb_walls, false);
    const BLANK = 0;
    const LIGHTBULB = 1;
    const MARK = 2;
    var size_case = Math.round(max_size / Math.max(nb_rows, nb_cols));
    var rec_x = 0;
    var rec_y = 0;
    var win = false;
    ctx.canvas.width = nb_cols * size_case;
    ctx.canvas.height = nb_rows * size_case;
    Module._solve(g);
    Module._is_over(g);
    for (i = 0; i < nb_rows; i++) {
        for (j = 0; j < nb_cols; j++) {
            if (Module._is_black(g, i, j))
                drawWall(ctx, rec_x, rec_y, size_case, Module._get_black_number(g, i, j), Module._has_error(g, i, j));
            else if (Module._is_lightbulb(g, i, j))
                drawLightBulb(ctx, rec_x, rec_y, win, Module._is_wrapping(g), size_case, Module._has_error(g, i, j));
            else if (Module._is_marked(g, i, j))
                drawMark(ctx, rec_x, rec_y, win, Module._is_wrapping(g), Module._is_lighted(g), size_case);
            else if (Module._is_blank(g, i, j))
                drawBlank(ctx, rec_x, rec_y, win, Module._is_wrapping(g), Module._is_lighted(g), size_case);
            rec_x += size_case;
        }
        rec_x = 0;
        rec_y += size_case;
    }
    Module._delete(g);
}

