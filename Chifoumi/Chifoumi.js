var pierre = document.getElementById("pierre"),
    feuille = document.getElementById("feuille"),
    ciseaux = document.getElementById("ciseaux"),
    result = document.getElementById("result"),
    score = document.getElementById("score"),
    Ordi = document.getElementById("Ordi"),
    reset = document.getElementById("reset"),
    score_bot = 0,
    score_joueur = 0;

/* Utilise la propriété innerHTML pour afficher le résultat dans ta balise result */

window.addEventListener("load", function () {
    
    "use strict";

    pierre.addEventListener("click", function () {
        
        var bot = Math.floor(Math.random() * 3 + 1);
        if (bot === 1) {
            Ordi.innerHTML = "L'ordinateur a joué pierre";
            result.innerHTML = "Égalité :/";
        }
        if (bot === 2) {
            Ordi.innerHTML = "L'ordinateur a joué feuille";
            result.innerHTML = "Perdu :(";
            score_bot++;
            score.innerHTML = score_joueur + " - " + score_bot;
        }
        if (bot === 3) {
            Ordi.innerHTML = "L'ordinateur a joué ciseaux";
            result.innerHTML = "Gagné! :)";
            score_joueur++;
            score.innerHTML = score_joueur + " - " + score_bot;
        }
    });
    
    feuille.addEventListener("click", function () {
        
        var bot = Math.floor(Math.random() * 3 + 1);
        if (bot === 1) {
            Ordi.innerHTML = "L'ordinateur a joué pierre";
            result.innerHTML = "Gagné! :)";
            score_joueur++;
            score.innerHTML = score_joueur + " - " + score_bot;
        }
        if (bot === 2) {
            Ordi.innerHTML = "L'ordinateur a joué feuille";
            result.innerHTML = "Égalité :/";
        }
        if (bot === 3) {
            Ordi.innerHTML = "L'ordinateur a joué ciseaux";
            result.innerHTML = "Perdu :(";
            score_bot++;
            score.innerHTML = score_joueur + " - " + score_bot;
        }
    });
    
    ciseaux.addEventListener("click", function () {
        
        var bot = Math.floor(Math.random() * 3 + 1);
        if (bot === 1) {
            Ordi.innerHTML = "L'ordinateur a joué pierre";
            result.innerHTML = "Perdu :(";
            score_bot++;
            score.innerHTML = score_joueur + " - " + score_bot;
        }
        if (bot === 2) {
            Ordi.innerHTML = "L'ordinateur a joué feuille";
            result.innerHTML = "Gagné! :)";
            score_joueur++;
            score.innerHTML = score_joueur + " - " + score_bot;
        }
        if (bot === 3) {
            Ordi.innerHTML = "L'ordinateur a joué ciseaux";
            result.innerHTML = "Égalité :/";
        }
    });
	
	reset.addEventListener("click", function () {
		score.innerHTML = "0 - 0";
		score_bot = 0;
		score_joueur = 0;
        Ordi.innerHTML = "";
		result.innerHTML = "Résultat";
	});
});