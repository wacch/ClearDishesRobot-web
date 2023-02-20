var was_pushed;
var button = document.getElementsByClassName('btn');
var is_pushed = [];

var url_params = new URLSearchParams(window.location.search);
if (url_params.has('seat')) {
    rtn = url_params.get('seat')
    if (rtn != '') {
        is_pushed = rtn.split(',');
        for (let i of button) {
            selected = is_pushed.indexOf(i.innerText);
            if (selected != -1) {
                i.style.border = "solid 4px #f00";
            }
        }
    }
}
console.log(is_pushed);

for(let i of button){
    i.addEventListener('click', function(){
        was_pushed = i;
        console.log('click', was_pushed.innerText);
        selected = is_pushed.indexOf(was_pushed.innerText);

        if (selected != -1) {
            was_pushed.style.border = 'none';
            is_pushed.splice(selected, 1);
            console.log('deselection');
        } else {
            is_pushed.push(was_pushed.innerText);
            this.style.border = "solid 4px #f00";
        }
        console.log(is_pushed);
    });
}

var ok_btn = document.getElementById('ok');
ok_btn.addEventListener('click', function () {
    if (is_pushed.length >= 1) {
        window.location = 'index.html' + '?seat=' + (is_pushed.sort()).toString();
    } else {
        alert('座席は最低でも一箇所以上選択してください。');
    }
});