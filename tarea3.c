#include <stdio.h>
#include <stdlib.h>
#include "tdas/list.h"
#include "tdas/heap.h"
#include "tdas/extra.h"
#include <string.h>
#include <time.h>

typedef struct {
    int maze[N][N];
    int x;
    int y;
    int steps;
    List* actions;
} State;

int distancia_L1(State* state) {
    return abs(state->x - (N-1)) + abs(state->y - (N-1));
}

void imprimirEstado(const State *estado) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (estado->x == i && estado->y == j) printf(" A ");
            else if (i == 0 && j == 0)             printf(" I ");
            else if (i == N-1 && j == N-1)         printf(" M ");
            else if (estado->maze[i][j] == 0)      printf(" . ");
            else                                    printf("[X]");
        }
        printf("\n");
    }
}

State crearEstadoInicial(int maze[N][N], int dificultad) {
    State estado;
    generate_maze(estado.maze, dificultad);
    estado.x = 0;
    estado.y = 0;
    estado.steps = 0;
    estado.actions = list_create();
    return estado;
}

State* copiarEstado(State* estado) {
    State* nuevo = (State*)malloc(sizeof(State));
    memcpy(nuevo->maze, estado->maze, sizeof(estado->maze));
    nuevo->x = estado->x;
    nuevo->y = estado->y;
    nuevo->steps = estado->steps;
    nuevo->actions = list_create();
    char* accion = list_first(estado->actions);
    while (accion != NULL) {
        list_pushBack(nuevo->actions, strdup(accion));
        accion = list_next(estado->actions);
    }
    return nuevo;
}

void liberarEstado(State* estado) {
    char* accion = list_first(estado->actions);
    while (accion != NULL) {
        free(accion);
        accion = list_next(estado->actions);
    }
    list_clean(estado->actions);
    free(estado->actions);
    free(estado);
}

int esMeta(State* estado) {
    return estado->x == N-1 && estado->y == N-1;
}

void imprimirSolucion(State* sol, int explorados) {
    printf("\n=== Solución encontrada ===\n");
    printf("Pasos realizados: %d\n", sol->steps);
    printf("Nodos explorados: %d\n", explorados);
    printf("Camino: ");
    char* accion = list_first(sol->actions);
    while (accion != NULL) {
        printf("%s", accion);
        accion = list_next(sol->actions);
        if (accion != NULL) printf(" -> ");
    }
    printf("\n\nEstado final:\n");
    imprimirEstado(sol);
}

void agregarVecinos(State* actual, List* vecinos, int visited[N][N]) {
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    char* nombres[] = {"Arriba", "Abajo", "Izquierda", "Derecha"};

    for (int i = 0; i < 4; i++) {
        int nx = actual->x + dx[i];
        int ny = actual->y + dy[i];
        if (nx >= 0 && nx < N && ny >= 0 && ny < N
            && actual->maze[nx][ny] == 0
            && !visited[nx][ny]) {
            State* nuevo = copiarEstado(actual);
            nuevo->x = nx;
            nuevo->y = ny;
            nuevo->steps++;
            list_pushBack(nuevo->actions, strdup(nombres[i]));
            list_pushBack(vecinos, nuevo);
        }
    }
}

void dfs(State estado_inicial) {
    int visited[N][N] = {0};
    int explorados = 0;

    List* pila = list_create();
    State* inicio = copiarEstado(&estado_inicial);
    list_pushFront(pila, inicio);

    while (list_first(pila) != NULL) {
        State* actual = (State*)list_popFront(pila);
        explorados++;

        if (visited[actual->x][actual->y]) {
            liberarEstado(actual);
            continue;
        }
        visited[actual->x][actual->y] = 1;

        if (esMeta(actual)) {
            imprimirSolucion(actual, explorados);
            liberarEstado(actual);
            list_clean(pila);
            free(pila);
            return;
        }

        List* vecinos = list_create();
        agregarVecinos(actual, vecinos, visited);

        State* vecino = (State*)list_first(vecinos);
        while (vecino != NULL) {
            list_pushFront(pila, vecino);
            vecino = (State*)list_next(vecinos);
        }
        list_clean(vecinos);
        free(vecinos);
        liberarEstado(actual);
    }

    printf("\nNo se encontró solución (DFS).\n");
    list_clean(pila);
    free(pila);
}

void bfs(State estado_inicial) {
    int visited[N][N] = {0};
    int explorados = 0;

    List* cola = list_create();
    State* inicio = copiarEstado(&estado_inicial);
    list_pushBack(cola, inicio);
    visited[inicio->x][inicio->y] = 1;

    while (list_first(cola) != NULL) {
        State* actual = (State*)list_popFront(cola);
        explorados++;

        if (esMeta(actual)) {
            imprimirSolucion(actual, explorados);
            liberarEstado(actual);
            list_clean(cola);
            free(cola);
            return;
        }

        List* vecinos = list_create();
        agregarVecinos(actual, vecinos, visited);

        State* vecino = (State*)list_first(vecinos);
        while (vecino != NULL) {
            visited[vecino->x][vecino->y] = 1;
            list_pushBack(cola, vecino);
            vecino = (State*)list_next(vecinos);
        }
        list_clean(vecinos);
        free(vecinos);
        liberarEstado(actual);
    }

    printf("\nNo se encontró solución (BFS).\n");
    list_clean(cola);
    free(cola);
}

void best_first(State estado_inicial) {
    int visited[N][N] = {0};
    int explorados = 0;

    Heap* heap = heap_create();
    State* inicio = copiarEstado(&estado_inicial);
    heap_push(heap, inicio, -distancia_L1(inicio));

    while (heap_top(heap) != NULL) {
        State* actual = (State*)heap_top(heap);
        heap_pop(heap);
        explorados++;

        if (visited[actual->x][actual->y]) {
            liberarEstado(actual);
            continue;
        }
        visited[actual->x][actual->y] = 1;

        if (esMeta(actual)) {
            imprimirSolucion(actual, explorados);
            liberarEstado(actual);
            free(heap);
            return;
        }

        List* vecinos = list_create();
        agregarVecinos(actual, vecinos, visited);

        State* vecino = (State*)list_first(vecinos);
        while (vecino != NULL) {
            heap_push(heap, vecino, -distancia_L1(vecino));
            vecino = (State*)list_next(vecinos);
        }
        list_clean(vecinos);
        free(vecinos);
        liberarEstado(actual);
    }

    printf("\nNo se encontró solución (Best First).\n");
    free(heap);
}

int main() {
    srand(time(NULL));
    int maze[N][N];
    int dificultad;
    do {
        printf("Ingrese la dificultad del laberinto (porcentaje de obstáculos, 0 a 100): ");
        scanf("%d", &dificultad);
        if (dificultad < 0 || dificultad > 100) {
            printf("Error: Por favor ingrese un valor válido entre 0 y 100.\n");
        }
    } while (dificultad < 0 || dificultad > 100);

    State estado_inicial = crearEstadoInicial(maze, dificultad);
    printf("\nEstado inicial del puzzle:\n");
    imprimirEstado(&estado_inicial);
    printf("Distancia L1: %d\n", distancia_L1(&estado_inicial));

    char opcion;
    do {
        puts("========================================");
        puts("     Escoge método de búsqueda");
        puts("========================================");
        puts("1) Búsqueda en Profundidad (DFS)");
        puts("2) Búsqueda en Anchura (BFS)");
        puts("3) Búsqueda Mejor Primero");
        puts("4) Salir");
        printf("Ingrese su opción: ");
        scanf(" %c", &opcion);
        switch (opcion) {
        case '1':
            dfs(estado_inicial);
            break;
        case '2':
            bfs(estado_inicial);
            break;
        case '3':
            best_first(estado_inicial);
            break;
        case '4':
            break;
        default:
            printf("Opción no válida. Por favor, ingrese una opción válida.\n");
            break;
        }
        if (opcion != '4') {
            presioneTeclaParaContinuar();
            limpiarPantalla();
            printf("\nEstado inicial del puzzle:\n");
            imprimirEstado(&estado_inicial);
            printf("Distancia L1: %d\n", distancia_L1(&estado_inicial));
        }
    } while (opcion != '4');

    list_clean(estado_inicial.actions);
    free(estado_inicial.actions);
    return 0;
}
