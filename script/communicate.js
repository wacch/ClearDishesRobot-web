//テキストメッセージ
const msg_default = '管理システムに通信中です<br>しばらくお待ち下さい...'
const msg_succeed_1 = '通信に成功しました<br>';
const msg_succeed_2 = '番にMierBotが向かいます';
const msg_failed = '通信が失敗しました<br>初めからやり直すか、再度接続を行ってください';
const msg_suspension = '通信が中断されました<br>ホーム画面へ戻ります';

//接続情報
const ip_addr = "ws://192.168.3.50:9001/"
const timeout = 10000

//css管理用変数
var stat = document.getElementById('stat');
var txt = document.getElementById('txt');
var error = document.getElementById('code');
var btn = document.getElementById('btn');
var btn_retry = document.getElementById('btn_retry');

//通常変数
var seat;
var url_params = new URLSearchParams(window.location.search);
var ack = fin = get = false;

//デバッグ用変数
var debug = false;
var mea_s = null
var diff_s = null

var acktimeout = function () {
    if (!fin) {
        fin = true;
        setFailed('connection failure; connection time out');
    }
};

setDefault();

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

if (url_params.has('seat')) {
    seat = url_params.get('seat');
} else if (url_params.has('get')) {
    get = true
}

websocketClient();

function setDefault() {
    ack = fin = false;
    stat.innerHTML = '通信中';
    txt.innerHTML = msg_default;
    error.style.visibility = 'hidden';
    btn.style.visibility = 'hidden';
    btn_retry.style.visibility = 'hidden';
}

function setFailed(msg) {
    stat.innerHTML = '通信失敗';
    txt.innerHTML = msg_failed;
    error.style.visibility = 'visible';
    error.innerHTML = msg;
    btn.style.visibility = 'visible';
    btn_retry.style.visibility = 'visible';
}

function setSucceed(msg = "") {
    stat.innerHTML = '通信成功';
    txt.innerHTML = msg_succeed_1 + '<span id="seat">' + seat + '</span>' + msg_succeed_2;
    btn.style.visibility = 'visible';
    if (debug) {
        error.style.visibility = 'visible';
        error.innerHTML = "DEBUG; send time(ms): " + diff_s + msg;
    }
}

function websocketClient() {
    var ws = new WebSocket(ip_addr);

    //接続時
    ws.onopen = async function (event) {
        console.log('接続完了', event);
        mea_s = new Date();
        ws.send("routeassign");
        ack = event.returnValue;
        if (ack) {
            if (!get)
                ws.send(seat);
            else
                ws.send("get");
        } else {
            setFailed('connection failure; unable to successfully connect to \'' + ip_addr + '\'');
        }
        setTimeout(acktimeout, timeout)
    };

    //エラー時
    ws.onerror = function (error) {
        console.log('接続エラー', error);
        //fin = true;
        //setFailed(error.returnValue);
    };

    //メッセージ受信時
    ws.onmessage = function (event) {
        console.log('メッセージ受信', event);
        var msg = event.data
        if (!fin && msg != 'debug') {
            fin = true;
            if (msg == 'ack') {
                var diff = new Date().getTime() - mea_s.getTime();
                setSucceed(' ,elapsed time(ms): ' + diff);
            } else if (msg == 'lock') {
                setFailed('transmission refuse; route is locked');
            } else if (msg == 'refuse') {
                setFailed('transmission refuse; routesearch-sys unavailable');
            } else {
                if (msg != "") {
                    window.location = 'robbotdrive.html' + '?seat=' + msg;
                } else {
                    window.location = 'robbotdrive.html';
                }
            }
            ws.close();
        } else if (msg == 'debug') {
            diff_s = new Date().getTime() - mea_s.getTime();
        }
    };

    //切断時
    ws.onclose = function () {
        console.log('通信切断');
        if(!fin)
            setFailed('connection failure; cannot connect to \'' + ip_addr + '\'');
    };
}