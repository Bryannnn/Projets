window.onload = function () {
    
lignePrincipal = document.getElementById("lignePrincipal");
lignePrincipalTd = lignePrincipal.getElementsByTagName("td");

var cols = 16;
var rows = 8;

for (var j = 0; j < rows; j++) {
    for (var i = 0; i < cols; i++) {
      
    var n = "0x"+document.getElementById("1"+j).innerHTML+lignePrincipalTd[i].innerHTML;
   
	document.getElementById(j).getElementsByTagName("td")[i].innerHTML = String.fromCharCode(n);
   
        }
    }
}