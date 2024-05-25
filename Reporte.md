## Reporte del juego "Viboritaz".

El juego "Viboritaz" es un juego de la serpiente en el que el jugador controla una serpiente que se mueve por la pantalla. El objetivo del juego es comer la mayor cantidad de manzanas posibles sin chocar con las paredes o con la serpiente misma. Cada vez que la serpiente come una manzana, su tamaño aumenta. El juego termina cuando la serpiente choca con una pared o con ella misma.

---
### Pantallas del juego:
**MENU PRINCIPAL DEL JUEGO:**
![Imagen 1](/build/resources/viboritaz_inicio.png)

Para iniciar el juego, unicamente es necesario presionar la tecla "Enter" del teclado. 
---
**JUEGO EN CURSO:**
![Imagen 2](/build/resources/viboritaz_juego.png)

Como podemos observar, ya tenia un puntaje de 24 manzanas comidas. Siempre se inicia con 1 manzana como puntaje. 
La manzana reaparece en una posicion aleatoria cada vez que la serpiente la come.
Para finalizar, si chocas con la pared o con la serpiente, el juego se reinicia automaticamente, regresandote a la pantalla de inicio.

---
**Realizacion**

El juego fue realizado con C, utilizando la terminacion ".cpp" para poder compilarlo con el compilador de C++ de raylib.
Se utilizo la libreria raylib para la realizacion del juego, la cual nos permite realizar juegos en 2D de manera sencilla.
Tambien, se utilizaron listas enlazadas para el manejo de la serpiente. 