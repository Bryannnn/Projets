// La fonction binDeci fait la conversion d'un nombre binaire en écriture en nombre décimale.
function binDeci(bin) {
    // deci et longueurBin sont des variables locales.
    // bin="10010100" ; bin[0]=1 ; longueurbin = 8;le dernier caractère bin[7]
    // Utilisation d'un opérateur conditionnel pour contrôler le champ en écriture binaire.
    // Contrôle des valeurs des variables dans la console du navigateur.
    if (isNaN("0b"+bin)||bin.length == 0) {
        deci = "Non binaire."
    }else{
        var deci = 0;
        var longueurBin = bin.length;
        for(var loop=0;loop<longueurBin;loop++){
            deci+=parseInt(bin[loop])*Math.pow(2,longueurBin-loop-1);
        }
    }
    return deci;
}

// Une fonction qui ne renvoie pas de valeur est une procédure.

function affichage_bin_deci(){
    var bin = document.getElementById("binaire");
    var deci = document.getElementById("decimale");
    deci.value = binDeci(bin.value);
}

function deciBin(deci){
    // Utilisation d'un opérateur conditionnel pour contrôler le champ en écriture décimale.    
    if (parseInt(deci) !== parseFloat(deci)||deci.length == 0) { // Si le ce n'est pas un nombre ou que la longueur du champ = 0 donc la variable est vide.
        bin = "Non décimal."
    }else{
    // bin est tableau vide qui va contenir des 1 et des 0 après la conversion : [1,1,1,0,0,0,1]
    var bintableau = [0];
    // La valeur bin sera le nombre sous sa forme binaire
    var bin = 0;
    var positionByte = 0;
    while(deci!=0){ //je commence une défini une boucle qui s'arrêtera lorsque deci sera égale à 0
        bintableau[positionByte]=deci%2; //on note le reste de la division euclidienne de deci:2 dans le tableau
        deci=Math.floor(deci/2); //deci prend la valeur du resultat de la division euclidienne
        positionByte+=1; // on avance de 1 dans le tableau pour pouvoir répéter la boucle
    }
        bintableau.reverse(); //La méthode reverse() transpose les éléments d'un tableau : le premier élément devient le dernier et le dernier devient le premier et ainsi de suite.
        bin = bintableau.join(""); //transforme le tableau [1,1,1,0,0,0,1] en la chaine de caractère "1110001" pour permettre l'affichage dans le formulaire.
    }
    return bin;
}


function affichage_deci_bin(){
    var bin = document.getElementById("bin1") ;
    var deci = document.getElementById("deci1") ;
    bin.value = deciBin(deci.value);
}

document.addEventListener("click",function(evenement){console.log("sourisX : " +evenement.clientX+"\nsourisY : "+evenement.clientY)})

var bin_deci = document.getElementById("binDeci");
var deci_bin = document.getElementById("deciBin");
var conversion_bin_deci_ = document.getElementById("conversion_bin_deci");
var conversion_deci_bin = document.getElementById("conversion_deci_bin");
bin_deci.addEventListener("click",function(){
    conversion_bin_deci.style.display = "block";
    conversion_deci_bin.style.display = "none";
    deci_bin.style.borderStyle = "groove";
    bin_deci.style.borderStyle = "ridge";
});
deci_bin.addEventListener("click",function(){
    conversion_deci_bin.style.display = "block";
    conversion_bin_deci.style.display = "none";
    bin_deci.style.borderStyle = "groove";
    deci_bin.style.borderStyle = "ridge";
})