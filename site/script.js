var name;
var LOGIN;
var LINE_BREAK;
var CONNECT_INTERVAL_ID;
var GAME_INTERVAL_ID;

$(document).ready(function () {
    //$("#connectDiv").hide();
    //$("#boardDiv").hide();
    //alert("here");
    $("#loginButton").click(onClickLogin);
   createBoard();
    drawCircle(1, 1, 0);
    drawCircle(2, 2, 1);
    drawCircle(2, 3, 0);
    drawCircle(2, 3, -1);
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
    $("#connectDiv").prepend("<h4> Hello, " + name + "</h4>");
    updatePlayerList();
    CONNECT_INTERVAL_ID = setInterval(updatePlayerList, 500);
}

function updatePlayerList() {
    //alert("before update players");
    $.post("players", LOGIN, playerListCallBack);
}

function playerListCallBack(data, status) {
    var q = eval(data);
    //alert("data: " + data + "\nstatus: " + status + "\nlength: " + q.length);
    //$("#userList").append("aba");
    if (q.length == 3) {
        //alert("here");
        initGame(q[2]);
        return;
    }
    $("#allUserList").empty();
    $("#inviteList").empty();
    for (i = 0; i < q[0].length; i++)
        $("#allUserList").append('<li onclick="invitePerson' + "('" + q[0][i] + "')" + '" >' + q[0][i] + "</li>");
    for (i = 0; i < q[1].length; i++)
        $("#inviteList").append('<li onclick="invitePerson' + "('" + q[1][i] + "')" + '" >' + q[1][i] + "</li>");
}

function invitePerson(player) {
    alert("you are invited: " + player);
    $.post("invite", LOGIN + "target=" + player + LINE_BREAK);
}
////////////////////////////////////////////// game

function initGame(enemy) {
    alert("game: " + name + " vs " + enemy);
    clearInterval(CONNECT_INTERVAL_ID);
    $("#connectDiv").css("display", "none");
    $("#boardDiv").css("display", "inline");
    createBoard();
    updateBoard();
    GAME_INTERVAL_ID = setInterval(500, updateBoard);
}

function updateBoard() {
    $.post("board", LOGIN, updateBoardCallBack);
}

function updateBoardCallBack(data, status) {
    var board = eval(data);
    for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++)
            drawCircle(i, j, board[i][j]);
}


function createBoard() {
    for (var i = 0; i < 8; i++) {
        var str = '<tr id="row' + i + '">' + 'gggg' + "</tr>";
        $("#newTable").append(str);
        for (var j = 0; j < 8; j++) {
            var cl = 'onclick="cellClick(' + i + ", " + j + ')"'
            var id = '"cell' + i + j + '"';
            var sz = 60;
            var canvas = "<canvas id=" + id +' width="' + sz + '"' + 'height="' + sz + '"></canvas>';
            str2 = "<td " + cl + ">" + canvas + "</td>";
            $("#row" + i).append(str2);
        }
    }

}



function cellClick(x, y) {
    alert("x: " + x + "\ny: " + y);
    $.post("move", LOGIN + "x=" + x + LINE_BREAK + "y=" + y + LINE_BREAK);
}


function drawCircle(x, y, color) {
    var id = "cell" + x + y;
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





