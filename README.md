![poster](https://github.com/joanvidal89/mz_chess/assets/161112464/e2985d84-4ff9-44b1-a6f2-988fbe33ac9f)

<div style="page-break-after: always;"></div>

# Índice

### · Introducción
### · Descripción del Proyecto
### · Viabilidad del Proyecto
### · Software y Herramientas
### · Módulos de Software Desarrollados
### · Tiempo de Desarrollo
### · Propuestas de Mejora
### · Conclusiones
### · Anexo

<div style="page-break-after: always;"></div>

# Introducción

<br>

### Objetivos del Proyecto

&emsp;MZ Chess se concibe como un proyecto para aprender programación a medio nivel utilizando C++.

&emsp;Consiste en desarrollar de un videojuego de ajedrez, un servicio API Restful y una base de datos SQLite.

<br>

### Contexto y Justificación del Proyecto

#### ¿Por qué ajedrez?
&emsp;El ajedrez es un juego milenario reconocido por su complejidad estratégica y sus beneficios cognitivos. En los últimos años, su popularidad ha crecido notablemente gracias a plataformas online y de streaming, aumentando su alcance y atractivo.

#### C++ y Raylib
&emsp;C++ ofrece un alto rendimiento y control sobre el hardware, ideal para desarrollo de videojuegos. De hecho, la gran mayoría de motores, privados o no, funcionan bien directamente o bien sobre una capa de C++.

&emsp;Raylib es una librería ligera y sin dependencias externas, adecuada para proyectos educativos y prototipos rápidos.

<div style="page-break-after: always;"></div>

# Descripción del Proyecto

### Visión general del videojuego

&emsp; El videojuego MZ Chess comienza siendo un juego en 2D con estética retro (Pixel Art) ya que, además de que resulta un estilo artístico atractivo que apela a la nostalgia, su baja resolución nativa implica un menor tiempo necesario para crear assets, y simplifica la programación.

&emsp;No obstante, aunque mantiene un espíritu "retro", la versión final de MZ Chess dista enormemente de esta idea inicial. Tratándose pues de un videojuego tridimensional con capacidades gráficas avanzadas.

### Funcionalidades Principales

&emsp;· Servicio web (API RESTful) en C++.  
&emsp;· Sistema de cuentas y puntuaciones usando SQLite3.  
&emsp;· MZ Launcher, cliente 2D para creación de cuentas y inicio de sesión.  
&emsp;· MZ Game, el videojuego de ajedrez.  
&emsp;&emsp;· Realización de la arquitectura y flujo del motor.  
&emsp;&emsp;· Creación de las assets, tanto 2D como 3D.  
&emsp;&emsp;· Sistema de comunicación con motor de inteligencia artificial.

### Alcance del Proyecto

&emsp;La realización de un juego tridimensional, por simple que resulte conceptualmente, requiere de mucho tiempo y esfuerzo.  

&emsp;La dificultad del proyecto radica en programar a medio-bajo nivel y evitando en lo posible utilizar librerías, motores u otras herramientas que opaquen el aprendizaje.

&emsp;Por esto, se ha intentado mantener la complejidad de las funcionalidades y servicios a niveles básicos, para que resultase factible realizar el proyecto en el tiempo limitado del que se dispone.

<div style="page-break-after: always;"></div>

# Viabilidad del Proyecto

### Análisis del Mercado

&emsp;La demanda de los videojuegos de ajedrez ha aumentado exponencialmente en los últimos años, con plataformas competitivas en línea como "chess.com" o "lichess.com" alcanzando 15 millones de usuarios, y títulos educativos como "Dr.Wolf" o "Chessable" obteniendo millones de descargas.

&emsp;Estas plataformas se han asentado como dominantes y resulta completamente inviable hacerse un hueco en el mercado del ajedrez competitivo.  

&emsp;MZ Chess no pretende entrar en dicho espacio y ofrece una experiencia arcade, inspirada en los juegos de la etapa temprana de los años 90: modo un jugador contra la IA basado en niveles de dificultad y un sistema de puntuaciones.

### Viabilidad Técnica y Económica

&emsp;Los recursos necesarios para la realización del proyecto son, única y exclusivamente, tiempo.  

&emsp;Todo lo necesario para el desarrollo del proyecto se realiza bajo el correcto uso de los respectivos términos de licencia de cada una de las herramientas empleadas. Y todas ellas son completamente gratuítas.

&emsp;En cuanto a monetización, obviamente el proyecto será de uso gratuíto y libre, y el código fuente descargable, modificable y redistribuíble desde la plataforma Github. *(Idealmente, éste proyecto podría servir de ayuda o inspiración para personas que quieran aprender a programar usando Raylib y C++)*.

&emsp;Suponiendo que se quisiera monetizar, se publicaría en una o varias plataformas de videojuegos online, con un precio reducido (acorde a los precios de otros juegos independientes) y una filosofía de "Buy once, play forever" (compra una única vez, juega para siempre). 

&emsp;*Me gustaría, a título personal, dejar expresamente claro que NUNCA y POR NINGÚN MOTIVO se incluirían prácticas de monetización predatorias (tan comúnmente utilizadas a día de hoy) como LOOT-BOXES, SEASON-PASS o PAY-TO-WIN.*

# Software y Herramientas

### C++

&emsp;Un lenguaje de programación de alto nivel y multiparadigma utilizado para el desarrollo de software.

&emsp;*Lenguaje principal de programación para implementar la lógica del videojuego, el motor, el servicio Restful, la comunicación FTP, etc.*

<br>

### Raylib

&emsp;Una librería header-only de renderizado gráfico, de enlace con el sistema y de audio escrita en C, diseñada para ser ligera.

&emsp;*Utilizado para renderizar gráficos en 3D, incluyendo modelos, texturas y animaciones, para manejar la interacción con el usuario y para la gestión del audio.*

<br>

### Httplib

&emsp;Una librería header-only de C++ para realizar solicitudes HTTP de manera sencilla y eficiente.

> [!NOTE]
> Originalmente se intentó realizar toda la comunicación FTP usando únicamente librerías del sistema Windows.  
<br>Debido a la complejidad y el coste de implementar encriptado SSL en la comunicación, se optó por una librería de terceros que ya ofrecía dicha funcionalidad. 

&emsp;*Empleado para la comunicación online del videojuego, incluyendo solicitudes a una API REST para la gestión de cuentas y puntuaciones.*

<br>

<div style="page-break-after: always;"></div>

# Software y Herramientas

### SQLite3

&emsp;Una biblioteca escrita en C que implementa un motor de base de datos SQL compacto y autosuficiente.

&emsp;*Utilizado para la gestión de datos locales, como la información de usuarios y puntuaciones, almacenada en una base de datos SQLite integrada en la aplicación.*

<br>

### Visual Studio Code

&emsp;Un editor de código fuente potente y ligero, con soporte para múltiples lenguajes de programación. 

&emsp;*Se ha utilizado para la edición y gestión del código C++ y GLSL.*

<br>

### MinGW 64

&emsp;Un compilador de C/C++ para Windows que proporciona un entorno de desarrollo completo para la compilación de aplicaciones. 

&emsp;*Se ha usado para generar los distintos ejecutables del proyecto, enlazar iconos y escribir metadatos.*

<br>

### Blender

&emsp;Una herramienta de software libre para la creación de gráficos 3D, incluyendo modelado, texturizado, animación y renderizado. 

&emsp;*Se usa para el diseño de los modelos 3D, la creación de texturas y UVs, pruebas de iluminación y animación...*

<br>

<div style="page-break-after: always;"></div>

# Software y Herramientas

### GIMP

 &emsp;Un editor de imágenes de código abierto utilizado para la edición y manipulación de gráficos rasterizados. 
 
 &emsp;*Se utiliza para el diseño y edición de textura e imágenes, principalmente las de la UI del videojuego y el "Launcher".*

<br>

### UTILIDADES

&emsp;**DBBrowser for SQLite**: Herramienta de código abierto para crear, buscar y editar ficheros de base de datos SQLite.

&emsp;**Postman**: Herramienta para crear y usar APIs.

<div style="page-break-after: always;"></div>

# Tablas de las Bases de Datos

&emsp;El diseño de la base de datos es sencillo. Como ya se ha indicado, el proyecto no se ha centrado en la complejidad de los sistemas.

Incluye dos tablas:

ACCOUNT
---
|COLUMN    | TYPE         | CONSTRAINTS             |
| -------- | ------------ | ----------------------- |
|ID        |INTEGER       |PRIMARY KEY AUTOINCREMENT|
|USERNAME  |TEXT          |UNIQUE NOT NULL          |
|PASSWORD  |TEXT          |NOT NULL                 |
|HINT      |TEXT          |NOT NULL                 |
|DATE      |TEXT          |NOT NULL                 |

HIGHSCORE
---
|COLUMN    | TYPE         | CONSTRAINTS             |
| -------- | ------------ | ----------------------- |
|ID        |INTEGER       |PRIMARY KEY AUTOINCREMENT|
|LEVEL     |INTEGER       |NOT NULL                 |
|ACCOUNT_ID|INTEGER       |FOREIGN KEY NOT NULL     |
|SCORE     |INTEGER       |NOT NULL                 |
|DATE      |TEXT          |NOT NULL                 |

<br>

<div style="page-break-after: always;"></div>

# Módulos de Software Desarrollados

## Client.exe

· **MZServer**: Es utilizado como subproceso por MZChess. Administra las peticiones y respuestas HTTP al servidor. Concretamente, se encarga de enviar nuevos registros de puntuaciones al completar los niveles y de pedir registros de puntuaciones (TOP 8) de cada uno de los niveles.

<br>

## Server.exe

· **MZConfig**: lee de un archivo de configuración los parámetros de inicio (dirección IP y puerto del servidor).  

· **MZDatabase**: encuentra, crea, abre o cierra la base de datos, realiza las queries y maneja la conversión de la respuesta a formato de texto plano para su posterior envío.  

· **MZServer**: inicializa, configura y controla la lógica del servidor.  

<br>

<div style="page-break-after: always;"></div>

# Módulos de Software Desarrollados

## Launcher.exe

· **MZAudio**: carga en memoria archivos de sonido, controla y actualiza los canales de audio, así como el volumen, tono y panning.  

· **MZComm**: administra las peticiones y respuestas HTTP al servidor. Lo hace de forma asíncrona utilizando la librería <future> de C++. Permite al usuario crear nuevas cuentas, utilizar un sistema de recuerdo de contraseña e iniciar sesión.

· **MZConfig**: lee de un archivo de configuración los parámetros de inicio (dirección IP, puerto del servidor, último usuario).  

· **MZExecute**: permite lanzar un proceso en Windows, se encarga de lanzar el juego de MZChess una vez se ha iniciado sesión, pasándole como argumentos aquellas variables necesarias para que continúe la comunicación con el servicio RESTful.  

· **MZInput**: controla la entrada de ratón y teclado, identifica el pulsado de teclas y permite filtrar la validez de una pulsación de tecla en un momento determinado y permite realizar cálculos con la posición real y virtual del ratón para interactuar con elementos de la interfaz de usuario.

· **MZLauncher**: actúa de cerebro del programa, es el nexo de unión entre los distintos elementos y controla el flujo de la aplicación. También configura y lanza la ventana donde se mostrará el lanzador.

· **MZScene**: las escenas controlan, a través de MZLauncher (director) el flujo del hilo principal, realizando las llamadas pertinentes al resto de clases del motor.

· **MZRender2D**: controla la carga en memoria y el dibujado de imágenes, el preparado de texturas NinePatch y la creación y dibujado de fuentes, además de la conversión, mediante Render Textures, entre diferentes resoluciones sin afectar a la resolución de renderizado nativa del programa.  

<div style="page-break-after: always;"></div>

# Módulos de Software Desarrollados

## Game.exe

· **MZAudio**: carga en memoria archivos de sonido, controla y actualiza los canales de audio y música, así como el volumen, tono y panning de los mismos.  

· **MZBoard**: contiene la lógica del juego de ajedrez: controla casillas, piezas, turnos, movimientos de las piezas, legalidad de movimientos, enroques, "en passant", jaques, condiciones de victoria, tablas por inacción, tablas por falta de material.

· **MZComm**: se encarga de lanzar un subproceso del cliente "client.exe", establecer una comunicación i/o a través de tuberías, y utilizar dichas tuberías para comunicarse a través de comandos CMD. Lo hace de forma asíncrona utilizando la librería <future> de C++. 

· **MZComposite**: crea y renderiza la imágen en tiempo de ejecución (Render Texture) en la que se realiza la composición de la imágen final que se muestra en pantalla. Ésta imágen se pasa a un programa de sombreador de fragmento (MZPostPro).

· **MZConfig**: lee de un archivo de configuración los parámetros de inicio (dirección IP, puerto del servidor, opciones gráficas y de sonido).  

· **MZDirector**: actúa de cerebro del programa, es el nexo de unión entre los distintos elementos y controla el flujo de la aplicación, también configura y lanza la ventana donde se mostrará el programa, y la adecuará al tamaño del monitor.  

· **MZInput**: controla la entrada del ratón y permite realizar cálculos de raycasting (conversión del puntero del ratón de un espacio 2D, la pantalla, a un espacio 3D, la escena) y, de nuevo, conversión de rayhits (vectores de impacto de un raycast) a un valor de dos dimensiones para generar una posición virtual del ratón y usarla con los elementos en pantalla.

· **MZMath**: clase de utilidad con funciones usadas de forma general como "eases", interpolaciones lineales, "clamp" y "dampen" de valores.

<div style="page-break-after: always;"></div>

# Módulos de Software Desarrollados

## Game.exe

· **MZPostpro**: programa escrito en GLSL (OpenGL Shading Language) y diseñado para aplicar diversos efectos gráficos a la imágen final que se mostrará en pantalla, realizando operaciones aritméticas sobre los fragmentos y sus coordenadas de textura. Entre los efectos que puede realizar se incluyen: resplandecer, FXAA (aproximación rápida anti-solapamiento), aberración cromática, viñetado y distorsión y límites de imágen.

· **MZRender3D**: realiza la carga y liberado de memoria de mallas 3D, texturas y materiales, sombreadores y se encarga de dibujar modelos (piezas del ajedrez y la máquina arcade) en el espacio tridimensional.

· **MZRenderVS**: crea y renderiza la imágen en tiempo de ejecución (Render Texture) en la que se realiza la composición de la interfaz de usuario que se muestra en las pantallas de la máquina arcade del juego.
Controla la carga en memoria y el dibujado de imágenes, el preparado de texturas NinePatch y la creación y dibujado de fuentes. Finalmente, también se encarga de aplicar dicha textura de renderizado al material de la malla 3D de las pantallas de la máquina arcade.

· **MZScene**: las escenas controlan, a través de MZDirector el flujo del hilo principal, realizando las llamadas pertinentes al resto de clases del motor.

· **MZStockfish**: se encarga de lanzar un subproceso del potente motor de Inteligencia Artificial "stockfish", establecer una comunicación i/o a través de tuberías, y utilizar dichas tuberías para comunicarse a través de comandos UCI (Universal Chess Interface) y nomenclatura FEN (Forsyth-Edwards Notation). Lo hace de forma asíncrona utilizando la librería <future> de C++. 

· **MZStruct**: clase de utilidad que contiene las definiciones de estructuras y enumerados que facilitan la comunicación entre las distintas partes del sector. 

<div style="page-break-after: always;"></div>

# Módulos de Software Desarrollados

### Conflictos Encontrados y Resolución

&emsp;La fase de pre-producción requirió de más tiempo y recursos de los originalmente planeados, ya que se realizaron varias pruebas de arte y diseño (algunas de ellas se llegaron a implementar en versiones rudimentarias del motor) que nunca llegaron a satisfacer los niveles de calidad que se esperaban del proyecto.

&emsp;Se optó, finalmente, por sacrificar tiempo de desarrollo en pos de realizar el videojuego en tres dimensiones, solventando así los problemas de diseño y creación de assets de arte.

&emsp;Diseñar una arquitectura que permita, de forma sencilla, trabajar simultáneamente con inputs (teclado, ratón, respuestas HTTP, respuestas a través de tubería...) y outputs (imágen, sonido, peticiones HTTP y UCI...), así como asegurarse de que el rendimiento es óptimo y se alcanza una tasa mínima de sesenta dibujados de cuadro por segundo, ha sido una tarea realmente complicada.

&emsp;Se han realizado numerosas iteraciones que han ido resolviendo los problemas encontrados, como cuellos de botella, problemas de memoria, problemas de comunicación entre clases o caídas de rendimiento.

<br>

<div style="page-break-after: always;"></div>

# Tiempo de Desarrollo

### Planificación del Proyecto

&emsp;En el diseño se invierte más tiempo del necesario en determinar un estilo artístico, realizando numerosas pruebas que no funcionan. Una vez elegido, la producción de assets se completa de forma muy eficiente, por lo que se compensa tiempo perdido.

&emsp;La fase de desarrollo lleva el tiempo esperado. Se es consciente de que la programación de un motor de juego tridimensional es una tarea realmente compleja.

&emsp;Se estima inicialmente un tiempo de desarrollo de 400 horas y se completa el desarrollo en ese tiempo. Aunque es cierto que algunas funcionalidades tuvieron que posponerse y se implementarán una vez presentado el TFC.

&emsp;No hay como tal una fase de pruebas, ya que se va comprobando la funcionalidad y se buscan vulnerabilidades mientras se desarrolla el proyecto. No obstante, se han completado más de 50 partidas de ajedrez contra distintos oponentes, y no se ha encontrado ningún fallo.

<br>

<div style="page-break-after: always;"></div>

# Propuestas de Mejora

### Aspectos Técnicos

&emsp;**Mejoras generales en el rendimiento y la arquitectura.**  
Entre otros, optimizar el código, minimizar instrucciones y reestructurar el flujo del programa en las escenas.

&emsp;**Mejoras en la seguridad de la comunicación y servicios.**  
Una alternativa sería utilizar el lenguaje "Rust", ideal para tratar con comunicación FTP y creación de servicios.

&emsp;**Optimización del código y la lógica del ajedrez.**  
La búsqueda de movimientos posibles se realiza de forma iterativa sobre una array de dos dimensiones. Se puede obtener de forma más óptima utilizando algoritmos, mapas de bits y valores conocidos como "números mágicos". Se consideró que su uso era demasiado complejo.

&emsp;**Mejoras en el rendimiento de los efectos de post-procesado.**  
Se puede implementar un método para trabajar sobre una Render Texture con un LOD (nivel de detalle) más bajo, lo que resulta en menos píxeles a procesar, y posteriormente reescalar.

<div style="page-break-after: always;"></div>

# Propuestas de Mejora

### Funcionalidades Adicionales

&emsp;**Implementación de un modo de juego de "Un Jugador" más completo.**  
Con nuevos niveles, retos, recompensas cosméticas, un modo de juego aventura (Rogue-Like), puzzles...

&emsp;**Implementación de un modo 1vs1 hot-seat.**  
Tal y como está diseñada la lógica del juego, resultaría muy simple realizar un modo de dos jugadores en el mismo PC.

&emsp;**Implementación de un modo 1vs1 en línea.**  
Sin adentrarse mucho en el marco competitivo, se podría realizar un simple sistema de ELO para partidas rápidas 1vs1 a través de la red.

<br>

<div style="page-break-after: always;"></div>

# Conclusiones

<br>

&emsp;El proyecto MZ Chess ha sido una iniciativa altamente ambiciosa, donde se ha desarrollado una estructura de clases que emula, de manera básica, el funcionamiento de motores de videojuegos comerciales como Unity, Unreal Engine y Godot.

&emsp;En este proyecto, he abordado la gestión de memoria, la carga de recursos, el control de objetos, el renderizado de escenas 3D, la gestión de entradas, la implementación de elementos de la interfaz de usuario... todo esto ha sido concebido y desarrollado desde cero.

&emsp;Este trabajo ha sido fundamental para el desarrollo de mis habilidades como programador, permitiéndome aprender a estructurar y optimizar código a un nivel más bajo del que estaba acostumbrado.  

&emsp;Aunque el resultado final puede no ser tan complejo como otros proyectos en los que he trabajado este año, la experiencia ha sido extremadamente satisfactoria a nivel personal.

&emsp;He descubierto, además, que cuanto más control me ofrece un lenguaje, más disfruto del proceso de programación. En este sentido, C++ se ha convertido en uno de mis lenguajes preferidos para proyectos personales.

<br>

<div style="page-break-after: always;"></div>

# Anexos

<br>

### Bibliografía

Referencias y recursos utilizados durante el desarrollo del proyecto.

**C++**  
https://cplusplus.com/  
https://stackoverflow.com/

**Raylib**  
https://www.raylib.com/cheatsheet/cheatsheet.html
https://www.raylib.com/cheatsheet/raymath_cheatsheet.html

**httplib**  
https://github.com/yhirose/cpp-httplib?tab=readme-ov-file#readme

**Ajedrez**  
https://www.chess.com  
https://github.com/official-stockfish/Stockfish

<br>
