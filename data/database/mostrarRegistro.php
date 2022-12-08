<?php
    header('Access-Control-Allow-Origin: *');
    header('Access-Control-Allow-Headers: Origin, X-Requestes-Whit, Content-Type, Accept');
    header('Content-Type: application/json');

    $con = mysqli_connect('localhost', 'root', '', 'garaje_inteligente');

    $registro=mysqli_query($con ,"SELECT `registro_id`, `dia`, `hora`, `estado` FROM `registro` ORDER BY `registro_id`");
    $vec=[];
    while($reg=mysqli_fetch_array($registro)){
        $vec[]=$reg;
    }
    $cad = json_encode($vec);
    echo $cad;
?>