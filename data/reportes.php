<?php
    $con = mysqli_connect('localhost', 'root', '', 'garaje_inteligente');
    $sql = "SELECT `registro_id`, `dia`, `hora`, `estado` FROM `registro` ORDER BY `registro_id`";
    $resultado = $con->query($sql);
    $sql2 = "SELECT `temperatura_id`, `dia`, `hora`, `valor` FROM `temperatura` ORDER BY `temperatura_id`";
    $resultado2 = $con->query($sql2);
    $sql3 = "SELECT `humedad_id`, `dia`, `hora`, `valor` FROM `humedad` ORDER BY `humedad_id`";
    $resultado3 = $con->query($sql3);
?>
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8"/>
    <meta http-equiv="X-UA-Compatible" content="IE=edge"/>
    <meta name="viewport" content="width=device-width, initial-scale=1.0"/>
    <script src="https://code.jquery.com/jquery-2.2.4.js" integrity="sha256-iT6Q9iMJYuQiMWNd9lDyBUStIq/8PuOW33aOqmvFpqI=" crossorigin="anonymous"></script>
    <link rel="stylesheet" href="./reportes.css"/>
    <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-rbsA2VBKQhggwzxH7pPCaAqO46MgnOM80zW1RWuH61DGLwZJEdK2Kadq2F9CUG65" crossorigin="anonymous"/>
    <title>Reportes | Garaje Inteligente</title>
  </head>
  <body>
    <!--Barra de navegación-->
    <nav class="sidebar-navigation">
    <ul>
        <li onclick="location.href='index.html'">
            <i class="fa fa-house"></i>
            <span class="tooltip">Inicio</span>
        </li>
        <li onclick="location.href='dashboard.html'">
            <i class="fa fa-square"></i>
            <span class="tooltip">Dashboard</span>
        </li>
        <li class="active" onclick="location.href='reportes.html'">
            <i class="fa fa-file"></i>
            <span class="tooltip">Reportes</span>
            <div class="indicador"></div>
        </li>
    </ul>
    </nav>

    <!--Contenido-->
    <div id="contenido">
        <div style="height: 20px"></div>
        <div class="contain text-center">
            <div class="row">
                <div class="col-8 texto">
                    <h1>Reportes | Garaje Inteligente</h1>
                </div>
                <div class="col">
                <img src="https://github.com/MicaelaGordillo/images_IoT/blob/main/garaje/fondo.png?raw=true" alt="" width="100%">
                </div>
            </div>
        </div>
        <br><br>

        <!--Tabla - Entradas y Salidas-->
        <div class="tabla">
            <table>
                <tr>
                    <th colspan="4">ENTRADAS Y SALIDAS</th>
                </tr>
                <tr>
                    <th>NRO</th>
                    <th>ACCIÓN</th>
                    <th>DÍA</th>
                    <th>HORA</th>
                </tr>
                <?php 
                while($row = $resultado->fetch_array(MYSQLI_ASSOC)) { ?>
					<tr>
						<td><?php echo $row['registro_id']; ?></td>
                        <td><?php 
                        if($row['estado']==1)
                            echo "Entrada";
                        else
                            echo "Salida"; ?>
                        </td>
                        <td><?php echo $row['dia']; ?></td>
                        <td><?php echo $row['hora']; ?></td>
                    </tr>
                <?php } 
                ?>
            </table>
        </div>
        <br><br>

        <div class="tabla" style="display: grid; grid-template-columns: repeat(2, 1fr); gap: 50px;">
            <!--Tabla - Temperatura-->
            <table>
                <tr>
                    <th colspan="4">TEMPERATURA</th>
                </tr>
                <tr>
                    <th>NRO</th>
                    <th>TEMPERATURA</th>
                    <th>DÍA</th>
                    <th>HORA</th>
                </tr>
                <?php 
                while($row2 = $resultado2->fetch_array(MYSQLI_ASSOC)) { ?>
					<tr>
						<td><?php echo $row2['temperatura_id']; ?></td>
                        <td><?php echo $row2['valor']; ?> °C</td>
                        <td><?php echo $row2['dia']; ?></td>
                        <td><?php echo $row2['hora']; ?></td>
                    </tr>
                <?php } 
                ?>
            </table>
            <!--Tabla - Humedad-->
            <table>
                <tr>
                    <th colspan="4">HUMEDAD</th>
                </tr>
                <tr>
                    <th>NRO</th>
                    <th>HUMEDAD</th>
                    <th>DÍA</th>
                    <th>HORA</th>
                </tr>
                <?php 
                while($row3 = $resultado3->fetch_array(MYSQLI_ASSOC)) { ?>
					<tr>
						<td><?php echo $row3['humedad_id']; ?></td>
                        <td><?php echo $row3['valor']; ?> g/m3</td>
                        <td><?php echo $row3['dia']; ?></td>
                        <td><?php echo $row3['hora']; ?></td>
                    </tr>
                <?php } 
                ?>
            </table>
        </div>
    </div>
    <br><br>

    <!--Boton PDF-->
    <div class="text-center">
        <button class="btn btn-primary" onclick="generarPDF()">Generar PDF</button>
    </div>

    <!--Scripts-->
    <script src="https://cdnjs.cloudflare.com/ajax/libs/html2pdf.js/0.10.1/html2pdf.bundle.min.js" integrity="sha512-GsLlZN/3F2ErC5ifS5QtgpiJtWd43JWSuIgh7mbzZ8zBps+dvLusV+eNQATqgA/HdeKFVgA5v3S/cIrLF7QnIg==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
    <script src="html2pdf.bundle.min.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/js/bootstrap.bundle.min.js" integrity="sha384-kenU1KFdBIe4zVF0s0G1M5b4hcpxyD9F7jL+jjXkk+Q2h455rYXK/7HAuoJl+0I4" crossorigin="anonymous"></script>
    <script src="https://kit.fontawesome.com/d32f40f0ab.js" crossorigin="anonymous"></script>
    <script>
        $('ul li').on('click', function() {
            $('li').removeClass('active');
            $('.indicador').visible = false;
            $(this).addClass('active');
            $(this).find('.indicador').visible = true;
        });

        function generarPDF() {
            var element = document.getElementById('contenido');
            var opt = {
                margin:       1,
                filename:     'reporteYMN.pdf',
                image:        { type: 'png', quality: 0.98 },
                html2canvas:  { scale: 2 },
                jsPDF:        { unit: 'in', format: 'letter', orientation: 'portrait' }
            };
            html2pdf().set(opt).from(element).save();
        }
    </script>
  </body>
</html>