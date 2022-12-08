<?php
    if(isset($_GET["humedad"])) {
        $humedad = $_GET["humedad"]; // toma la humedad del HTTP GET
     
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
     
        $sql = "INSERT INTO `humedad` (`dia`, `hora`, `valor`) VALUES (CURDATE(), CURTIME(), $humedad)";
     
        if ($connection->query($sql) === TRUE) {
           echo "Nuevo registro creado con éxito";
        } else {
           echo "Error: " . $sql . " => " . $connection->error;
        }
     
        $connection->close();
    } else {
        echo "Humedad no está configurado en la solicitud HTTP.";
    }
?>