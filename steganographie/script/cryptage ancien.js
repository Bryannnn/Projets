window.addEventListener("load", function () {
    
    "use strict";
    

    
    //définis le canvas
    var canvas = document.getElementById("canvas");
    var canvas2 = document.getElementById("canvas_display");
    var environnement = canvas.getContext("2d");
    //récuperer image
    var image = document.getElementById("image");
    //variable qui étudie la validité de l'image
    var controle = true;
    //mets le canvas à la taille de l'image
    canvas.height = image.height;
    canvas.width = image.width;
    //bouton enregistrer
    var bouton = document.getElementById("enregistrer");


//n'affiche pas le canvas
    canvas.style.display = "none";
    var choix = "Un Message";
//en fonction du choix:
    //cache la 2eme image
    document.getElementById("image_cacher").style.display = "none";
    //quand on veut cacher une image, affiche le 2eme bouton parcourir
    document.getElementById("choix").addEventListener("change", function() {
        //récupère la valeur du select :  image ou message
        choix = document.getElementById('choix').value;
            if (choix == "Un Message") {
                document.getElementById("image_cacher").style.display = "none";
                document.getElementById("messageForm").style.display = "block";
            } else if (choix == "Une Image") {
                document.getElementById("image_cacher").style.display = "block";
                document.getElementById("messageForm").style.display = "none";
            }
    })
    
    
    //récupère l'image et l'affiche au clic 
    var fichier = document.getElementById("image_upload"), //bouton "parcourir"
    image = document.getElementById("image"); //canvas image

    //quand un fichier est choisit, le charge automatiquement
    fichier.addEventListener("change", function () { 
        //affiche l'image
        image.style.display = "block";
        //n'affiche plus le message "image cryptée"
        canvas2.style.display = "none";
        //n'affiche pas le canvas
        canvas.style.display = "none";
        
        var poids = this.files[0]["size"]; //poids du fichier
        var type = this.files[0]["type"]; //type du fichier
        //nombre de caractère maximal
        var caractereMax = poids / 32;
        
        //vérifie le type de l'image
        type = type.split("");
        var format = [];
        for (var i=0; i<5; i++){
            format.push(type[i]);
        }
        type = format.join("");
        //vérifie le poids de l'image
        if (poids > 1000000) {
            alert("Le fichier doit faire moins d'un Mo!")
            controle = false;
        } else {
            controle = true;
        }
        //vérifie que le fichier est une image
        if (type == "image" && controle){
            var file = this.files[0]; //tableau "files" contient tous les fichiers sélectionnés       
            var reader = new FileReader(); //lit le fichier
            image.style.display = "block";
            reader.addEventListener('load', function() {            
                image.src = this.result;  //affiche l'image
            });
            reader.readAsDataURL(file);
        } else {
            if (type != "image"){
                alert("Ce fichier n'est pas une image!");   
            }
        } 

        
    }); 
    
    
    
    
    
    
    
//fonction de cryptage , message dans une image
    function crypterMessage () {        
//récupération et transcription du message en binaire 
        if (controle) {
            //récupère le message
            var message = document.getElementById('message').value+"";
            // longueur du message
            var msg_len = message.length; 
            var msg_bin = "";
            //traduit le message en code ascii (hexa)
            for (var k = 0; k < msg_len; k++){
                var lettre_ascii = message.charCodeAt(k); //lettre dans son code ascii
                var lettre_bin = lettre_ascii.toString(2) + ""; // + "" permet d'avoir une chaine de caractère (en binaire)
                while (1) {
                    //si la longueur de la lettre en bin fait 8, sors de la boucle
                    if (lettre_bin.length == 8) {
                        break;
                    //sinon rajoute assez de 0 pour arriver à des lettres codés sur 1 octet
                    } else {
                    lettre_bin = "0" + lettre_bin;
                    }
                }
                //le message entier en binaire est la somme des lettres
                msg_bin = msg_bin + lettre_bin;
            }



            //mets le canvas à la taille de l'image
            canvas.height = image.height;
            canvas.width = image.width;
            //dessine l'image dans le canvas
            environnement.drawImage(image, 0, 0);
            //récupère les données de l'image (composantres RVBA de chaque px)
            var imageDonnees = environnement.getImageData(0, 0, canvas.width, canvas.height);
            //crée un tableau avec ces données
            var tableauPixel = imageDonnees.data;
            //copie du tableau qui agit comme sauvegarde (ne sera pas changée)
            var tableauPixel1 = tableauPixel.slice(0, tableauPixel.length);





    //insertion du message dans l'image
            console.log("message en binaire = ", msg_bin);
            //pour chaque pixel 
            for(var k = 0; k < tableauPixel.length; k += 4) {

                var i = k/4;

    //les différentes possibilités
                if (msg_bin[i] == 0 && tableauPixel[k]%2 == 0) {
                    //ne rien faire, la composante couleur est déjà paire 
                } 

                //si bit = 0 et composante impaire:
                else if (msg_bin[i] == 0 && tableauPixel[k]%2 == 1) {
                    //si coueleur noir (0) on ne peut que ajouter 1
                    if (tableauPixel[k] == 0) {
                        tableauPixel[k] = tableauPixel[k] + 1;
                    //sinon on retire 1 (car si couleur blanche (255) il y a depassement de capacité )
                    } else {
                        tableauPixel[k] = tableauPixel[k] - 1;
                    }
                } 

                //si bit = 1 et composante pair
                else if (msg_bin[i] == 1 && tableauPixel[k]%2 == 0) { // % = modulo (reste de la division euclidienne)
                    //si coueleur noir (0) on ne peut que ajouter 1
                    if (tableauPixel[k] == 0) {
                        tableauPixel[k] = tableauPixel[k] + 1;
                    //sinon on retire 1 (car si couleur blanche (255) il y a depassement de capacité )
                    } else {
                        tableauPixel[k] = tableauPixel[k] + 1;
                    }


                } else if (msg_bin[k] == 1 && tableauPixel[k]%2 == 1) {
                    //ne rien faire, composante couleur déjà impaire
                }

            }
            //affiche l'image avec le message caché
            environnement.putImageData(imageDonnees, 0, 0);




            //vérification du message
            console.log(tableauPixel1);
            console.log(tableauPixel);
            var long = tableauPixel.length; //longueur du tableau
            var messDecrypt = [];
            for (var z=0 ; z < long; z += 4) {
                if (tableauPixel[z]%2 == 0) {
                    messDecrypt.push(0);
                } else {
                    messDecrypt.push(1)   
                }
            }
            console.log("message = ",  messDecrypt);
        }
    }
//fin fonction cryptage image
  
    
    
    
    
    
    


//fonction de cryptage ,image dans une image
    function crypterImage () {        
//récupération et transcription du message en binaire 
        if (controle) {
            //récupère le message
            var message = document.getElementById('message').value+"";
            // longueur du message
            var msg_len = message.length; 
            var msg_bin = "";
            //traduit le message en code ascii (hexa)
            for (var k = 0; k < msg_len; k++){
                var lettre_ascii = message.charCodeAt(k); //lettre dans son code ascii
                var lettre_bin = lettre_ascii.toString(2) + ""; // + "" permet d'avoir une chaine de caractère (en binaire)
                while (1) {
                    //si la longueur de la lettre en bin fait 8, sors de la boucle
                    if (lettre_bin.length == 8) {
                        break;
                    //sinon rajoute assez de 0 pour arriver à des lettres codés sur 1 octet
                    } else {
                    lettre_bin = "0" + lettre_bin;
                    }
                }
                //le message entier en binaire est la somme des lettres
                msg_bin = msg_bin + lettre_bin;
            }



            //mets le canvas à la taille de l'image
            canvas.height = image.height;
            canvas.width = image.width;
            //dessine l'image dans le canvas
            environnement.drawImage(image, 0, 0);
            //récupère les données de l'image (composantres RVBA de chaque px)
            var imageDonnees = environnement.getImageData(0, 0, canvas.width, canvas.height);
            //crée un tableau avec ces données
            var tableauPixel = imageDonnees.data;
            //copie du tableau qui agit comme sauvegarde (ne sera pas changée)
            var tableauPixel1 = tableauPixel.slice(0, tableauPixel.length);





    //insertion du message dans l'image
            console.log("message en binaire = ", msg_bin);
            //pour chaque pixel 
            for(var k = 0; k < tableauPixel.length; k += 4) {

                var i = k/4;

    //les différentes possibilités
                if (msg_bin[i] == 0 && tableauPixel[k]%2 == 0) {
                    //ne rien faire, la composante couleur est déjà paire 
                } 

                //si bit = 0 et composante impaire:
                else if (msg_bin[i] == 0 && tableauPixel[k]%2 == 1) {
                    //si coueleur noir (0) on ne peut que ajouter 1
                    if (tableauPixel[k] == 0) {
                        tableauPixel[k] = tableauPixel[k] + 1;
                    //sinon on retire 1 (car si couleur blanche (255) il y a depassement de capacité )
                    } else {
                        tableauPixel[k] = tableauPixel[k] - 1;
                    }
                } 

                //si bit = 1 et composante pair
                else if (msg_bin[i] == 1 && tableauPixel[k]%2 == 0) { // % = modulo (reste de la division euclidienne)
                    //si coueleur noir (0) on ne peut que ajouter 1
                    if (tableauPixel[k] == 0) {
                        tableauPixel[k] = tableauPixel[k] + 1;
                    //sinon on retire 1 (car si couleur blanche (255) il y a depassement de capacité )
                    } else {
                        tableauPixel[k] = tableauPixel[k] + 1;
                    }


                } else if (msg_bin[k] == 1 && tableauPixel[k]%2 == 1) {
                    //ne rien faire, composante couleur déjà impaire
                }

            }
            //affiche l'image avec le message caché
            environnement.putImageData(imageDonnees, 0, 0);




            //vérification du message
            console.log(tableauPixel1);
            console.log(tableauPixel);
            var long = tableauPixel.length; //longueur du tableau
            var messDecrypt = [];
            for (var z=0 ; z < long; z += 4) {
                if (tableauPixel[z]%2 == 0) {
                    messDecrypt.push(0);
                } else {
                    messDecrypt.push(1)   
                }
            }
            console.log("message = ",  messDecrypt);
        }
    }
//fin fonction cryptage image
    
    
    
 
 
    

    
    
    //crypte quand on appuie sur entrée
    document.addEventListener('keydown', function(event) {
        var keyName = event.key;
        if (keyName === 'Enter') {
            //récupère la valeur du select :  image ou message
            if (choix == "Un Message") {
                crypterMessage();
            } else if (choix == "Une Image") {
                crypterImage();
            }
            image.style.display = "none";
            canvas.style.display = "block";
            canvas2.style.display = "block";
        } 
    });
    
    
    //crypte au clic sur le bouton crypter
    document.getElementById("crypter").addEventListener("click", function () {
        if (choix == "Un Message") {
            crypterMessage();
        } else if (choix == "Une Image") {
            crypterImage(); 
        }
        image.style.display = "none";
        canvas.style.display = "block";
        canvas2.style.display = "block";
    });
   
    //bouton enregister l'image            
        bouton.addEventListener("click", function (e) {  
            bouton.download="image.png";  //Nom du fichier que l'on télécharge par défaut
            bouton.href = canvas.toDataURL("image/png"); //La bouction toDataURL() réecris le canvas sous forme de texte en PNG
        }, false);
    
    
});


