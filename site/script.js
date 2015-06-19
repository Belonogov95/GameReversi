var name;


$(document).ready(function () {
    $("#loginButton").click(onClickLogin);

});

function onClickLogin() {
    name = $("#username").val();
    $.post("login", name, loginCallBack);
}

function loginCallBack(data, status) {
    //alert("data: " + data + "\nstatus: " + status);
    //alert("here3")
    if (data == "unsuccess") {
        alert("this login has already taken. Try another.");
        return;
    }
    $("#loginDiv").hide();
    updatePlayerList();
    //setInterval(updatePlayerList, 500);
}

function updatePlayerList() {
    //alert("before update players");
    $.post("players", name, playerListCallBack);
}

function playerListCallBack(data, status) {
    var q = eval(data);
    alert("data: " + data + "\nstatus: " + status + "\nlength: " + q.length);
    $("#userList").append("aba");
}



