const msg_default = '配膳ロボットに通信中です<br>しばらくお待ち下さい...'
const msg_succeed_1 = '通信が成功しました<br>';
const msg_succeed_2 = '番に配膳ロボットが向かいます';
const msg_failed = '通信が失敗しました<br>初めからやり直してください';
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
var ack = false, lock = false;

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
/*var ret = setTimeout(function(){
    // 指定した秒数後に実行したい処理を書く
    var num = Math.floor(Math.random()*2);
    if(num == 0){
        // succeed
        console.log('succeed');
        stat.innerHTML = '通信成功';
        txt.innerHTML = succeed_1 + '<span id="seat">' + seat + '</span>' + succeed_2;
        btn.innerHTML = 'OK';
    }
    else if(num == 1){
        // failed
        console.log('failed');
        stat.innerHTML = '通信失敗';
        txt.innerHTML = failed;
        btn.innerHTML = 'ホームへ'
    }
    btn.removeEventListener('click', btn_cancel);
    btn.addEventListener('click', btn_go_home);
}, 5000);*/


function setDefault() {
    lock = ack = false;
    stat.innerHTML = '通信中';
    txt.innerHTML = msg_default;
    error.style.visibility = 'hidden'
    btn.style.visibility = 'hidden'
    btn_retry.style.visibility = 'hidden'
}

function setFailed() {
    stat.innerHTML = '通信失敗';
    txt.innerHTML = msg_failed;
    error.style.visibility = 'visible'
    error.innerHTML = 'WebSocket connection to \'' + ip_addr + '\' failed'
}

function setSucceed() {
    stat.innerHTML = '通信成功';
    txt.innerHTML = msg_succeed_1 + '<span id="seat">' + seat + '</span>' + msg_succeed_2;
}

/*function btn_go_home() {
    window.location = 'index.html';
}
function btn_cancel() {
    clearTimeout(ret);
    console.log('cancel');
    stat.innerHTML = '通信中断';
    txt.innerHTML = msg_suspension;
    btn.innerHTML = 'ホームへ'
    this.removeEventListener('click', btn_cancel);
    this.addEventListener('click', btn_go_home);
}
btn.addEventListener('click', btn_cancel);*/

function websocketClient() {
    var ws = new WebSocket(ip_addr);

    //接続時
    ws.onopen = async function (event) {
        console.log('接続完了',event);
        ws.send("routeassign");
        await _sleep(2000);
        ws.send(seat);
        ack = event.returnValue;
        Responce();
        ws.close();
    };

    //エラー時
    ws.onerror = function (error) {
        console.log('接続エラー',error);
    };

    //メッセージ受信時
    ws.onmessage = function (event) {
        console.log('メッセージ受信',event);
    };

    //切断時
    ws.onclose = function () {
        console.log('通信切断');
        Responce();
    };
}

function Responce() {
    if (!ack) {
        lock = true;
        setFailed();
    } else if (ack && !lock) {
        setSucceed();
    }
    btn.style.visibility = 'visible'
    btn_retry.style.visibility = 'visible'
}