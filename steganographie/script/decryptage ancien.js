window.addEventListener("load", function () {
    
    
    //récupère l'image et l'affiche
    
    var fichier = document.getElementById("image0");
    var image = document.getElementById("image");

    fichier.addEventListener("change", function () {
  
        var file = this.files[0];        
        var reader = new FileReader();
        image.style.display = "block";
        reader.addEventListener('load', function() {            
            image.src = this.result;         
        });

        reader.readAsDataURL(file);

    }); 
    

    function decrypter() {
        // définit le canvas
        var canvas = document.getElementById("canvas");
        var environnement = canvas.getContext("2d");
        var msg = [];
        canvas.height = image.height;
        canvas.width = image.width; 
        //dessine l'image dans le canvas
        environnement.drawImage(image, 0, 0);
        //récupère les données de l'image (composantres RVBA de chaque px)
        var imageDonnees = environnement.getImageData(0, 0, canvas.width, canvas.height);
        //crée un tableau avec ces données
        var tableauPixel = imageDonnees.data;
        //console.log(tableauPixel);
        //récupère les bits de poids faible
        var bit = [];
        var msg_len = document.getElementById("length").value;
        //console.log("longeur du msg : " + msg_len);
        //stocke les valeurs jusqu'à 8 (8 * 4 = 32; nb de compasantes RVBA)
        for (var k = 0; k < msg_len; k++){
            for (var y = 0; y < 32 ; y += 4){
            var i = k * 32 + y;
                
            /* if (tableauPixel[i] == 0 && tableauPixel[i + 1] == 0 && tableauPixel[i + 2] == 0){
                bit.push(0);
            } else {
                bit.push(1);
            }
            */
            if (tableauPixel[i]%2 == 0){
                bit.push(0);
            } else if (tableauPixel[i]%2 == 1){
                bit.push(1);
            } else {
                console.log("erreur");
            }
           
            
        };          
            //le caractère entier en binaire est la somme des bits
            var octet = bit.join("");
            //convertit l'octet en hexa (1 caractère)
            var deci = parseInt(octet, 2);
            //convertit en caractère avec son code ASCII
            var caractere = String.fromCharCode(deci);
            //stocke le caractère dans une variable
            msg.push(caractere);
            bit = [];
            };
        

        
        //affiche le message
        var message = document.getElementById("message");
        var mot = msg.join("");
        message.value = mot;
        
    }
    
    
    //decrypte quand on appuie sur entrée
    document.addEventListener('keydown', function(event) {
        var keyName = event.key;
        if (keyName === 'Enter') {
            decrypter();
          } 
    });
    
    
    //decrypte au clic sur le bouton
    document.getElementById("decrypter").addEventListener("click", function () {
        decrypter();
    }); 
    
    
});