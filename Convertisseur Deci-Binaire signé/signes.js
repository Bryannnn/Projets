/*

    **********************************DARKSATHI*************************************
    
    J'ai utilisé dans cette page la norme de codage JSLint : http://jslint.com/

*/


/* 
Le but de ce mini projet est de donner l'écriture binaire signé d'un nombre relatif. 
L'entrée est un nombre décimale dont le type sera un nombre(number) ou chaîne de caractères(string).
La forme de l'affichage de la réponse dans le DOM est laissée libre à l'imagination du programmeur.
*/


/* La fonction "complementAUn" retourne une chaîne de caractère qui est le complement à un du nombre binaire "bin". */
/* "bin" est un nombre binaire de type chaîne de caractères ou nombre */
/* "codage" est le nombre de bits choisi pour coder les nombres.
/* "codagePc" est le nombre de bits utilisé par javascript pour coder les nombres binaires : 32 bits */

function complementAUn(bin, codage, codagePc) {
    
    "use strict";
    
    var complement_a_un = parseInt(bin,2),
        decalage = codagePc-codage;
    complement_a_un = ~complement_a_un;
    complement_a_un = complement_a_un & (complement_a_un << decalage >>> decalage);
    
    return complement_a_un.toString(2);
}
    /* 
    Le mode "use strict" force le programmeur à coder idéalement, Brackets vous indiquera vos erreurs au fur et à mesure de l'enregistrement de votre code. 
    Attention, avec ce mode de codage, si votre algorithme et correct mais non normalisé alors il ne fonctionnera pas!!!!
    */
    
    /*
    - On déclare une variable "complement_a_un" qui sera de type nombre et sera affectée de la valeur de "bin".
      Attention "bin" est un nombre binaire qui peut être de type chaîne de caractères.
    - On déclare une variable "decalage" qui sera de type nombre et sera affectée de la différence entre valeur référencées par "codagePc" et "codage".
    - On modifie la valeur de "complement_a_un" en inversant tous ses bits(Attention codage sur 32 bits en javascript).
      On utilisera astucieusement << et >>>.
    - On modifie la valeur de "complement_a_un" en passant les "decalage" bits de poids fort à 0.(On justifiera cette affectation.)
    - on retourne le complément à un de "bin" sous sa forme binaire avec pour type "string".    
    */
    
//}

/* La fonction "complementADeux" retourne une chaîne de caractère qui est le complement à deux du nombre binaire "bin". */
/* "bin" est un nombre binaire de type chaîne de caractères ou nombre. */

function complementADeux(bin, codage, codagePc) {
    
    "use strict";
    
    var decalage= codagePc-codage,
        complement_a_deux = parseInt(complementAUn(bin, codage, codagePc), 2)+1
    complement_a_deux = complement_a_deux & (complement_a_deux << decalage >>> decalage);
    
    return complement_a_deux.toString(2);
}
    /*
    - On déclare une variable "decalage" qui sera de type nombre et sera affectée de la différence entre "codagePc" et "codage".
    - On déclare une variable "complement_a_deux" qui sera de type nombre et sera affectée de la valeur "complementAUn(bin, codage, codagePc)" augmenté de 1.
    - On modifie la valeur de "complement_a_deux" en passant les "decalage" bits de poids fort à 0.
    - on retourne le complément à deux de "bin" sous sa forme binaire avec pour type "string".
    */
//}

/* La fonction "signe" donne l'écriture d'un entier relatif sous la forme d'un signé sur une capacité de "codage" bits. */
/* ATTENTION DIFFICILE! */

function signe(deci, codage, codagePc) {
    
    "use strict";
    //if (parseInt(deci) !== parseFloat(deci)||deci.length == 0) {
        //bin = "Il faut entrer un nombre décimal"
    //}else{
	var bin = 0;
    if (parseInt(deci,10) <0) {
        deci = parseInt(deci, 10) * (-1);
        bin = complementADeux(deciBin(deci), codage, codagePc);
    }else{
        bin = parseInt(parseInt(deciBin(deci),2).toString(2), 2);
        bin = (bin >>> 8) << 8 ^ bin;
        bin = bin.toString(2);
		while (bin.length<8) {
			bin = "0" + bin;
		}
    }
    return bin;
}

function affichage_bin_signe() {
    var deci = window.prompt("donner une valeur entre -128 et 127");
    var codage = 8;
    var bin = document.getElementById("result").innerHTML = deci + " a pour écriture binaire signée sur " + codage + " bits :" + signe(deci, codage, 32);
}

var bouton = document.getElementById('bouton');
bouton.addEventListener("click", affichage_bin_signe);

    /*
    - On déclare une variable "bin" de type nombre initialisée à 0.
    - Si "deci" représente un nombre négatif alors
                                           deci prend la valeur de son opposé.
                                           On modifie la valeur de "bin" par le complément à deux de "deci".
      Sinon
                                           "bin" de type nombre, prend la valeur binaire de "deci".
                                           "bin" prend la valeur des codage bits de "bin" de poids faible et les autres bits sont passés à 0.
                                           "bin" de type string, référence la valeur de bin sous sa forme binaire.
                                           
    - On retourne la chaine de caractère "bin".
    */


/*
Réaliser alors un test dans la console du navigateur en testant : 
            signe(15, 4, 32) ---> "1111"
            signe(-15, 4, 32) ---> "1"
            signe(125, 8, 32) ---> "1111101"
            signe(-125, 8, 32) ---> "10000011"
*/

/*
BONUS : Mettre en place les tests de validités necessaires pour éviter ce type d'erreur : 
         signe(-257, 8, 32) ---> "11111111"

*/

/*

INFORMATIONS UTILES

- Attention! il faut prendre garde au codage utilisé en javascript : 32 bits.

- ~ C'est le NON appliqué bit à bit :
                
            ~11011  --> 11111111111111111111111111100100   ATTENTION! SUR UN CODAGE 32 bits

- << décalage des bits vers la gauche : 00000000000000000000000000001111 << 2  --> 00000000000000000000000000111100

- >> décalage des bits vers la droite avec propagation du signe : 
                10000000011101110001111110111111 >> 2 --> 10000000000111011100011111101111
                
- >>> décalage des bits vers la droite sans propagation du signe :
                10000000011101110001111110111111 >>> 2 --> 00000000000111011100011111101111
                
- & est le ET logique appliqué bit à bit :

                00000000000000000000000000001001
            &
                00000000000000000000000000001110
            =
                00000000000000000000000000001000

- | est le OU logique appliqué bit à bit : 

                00000000000000000000000000001001
            |
                00000000000000000000000000001110
            =
                00000000000000000000000000001111
                
- ^ est le OU exlusif(XOR) appliqué bit à bit :
                
                00000000000000000000000000001001
            ^
                00000000000000000000000000001110
            =
                00000000000000000000000000000111

- fonction toString() :  nombre.toString(2) affichera le nombre sous sa forme binaire.

- fonction join("")  : Si tableau = ["1","1","1","1","1"] alors tableau.join("") = "11111" .

- parseInt(nombre, 2) nombre est considéré comme écrit en base 2 et sera converti en type nombre.

-       typeof 25 --> "nombre"
        typeof "25" --> "string"


*/

