/*

    **********************************DARKSATHI*************************************
    
    J'ai utilisé dans toute cette page la norme de codage JSLint : http://jslint.com/
    
*/

function binDeci(bin) {
    
    "use strict";
    
    var deci = 0,
        longueurBin = bin.length,
        loop;
    for (loop = 0; loop < longueurBin; loop = loop + 1) {
        deci = deci + parseInt(bin[loop], 2) * Math.pow(2, longueurBin - loop - 1);
    }
    return deci;
}

function binDeciSimplifiee(bin) {
    
    "use strict";
    
    return parseInt(bin, 2);
}

function affichageBinDeci() {
    
    "use strict";
    
    var bin = document.getElementById("binaire"), deci = document.getElementById("decimale");
    deci.value = binDeci(bin.value);
}


function deciBin(deci) {
    
    "use strict";
     
    var bintableau = [0],
        bin = 0,
        positionByte = 0;
    while (deci !== 0) {
        bintableau[positionByte] = deci % 2;
        deci = Math.floor(deci / 2);
        positionByte = positionByte + 1;
    }
    bin = bintableau.reverse();
    return bin.join("");
}

function affichageDeciBin() {
    
    "use strict";
    
    var bin = document.getElementById("binaire2"), deci = document.getElementById("decimale2");
    bin.value = deciBin(deci.value);
}


