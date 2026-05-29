# Tarea 3

## Descripción 

Este repositorio contiene un programa de Navegación en un Laberinto NxN(con una dificultad ajustable de 0 a 100), en este, un agente recorrera el laberinto buscando una salida y sorteando obstáculos, el agente buscará la ruta más corta para llegar a la meta, este programa cuenta con 3 métodos de búsqueda, búsqueda en anchura(BFS), búsqueda en profundidad(DFS) y A star(A*), los tres métodos intentaran hayar la ruta más corta.

## Cómo compilar y ejecutar el programa

Para ejecutar el programa, primero debes importar el repositorio a Replit, para esto, debes iniciar sesión en Replit, después apretar la opción "Import code or design", seguido apretar la opción de Github y  en esta copiar la URL del repositorio: https://github.com/lucianosanmartin/Tarea3
 
Una vez importado el repositorio, debe presionar "Open library"(esta se haya al lado de la opción Publish e Invite), presionar la opción "File Tree" y abrir el archivo tarea3.c. Ahora debemos compilar, para esto debe elegir la opción "Tools & files"(que debe hayarse justo al lado de la ventana de la tarea3.c), en buscador debe escribir "console" y  seleccionar la opción del mismo nombre, una vez dentro de console, debe selccionar la opcion de correr el programa.

## Funcionalidades

-La opción de búsqueda en profundidad puede que no devuelva la ruta más corta el 100% de las veces

## Ejemplo de uso

Al inicio se le mostrará un mensaje pidiendole que ingrese la dificultad, al ingresar un valor válido se generará un laberinto

### Seleccionar dificultad

Ingrese la dificultad del laberinto (porcentaje de obstáculos, 0 a 100): 90

Estado inicial del laberinto(I=inicio, M=meta, [X]=obstáculo, .=espacio libre):

*se imprime el estado inicial del laberinto *

Distancia L1 al inicio: 15(ejemplo)

========================================

Escoge método de búsqueda

========================================

1) Búsqueda en Profundidad (DFS)

2) Búsqueda en Anchura (BFS)

3) Búsqueda A* (Mejor Primero)

4) Salir

### Búsqueda en Profundidad (DFS)

Ingrese su opción: 1

==============================

Búsqueda en Profundidad (DFS)

==============================

¡Aviso: DFS puede no encontrar la solución mas corta!

¡Solución encontrada!

Costo del camino (pasos): 17(ejemplo)

Cantidad de iteraciones (nodos expandidos): 23(ejemplo)

*se imprime la ruta *

### Búsqueda en Anchura (BFS)

Ingrese su opción: 2

==============================

   Búsqueda en Anchura (BFS)

==============================

¡Este metodo garantiza la solucion mas corta!

¡Solución encontrada!

Costo del camino (pasos): 24(ejemplo)

Cantidad de iteraciones (nodos expandidos): 30(ejemplo)

*se imprime la ruta *

### Búsqueda A* (Mejor Primero)

Ingrese su opción: 3

==============================

  Búsqueda A* (Mejor Primero)

==============================

¡Este metodo garantiza la solucion mas corta de manera mas eficiente!

¡Solución encontrada!

Costo del camino (pasos): 17(ejemplo)

Cantidad de iteraciones (nodos expandidos): 23(ejemplo)

*se imprime la ruta *
