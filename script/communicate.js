var stat = document.getElementById('stat');
var txt = document.getElementById('txt');
var btn = document.getElementById('btn');
const succeed_1 = '通信が成功しました<br>';
const succeed_2 = '番に配膳ロボットが向かいます';
const failed = '通信が失敗しました<br>初めからやり直してください';
const suspension = '通信が中断されました<br>ホーム画面へ戻ります';

var seat = 'None';
var url_params = new URLSearchParams(window.location.search);
if(url_params.has('seat')){
    seat = url_params.get('seat');
}

console.log('start');
var ret = setTimeout(function(){
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
}, 5000);


function btn_go_home(){
    window.location = 'index.html';
}
function btn_cancel(){
    clearTimeout(ret);
    console.log('cancel');
    stat.innerHTML = '通信中断';
    txt.innerHTML = suspension;
    btn.innerHTML = 'ホームへ'
    this.removeEventListener('click', btn_cancel);
    this.addEventListener('click', btn_go_home);
}
btn.addEventListener('click', btn_cancel);