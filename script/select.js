var was_pushed;
var button = document.getElementsByClassName('btn');
for(let i of button){
    i.addEventListener('click', function(){
        if(was_pushed != undefined){
            was_pushed.style.border = 'none';
        }
        was_pushed = i;
        console.log('click', was_pushed.innerText);
        this.style.border = "solid 4px #f00";
    });
}

var ok_btn = document.getElementById('ok');
ok_btn.addEventListener('click', function(){
    window.location = 'index.html' + '?seat=' + was_pushed.innerText;
});