const arrow = '<p>↓</p>\n';
const end = '<p>キッチン</p>';
const seat = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M'];
var num = Math.floor(Math.random()*3 + 1);
var txt = document.getElementsByClassName('wrapper');
var string = ''

for(var i = 0; i < num; i++){
    string = string + '<p><span id="seat">' + seat[Math.floor(Math.random()*seat.length)] + '</span>番' + Math.floor(Math.random()*6 + 1) + '人</p>\n' + arrow;
}
string = string + end;
txt[0].innerHTML = string;

// var btn_home = document.getElementById('home');
// btn_home.addEventListener('click', function(){
//     window.location = 'index.html';
// });
// var btn_home = document.getElementById('home');
// function go_to_home(){
//     window.location = 'index.html';
// }
// btn_home.addEventListener('click', go_to_home);

// function rand_status(){
//     for(var i = 0; i < num; i++){
//         string = string + '<p><span id="seat">' + seat[Math.floor(Math.random()*seat.length)] + '</span>番' + Math.floor(Math.random()*6 + 1) + '人</p>\n' + arrow;
//     }
//     string = string + end;
//     txt[0].innerHTML = string;
// }
// window.onload = function(){
//     // console.log('onload');
//     rand_status();
// }