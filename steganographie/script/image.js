window.addEventListener("load", function () {

    "use strict";



    //définis le canvas
    var canvas = document.getElementById("canvas");
    var canvas_cache = document.getElementById("canvas_cache");
    var fin_process = document.getElementById("fin_process");
    var environnement = canvas.getContext("2d");
    var environnement_cache = canvas_cache.getContext("2d");
    //récuperer image
    var image = document.getElementById("image");
    //variable qui étudie la validité de l'image
    var controle = true;
    //mets le canvas à la taille de l'image
    canvas.height = image.height;
    canvas.width = image.width;
    canvas_cache.height = image.height;
    canvas_cache.width = image.width;
    document.getElementById("canvas_cache").style.display = "none";
    //bouton enregistrer
    var bouton = document.getElementById("enregistrer");


    //n'affiche pas le canvas
    canvas.style.display = "none";
    var choix = "Un Message";
    //Affichage des options en fonction du choix:
    //cache la 2eme image
    document.getElementById("image_cacher").style.display = "none";
    //quand on veut cacher une image, affiche le 2eme bouton parcourir
    document.getElementById("choix").addEventListener("change", function () {
        fin_process.style.display = "none";
        image.style.display = "block";
        image.src = "../images/anonymous.png";
        //récupère la valeur du select :  image ou message
        choix = document.getElementById('choix').value;
        if (choix == "Un Message") {
            document.getElementById("image_cacher").style.display = "none";
            document.getElementById("messageForm").style.display = "block";
            document.getElementById("image2").style.display = "none";
            document.getElementById("canvas_cache").style.display = "none";
        } else if (choix == "Une Image") {
            document.getElementById("image2").style.display = "block";
            document.getElementById("image_cacher").style.display = "block";
            document.getElementById("messageForm").style.display = "none";
        }
    })






    //récupère l'image que l'on cache et l'affiche au clic 
    var image_cache = document.getElementById("image_cache");
    var image2 = document.getElementById("image2"); //canvas image cachée
    image_cache.addEventListener("change", function () {
        var image_cache = this.files[0]; //tableau "files" contient tous les fichiers sélectionnés       
        var reader = new FileReader(); //lit l'image'
        reader.addEventListener('load', function () {
            image2.src = this.result; //affiche l'image
        });
        reader.readAsDataURL(image_cache);
        image2.style.display = "block";
        canvas_cache.style.display = "none";
        fin_process.style.display = "none";
    });




    //récupère l'image où l'on cache l'image et l'affiche au clic 
    var fichier = document.getElementById("image_upload"), //bouton "parcourir"
        image = document.getElementById("image"); //canvas image
    //quand un fichier est choisit, le charge automatiquement
    fichier.addEventListener("change", function () {
        console.log(fichier.value);
        //affiche l'image
        image.style.display = "block";
        //n'affiche plus le message "image cryptée"
        fin_process.style.display = "none";
        //n'affiche pas le canvas
        canvas.style.display = "none";

        var poids = this.files[0]["size"]; //poids du fichier
        var type = this.files[0]["type"]; //type du fichier
        //nombre de caractère maximal
        var caractereMax = poids / 32;

        //vérifie le type de l'image
        type = type.split("");
        var format = [];
        for (var i = 0; i < 5; i++) {
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
        if (type == "image" && controle) {
            var file = this.files[0]; //tableau "files" contient tous les fichiers sélectionnés       
            var reader = new FileReader(); //lit le fichier
            image.style.display = "block";
            reader.addEventListener('load', function () {
                image.src = this.result; //affiche l'image
            });
            reader.readAsDataURL(file);
        } else {
            if (type != "image") {
                alert("Ce fichier n'est pas une image!");
            }
        }
        image2.style.display = "block";
        canvas_cache.style.display = "none";
        fin_process.style.display = "none";


    });






    //fonction de cryptage , message dans une image
    function crypterMessage() {
        //récupération et transcription du message en binaire 
        if (controle) {
            //récupère le message
            var message = document.getElementById('message').value + "";
            // longueur du message
            var msg_len = message.length;
            var msg_bin = "";
            //traduit le message en code ascii (hexa)
            for (var k = 0; k < msg_len; k++) {
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
            for (var k = 0; k < tableauPixel.length; k += 4) {

                var i = k / 4;

                //les différentes possibilités
                if (msg_bin[i] == 0 && tableauPixel[k] % 2 == 0) {
                    //ne rien faire, la composante couleur est déjà paire 
                }

                //si bit = 0 et composante impaire:
                else if (msg_bin[i] == 0 && tableauPixel[k] % 2 == 1) {
                    //si coueleur noir (0) on ne peut que ajouter 1
                    if (tableauPixel[k] == 0) {
                        tableauPixel[k] = tableauPixel[k] + 1;
                        //sinon on retire 1 (car si couleur blanche (255) il y a depassement de capacité )
                    } else {
                        tableauPixel[k] = tableauPixel[k] - 1;
                    }
                }

                //si bit = 1 et composante pair
                else if (msg_bin[i] == 1 && tableauPixel[k] % 2 == 0) { // % = modulo (reste de la division euclidienne)
                    //si coueleur noir (0) on ne peut que ajouter 1
                    if (tableauPixel[k] == 0) {
                        tableauPixel[k] = tableauPixel[k] + 1;
                        //sinon on retire 1 (car si couleur blanche (255) il y a depassement de capacité )
                    } else {
                        tableauPixel[k] = tableauPixel[k] + 1;
                    }


                } else if (msg_bin[k] == 1 && tableauPixel[k] % 2 == 1) {
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
            for (var z = 0; z < long; z += 4) {
                if (tableauPixel[z] % 2 == 0) {
                    messDecrypt.push(0);
                } else {
                    messDecrypt.push(1)
                }
            }
            console.log("message = ", messDecrypt);
        }
    }
    //fin fonction cryptage image




    //fonction de cryptage ,image dans une image
    function crypterImage() {
        //récupération et transcription du message en binaire 
        if (controle) {


            //récupère les données de l'image à cacher
            //mets le canvas_cache à la taille de l'image où l'on cache
            canvas_cache.height = image2.height;
            canvas_cache.width = image2.width;
            //dessine l'image dans le canvas
            environnement_cache.drawImage(image2, 0, 0);
            //récupère les données de l'image (composantes RVBA de chaque px)
            var imageDonnees2 = environnement_cache.getImageData(0, 0, canvas_cache.width, canvas_cache.height);
            //crée un tableau avec ces données
            var tableauPixel2 = imageDonnees2.data;
            //copie du tableau qui agit comme sauvegarde (ne sera pas changée)
            var tableauPixel3 = tableauPixel2.slice(0, tableauPixel2.length);



            //mets le canvas à la taille de l'image où l'on cache
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
            canvas.style.display = "block";


            /*var px_a_cacher = tableauPixel2[3].toString(2) + "";
            var test = px_a_cacher.slice(0,4);
            console.log(typeof test);*/
            


            //insertion de l'image dans l'image
            //pour chaque pixel 
            for (var k = 0; k < tableauPixel.length; k += 4) {
                for (var i = 0; i < 3; i++) {
                    var px_a_cacher = (tableauPixel2[k + i].toString(2) + "").slice(0, 4);
                    while (1) {
                        //si la longueur de la lettre en bin fait 8, sors de la boucle
                        if (px_a_cacher.length == 4) {
                            break;
                            //sinon rajoute assez de 0 pour arriver à des lettres codés sur 1 octet
                        } else {
                            px_a_cacher = "0" + px_a_cacher;
                        }
                    }
                    var px_ou_Cacher = (tableauPixel[k + i].toString(2) + "").slice(0, 4);
                    while (1) {
                        //si la longueur de la lettre en bin fait 8, sors de la boucle
                        if (px_ou_Cacher.length == 4) {
                            break;
                            //sinon rajoute assez de 0 pour arriver à des lettres codés sur 1 octet
                        } else {
                            px_ou_Cacher = "0" + px_ou_Cacher;
                        }
                    }
                    tableauPixel[k + i] = parseInt(px_ou_Cacher+ "" + px_a_cacher);
                }
            }

            //affiche l'image avec le message caché
            environnement.putImageData(imageDonnees, 0, 0);


            console.log(px_a_cacher);
            console.log(px_ou_Cacher);
            console.log(tableauPixel[ tableauPixel.length-1]);

        }
    }
    //fin fonction cryptage image









    //crypte quand on appuie sur entrée
    document.addEventListener('keydown', function (event) {
        var keyName = event.key;
        if (keyName === 'Enter') {
            //récupère la valeur du select :  image ou message
            if (choix == "Un Message") {
                crypterMessage();
                canvas.style.display = "block";
                canvas_cache.style.display = "none";
            } else if (choix == "Une Image") {
                crypterImage();
                canvas.style.display = "block";
                canvas_cache.style.display = "none";
                image2.style.display = "none";
            }
            image.style.display = "none";
            fin_process.style.display = "block";
        }
    });


    //crypte au clic sur le bouton crypter
    document.getElementById("crypter").addEventListener("click", function () {
        if (choix == "Un Message") {
            crypterMessage();
            canvas.style.display = "block";
            canvas_cache.style.display = "none";
        } else if (choix == "Une Image") {
            crypterImage();
            canvas.style.display = "block";
            canvas_cache.style.display = "none";
            image2.style.display = "none";
        }
        image.style.display = "none";
        fin_process.style.display = "block";
    });

    //bouton enregister l'image            
    bouton.addEventListener("click", function (e) {
        bouton.download = "image.png"; //Nom du fichier que l'on télécharge par défaut
        bouton.href = canvas.toDataURL("image/png"); //La bouction toDataURL() réecris le canvas sous forme de texte en PNG
    }, false);


});
