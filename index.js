var num = Math.floor(Math.random()*6 + 1);
document.getElementById('people').innerHTML = num;

var url_params = new URLSearchParams(window.location.search);
if(url_params.has('seat')){
    document.getElementById('seat').innerHTML = url_params.get('seat');
}

var btn_ok = document.getElementById('ok');
var btn_no = document.getElementById('no');

btn_ok.addEventListener('click', function(){
    window.location = 'index.html' + '?seat=' + url_params.get('seat');
});
btn_no.addEventListener('click', function(){
    window.location = 'index.html';
});