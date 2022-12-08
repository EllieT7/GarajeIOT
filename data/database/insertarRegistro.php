<?php
    if(isset($_GET["estado"])) {
        $estado = $_GET["estado"];

        $servername = "localhost";
        $username = "root";
        $password = "";
        $database_name = "garaje_inteligente";
     
        // Crea coneccion MySQL desde PHP al servidor MySQL 
        $connection = new mysqli($servername, $username, $password, $database_name);
        // Verificando coneccion 
        if ($connection->connect_error) {
           die("Conexión MySQL fallida: " . $connection->connect_error);
        }
     
        // Insertar datos en la tabla
        $sql = "INSERT INTO `registro` (`garaje_id`, `temperatura_id`, `humedad_id`, `dia`, `hora`, `estado`) VALUES (1, (SELECT MAX(temperatura_id) FROM temperatura), (SELECT MAX(humedad_id) FROM humedad), CURDATE(), CURTIME(),'$estado')";
        
        if ($connection->query($sql) === TRUE) {
           echo "Nuevo registro creado con éxito";
        } else {
           echo "Error: " . $sql . " => " . $connection->error;
        }
     
        $connection->close();
    } else {
        echo "El estado no está configurado en la solicitud HTTP.";
    }
?>