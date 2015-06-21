var name;
var LOGIN;
var LINE_BREAK;
var CONNECT_INTERVAL_ID;
var GAME_INTERVAL_ID;
var CELL_SIZE = 60;
var title;

$(document).ready(function () {
    //$("#connectDiv").hide();
    //$("#boardDiv").hide();
    //alert("here");
    $("#loginButton").click(onClickLogin);
   //createBoard();
   // drawCircle(1, 1, 0);
   // drawCircle(2, 2, 1);
   // drawCircle(2, 3, 0);
   // drawCircle(2, 3, -1);
});

function onClickLogin() {
    name = $("#username").val();
    LINE_BREAK = "\r\n";
    LOGIN = "login=" + name + LINE_BREAK;

    $.post("login", LOGIN, loginCallBack);
}

///////////////////////////////////// connect

function loginCallBack(data, status) {
    //alert("data: " + data + "\nstatus: " + status);
    //alert("here3")
    if (data == "unsuccess") {
        alert("this login has already taken. Try another.");
        return;
    }
    $("#loginDiv").css("display", "none");
    //alert("here3")
    $("#connectDiv").css("display", "inline");
    $("#mainDiv").prepend("<h4> Hello, " + name + "</h4>");
    updatePlayerList();
    CONNECT_INTERVAL_ID = setInterval(updatePlayerList, 500);
}

function updatePlayerList() {
    //alert("before update players");
    $.post("players", LOGIN, playerListCallBack);
}

function playerListCallBack(data, status) {
    var r = eval(data);
    q = [eval(r[0]), eval(r[1])];

    //alert("data: " + data + "\nstatus: " + status + "\nlength: " + q.length);
    //$("#userList").append("aba");
    if (r.length == 4) {
        //alert("here");
        initGame(r[2], r[3]);
        return;
    }
    $("#allUserList").empty();
    $("#inviteList").empty();
    for (j = 0; j < 2; j++)
        for (i = 0; i < q[j].length; i++) {
            var tmp ='<li><button onclick="invitePerson' + "('" + q[j][i] + "')" + '" >' + q[j][i] + "</button></li>";
            if (j == 0)
                $("#allUserList").append(tmp);
            else
                $("#inviteList").append(tmp);
        }
}

function invitePerson(player) {
    //alert("you are invited: " + player);
    $.post("invite", LOGIN + "target=" + player + LINE_BREAK);
}
////////////////////////////////////////////// game

function initGame(enemy, color) {
    alert("game: " + name + " vs " + enemy);
    clearInterval(CONNECT_INTERVAL_ID);
    $("#connectDiv").css("display", "none");
    $("#boardDiv").css("display", "inline");

    if (color == "1")
        title = name + " vs " + enemy;
    else
        title = enemy + " vs " + name;

    //$("#titleGame").append(createCanvas("leftCircle", CELL_SIZE) +  title +
            //createCanvas("rightCircle", CELL_SIZE));
    drawCircleById("leftCircle", 1);
    drawCircleById("rightCircle", 2);
    createBoard();
    updateBoard();
    GAME_INTERVAL_ID = setInterval(updateBoard, 500);
}

function updateBoard() {
    //alert("send post");
    $.post("board", LOGIN, updateBoardCallBack);
}

function updateBoardCallBack(data, status) {
    var q = eval(data);
    var board = eval(q[0]);
    $("#turnGame").text("turn: " + ((q[3] == "1")? "white" : "black"));
    $("#forText").html("<b>" + q[1] +  "</b>" + "     " + title + "     " + "<b>" + q[2] + "</b>");
    //alert(data);
    if (q[4] == "1") {
        $("#forWinner").html(q[5]);
        clearInterval(GAME_INTERVAL_ID);
        $("a").css("display", "inline");
    }

    for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++)
            drawCircle(i, j, board[i][j]);
}


function createBoard() {
    for (var i = 0; i < 8; i++) {
        var str = '<tr id="row' + i + '">' + "</tr>";
        $("#newTable").append(str);
        for (var j = 0; j < 8; j++) {
            var cl = 'onclick="cellClick(' + i + ", " + j + ')"';
            var id = '"cell' + i + j + '"';
            var canvas = createCanvas(id, CELL_SIZE);
            str2 = "<td " + cl + ">" + canvas + "</td>";
            $("#row" + i).append(str2);
        }
    }

}

function createCanvas(id, sz) {
   return "<canvas id=" + id +' width="' + sz + '"' + 'height="' + sz + '"></canvas>';
}

function cellClick(x, y) {
    //alert("x: " + x + "\ny: " + y);
    $.post("move", LOGIN + "x=" + x + LINE_BREAK + "y=" + y + LINE_BREAK);
}

function drawCircle(x, y, color) {
    var id = "cell" + x + y;
    drawCircleById(id, color);
}

function drawCircleById(id, color) {
   var canvas = document.getElementById(id);
    var context = canvas.getContext('2d');
    if (color == 0) {
        context.clearRect(0, 0, canvas.width, canvas.height);
        return;
    }

    var centerX = canvas.width / 2;
    var centerY = canvas.height / 2;
    var radius = canvas.width * .4;
    context.beginPath();
    context.arc(centerX, centerY, radius, 0, 2 * Math.PI, false);
    if (color == 1)
        context.fillStyle = 'white';
    else
        context.fillStyle = 'black';
    context.fill();
    context.lineWidth = 5;
    context.strokeStyle = '#003300';
    context.stroke();
}





