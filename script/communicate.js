const msg_default = '管理システムに通信中です<br>しばらくお待ち下さい...'
const msg_succeed_1 = '通信に成功しました<br>';
const msg_succeed_2 = '番にMierBotが向かいます';
const msg_failed = '通信が失敗しました<br>初めからやり直すか、再度接続を行ってください';
const msg_suspension = '通信が中断されました<br>ホーム画面へ戻ります';

const ip_addr = "ws://192.168.3.50:9001/"
const _sleep = (ms) => new Promise((resolve) => setTimeout(resolve, ms));

var stat = document.getElementById('stat');
var txt = document.getElementById('txt');
var error = document.getElementById('code');
var btn = document.getElementById('btn');
var btn_retry = document.getElementById('btn_retry');

var seat;
var url_params = new URLSearchParams(window.location.search);
var ack = fin = false;

setDefault();

if(url_params.has('seat')){
    seat = url_params.get('seat');
}

window.addEventListener('beforeunload', function dontReload(e) {
    e.preventDefault();
}, false);

btn.addEventListener('click', function () {
    window.location = 'home.html';
});

btn_retry.addEventListener('click', function () {
    websocketClient();
    setDefault();
});

websocketClient();

function setDefault() {
    ack = fin = false;
    stat.innerHTML = '通信中';
    txt.innerHTML = msg_default;
    error.style.visibility = 'hidden'
    btn.style.visibility = 'hidden'
    btn_retry.style.visibility = 'hidden'
}

function setFailed(msg) {
    stat.innerHTML = '通信失敗';
    txt.innerHTML = msg_failed;
    error.style.visibility = 'visible'
    error.innerHTML = msg
    btn.style.visibility = 'visible'
    btn_retry.style.visibility = 'visible'
}

function setSucceed() {
    stat.innerHTML = '通信成功';
    txt.innerHTML = msg_succeed_1 + '<span id="seat">' + seat + '</span>' + msg_succeed_2;
    btn.style.visibility = 'visible'
}

function websocketClient() {
    var ws = new WebSocket(ip_addr);

    //接続時
    ws.onopen = async function (event) {
        console.log('接続完了',event);
        ws.send("routeassign");
        await _sleep(2000);
        ws.send(seat);
        ack = event.returnValue;
    };

    //エラー時
    ws.onerror = function (error) {
        console.log('接続エラー',error);
    };

    //メッセージ受信時
    ws.onmessage = function (event) {
        console.log('メッセージ受信', event);
        var msg = event.data
        fin = true;
        if (msg == 'ack') {
            setSucceed();
        } else if (msg == 'lock') {
            setFailed('transmission refuse; route is locked');
        } else if (msg == 'refuse') {
            setFailed('transmission refuse; routesearch-sys unavailable');
        }
        ws.close();
    };

    //切断時
    ws.onclose = function () {
        console.log('通信切断');
        if(!fin)
            setFailed('connection failure; cannot connect to \'' + ip_addr + '\'');
    };
}