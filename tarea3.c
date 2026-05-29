#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tdas/list.h"
#include "tdas/heap.h"
#include "tdas/extra.h"
#include "tdas/stack.h"
#include "tdas/queue.h"

typedef struct {//se declara la estructura State que representa el estado del laberinto
    int maze[N][N];// Matriz NxN que representa el laberinto
    int x;// Fila actual del agente
    int y;// Columna actual del agente
    int x_final;// Fila de la meta
    int y_final;// Columna de la meta
    int steps;// Pasos dados hasta el estado actual
    List* actions;// Lista de movimientos para llegar al estado
} State;

int distancia_L1(State* state)//Heuristica de manhattan para determinar la distancia desde el estado actual hasta la meta
{
    return abs(state->x - state->x_final) + abs(state->y - state->y_final);
}

int is_final_state(State* state)//Funcion que verifica si la posicion actual es la posicion final 
{
    return state->x == state->x_final && state->y == state->y_final;
}

State* transition(State* current, int action)//Se genera un nuevo estado aplicando una acción
{
    int dx=0,dy=0;//Se inicializan las variables dx y dy en 0, estas representan el cambio en la posicion del agente
    if(action==1)
    {
        dx=-1;//Arriba
    }
    else 
    {
        if(action==2)
        {
            dx=1;//Abajo
        }
        else 
        {
            if(action==3)
            {
                dy=-1;//Izquierda
            }
            else  
            {
                if(action==4)
                {
                    dy=1;//Derecha
                }
            }
        }
    }
    int nx=current->x + dx;//Se calcula la nueva posicion en X del agente 
    int ny=current->y + dy;//Se calcula la nueva posicion en Y del agente
    if(nx<0 || nx>=N || ny<0 || ny>=N)//Se verifica si la posicion se haya en los límites del laberinto
    {
        return NULL;
    }
    if(current->maze[nx][ny]==1)//Se verifica que la posicion no sea un obstáculo
    {
        return NULL;
    }
    State* next=(State*)malloc(sizeof(State));//Se crea un nuevo estado
    if(next==NULL)//Si no se pudo asignar memoria se retorna NULL
    {
        return NULL;
    }
    memcpy(next->maze,current->maze,sizeof(current->maze));//Se copia el laberinto al nuevo estado
    next->x=nx;//Se actualiza la posicion del agente en X
    next->y=ny;//Se actualiza la posicion del agente en Y
    next->x_final=current->x_final;//Se copia la posicion final del laberinto en X
    next->y_final=current->y_final;//Se copia la posicion final del laberinto en Y
    next->steps=current->steps + 1;//Se incrementa el numero de pasos
    next->actions=list_create();//Se crea una nueva lista de acciones
    int* act;//Se crea un puntero para iterar sobre las acciones
    void* ptr=list_first(current->actions);//Se obtiene el primer elemento de la lista de acciones del estado actual
    while(ptr!=NULL)//Se itera sobre las acciones del estado actual mientras haya acciones
    {
        int* copy=(int*)malloc(sizeof(int));//Se reserva memoria para una copia de la accion
        *copy= *(int*)ptr;//Se copia la accion actual
        list_pushBack(next->actions,copy);//Se agrega la accion copiada a la lista de acciones del nuevo estado
        ptr=list_next(current->actions);//Se pasa a la siguiente accion en la lista
    }
    int* new_action=(int*)malloc(sizeof(int));//Se reserva memoria para la nueva accion
    *new_action=action;//Se asigna la accion actual a la nueva accion
    list_pushBack(next->actions,new_action);//Se agrega la nueva accion a la lista de acciones del nuevo estado
    return next;//Se retorna el nuevo estado
}

void free_state(State* state)//Funcion para liberar la memoria de un estado
{
    if(state==NULL)//Si el estado es NULL se retorna
    {
        return;
    }
    int* act=(int*)list_first(state->actions);//Se obtiene el primer elemento de la lista de acciones del estado
    while(act!=NULL)//Mientras haya acciones en la lista se continua el bucle
    {
        free(act);//Se libera la memoria de la accion actual
        act=(int*)list_next(state->actions);//Se pasa a la siguiente accion en la lista
    }
    list_clean(state->actions);//Se limpia la lista de acciones
    free(state->actions);//Se libera la memoria de la lista de acciones
    free(state);//Se libera la memoria del estado
}

void imprimirEstado(const State* estado)//Funcion para imprimir el estado del laberinto
{
    printf("\n");//Salto de línea para mqyor orden
    for(int i=0;i<N;i++)//Se itera sobre las filas del laberinto
    {
        for(int j=0;j<N;j++)//Se itera sobre las columnas del laberinto
        {
            if(estado->x==i && estado->y==j)//Se verifica si la posicion actual es la posicion del agente
            {
                printf(" A ");//Posición actual del agente
            }
            else
            {
                if(i==0 && j==0)//Se verifica si la posicion actual es la posicion inicial del laberinto
                {
                    printf(" I ");//Inicio del laberinto
                }
                else
                {
                    if(i==estado->x_final && j==estado->y_final)//Se verifica si la posicion actual es la posicion final del laberinto
                    {
                        printf(" M ");//Meta del laberinto
                    }
                    else
                    {
                        if(estado->maze[i][j]==1)//Se verifica si la posicion actual es un obstáculo
                        {
                            printf("[X]");//Obstáculo
                        }
                        else//Si no es ninguna de las anteriores, es una celda libre
                        {
                            printf(" . ");//Celda libre
                        }
                    }
                }
            }
            
        }
        printf("\n");//Salto de línea para pasar a la siguiente fila
    }
}

void imprimirRuta(State* estado_inicial, State* estado_final)//Se imprime el laberinto con la ruta final dibujada con '+'
{
    int ruta[N][N];//Se crea una matriz para almacenar la ruta
    memset(ruta,0,sizeof(ruta));//Se inicializa la matriz en 0
    int cx=0,cy=0;//Se inicializan las variables cx y cy en 0, estas representan la posicion actual en la ruta
    ruta[cx][cy]=1;//Se marca la posicion inicial en la ruta
    void* ptr=list_first(estado_final->actions);//Se obtiene el primer elemento de la lista de acciones del estado final
    while(ptr!=NULL)//Se itera sobre las acciones del estado final mientras haya acciones
    {
        int action= *(int*)ptr;//Se obtiene la accion actual
        if(action==1)
        {
            cx--;//Se mueve hacia arriba
        }
        else 
        {
            if(action==2)
            {
                cx++;//Se mueve hacia abajo
            }
            else
            {
                if(action==3)
                {
                    cy--;//Se mueve hacia la izquierda
                }
                else
                {
                    if(action==4)
                    {
                        cy++;//Se mueve hacia la derecha
                    }
                }
            }
        }
        ruta[cx][cy]=1;//Se marca la posicion actual en la ruta
        ptr=list_next(estado_final->actions);//Se pasa a la siguiente accion en la lista
    }
    printf("\n");//Salto de línea para mayor orden
    for(int i=0;i<N;i++)//Se itera sobre las filas del laberinto
    {
        for(int j=0;j<N;j++)//Se itera sobre las columnas del laberinto
        {
            if(i==0 && j==0)//Se verifica si la posicion actual es la posicion inicial del laberinto
            {
                printf(" I ");
            }
            else
            {
                if(i==estado_final->x_final && j==estado_final->y_final)//Se verifica si la posicion actual es la posicion final del laberinto
                {
                    printf(" M ");
                }
                else
                {
                    if(estado_inicial->maze[i][j]==1)//Se verifica si la posicion actual es un obstáculo
                    {
                        printf("[X]");
                    }
                    else 
                    {
                        if(ruta[i][j])//Se verifica si la posicion actual es parte de la ruta
                        {
                            printf(" + ");// Marca la ruta encontrada
                        } 
                        else//Si no es ninguna de las anteriores, es una celda libre
                        {
                            printf(" . ");
                        }
                    }
                }
            }
        }
        printf("\n");//Salto de línea para pasar a la siguiente fila
    }
}

State crearEstadoInicial(int dificultad)//Funcion que crea el estado inicial del laberinto
{
    State estado;//Se crea una variable de tipo State para almacenar el estado inicial
    generate_maze(estado.maze,dificultad);//Se genera el laberinto con la dificultad ingresada
    estado.x=0;//Se inicializa la posicion del agente en la posicion inicial X del laberinto
    estado.y=0;//Se inicializa la posicion del agente en la posicion inicial Y del laberinto
    estado.x_final=N-1;//Se inicializa la posicion final del laberinto en X
    estado.y_final=N-1;//Se inicializa la posicion final del laberinto en Y
    estado.steps=0;//Se inicializa el numero de pasos en 0
    estado.actions=list_create();//Se crea una lista vacia para almacenar las acciones
    return estado;//Se retorna el estado inicial
}

void dfs(State estado_inicial)//Funcion para realizar la busqueda en profundidad
{
    printf("\n==============================\n");
    printf("Búsqueda en Profundidad (DFS)\n");
    printf("==============================\n");
    printf("¡Aviso: DFS puede no encontrar la solución mas corta!\n\n");
    int visited[N][N];//Se crea una matriz para almacenar los nodos visitados
    memset(visited,0,sizeof(visited));//Se inicializa la matriz en 0
    Stack* pila=stack_create(NULL);//Se crea una pila para almacenar los nodos a visitar
    State* init=(State*)malloc(sizeof(State));//Se reserva memoria para el estado inicial
    *init=estado_inicial;//Se copia el estado inicial al nuevo estado
    init->actions=list_create();//Se crea una lista vacia para almacenar las acciones
    stack_push(pila,init);//Se agrega el estado inicial a la pila
    int iteraciones=0;//Se inicializa el contador de iteraciones en 0
    State* solucion=NULL;//Se inicializa el puntero a la solucion en NULL
    while(stack_top(pila)!=NULL)//Mientras haya nodos en la pila se continua el bucle
    {
        State* actual=(State*)stack_pop(pila);//Se obtiene el nodo actual de la pila
        iteraciones++;//Se incrementa el contador de iteraciones
        if(visited[actual->x][actual->y])//Si el nodo actual ya fue visitado se libera la memoria y se continua
        {
            free_state(actual);//Se libera la memoria del nodo actual
            continue;//Se continua con el siguiente nodo en la pila
        }
        visited[actual->x][actual->y]=1;//Se marca el nodo actual como visitado
        if(is_final_state(actual))//Si el nodo actual es el estado final se guarda la solucion y se sale del bucle
        {
            solucion=actual;//Se guarda la solucion
            break;
        }
        for(int accion=1;accion<=4;accion++)//Se itera sobre las acciones posibles
        {
            State* vecino=transition(actual,accion);//Se genera el estado vecino aplicando la accion actual
            if(vecino!=NULL && !visited[vecino->x][vecino->y])//Si el estado vecino no es NULL y no ha sido visitado se agrega a la pila
            {
                stack_push(pila,vecino);//Se agrega el estado vecino a la pila
            }
            else
            {
                if(vecino!=NULL)//Si el estado vecino no es NULL se libera la memoria
                {
                    free_state(vecino);
                }
            }
        }
        free_state(actual);//Se libera la memoria del nodo actual
    }
    State* s;//Se crea un puntero para iterar sobre los nodos restantes en la pila
    while((s=(State*)stack_pop(pila))!=NULL)//Mientras haya nodos en la pila se libera la memoria
    {
        free_state(s);
    }
    free(pila);//Se libera la memoria de la pila
    if(solucion!=NULL)//Si se encontro una solucion se imprime la informacion de la solucion
    {
        printf("¡Solución encontrada!\n");
        printf("Costo del camino (pasos): %d\n",solucion->steps);
        printf("Cantidad de iteraciones (nodos expandidos): %d\n",iteraciones);
        imprimirRuta(&estado_inicial,solucion);
        free_state(solucion);//Se libera la memoria de la solucion
    } 
    else//Si no se encontro una solucion se imprime un mensaje de error
    {
        printf("No se encontró solución con DFS.\n");
        printf("Iteraciones realizadas: %d\n",iteraciones);
    }
}

void bfs(State estado_inicial)//Funcion para realizar la busqueda en anchura
{
    printf("\n==============================\n");
    printf("   Búsqueda en Anchura (BFS)\n");
    printf("==============================\n");
    printf("¡Este metodo garantiza la solucion mas corta!\n\n");
    int visited[N][N];
    memset(visited,0,sizeof(visited));
    Queue* cola=queue_create(NULL);//Se crea una cola para almacenar los nodos a visitar
    State* init=(State*)malloc(sizeof(State));//Se reserva memoria para el estado inicial
    *init=estado_inicial;//Se copia el estado inicial al nuevo estado
    init->actions=list_create();//Se crea una lista vacia para almacenar las acciones
    queue_insert(cola,init);//Se agrega el estado inicial a la cola
    visited[init->x][init->y]=1;//Se marca el estado inicial como visitado
    int iteraciones=0;//Se inicializa el contador de iteraciones en 0
    State* solucion=NULL;//Se inicializa el puntero a la solucion en NULL
    while(queue_front(cola)!=NULL)//Mientras haya nodos en la cola se continua el bucle
    {
        State* actual=(State*)queue_remove(cola);//Se obtiene el nodo actual de la cola
        iteraciones++;//Se incrementa el contador de iteraciones
        if(is_final_state(actual))//Si el nodo actual es el estado final se guarda la solucion y se sale del bucle
        {
            solucion=actual;
            break;
        }
        for(int accion=1;accion<=4;accion++)//Se itera sobre las acciones posibles
        {
            State* vecino=transition(actual,accion);//Se genera el estado vecino aplicando la accion actual
            if(vecino!=NULL && !visited[vecino->x][vecino->y])//Si el estado vecino no es NULL y no ha sido visitado se agrega a la cola
            {
                visited[vecino->x][vecino->y]=1;//Se marca el estado vecino como visitado
                queue_insert(cola,vecino);//Se agrega el estado vecino a la cola
            }
            else
            {
                if(vecino!=NULL)//Si el estado vecino no es NULL se libera la memoria
                {
                    free_state(vecino);//Se libera la memoria del estado vecino
                }
            }
        }
        free_state(actual);//Se libera la memoria del nodo actual
    }
    State* s;//Se crea un puntero para iterar sobre los nodos restantes en la cola
    while((s=(State*)queue_remove(cola))!=NULL)//
    {
        free_state(s);
    }
    free(cola);//Se libera la memoria de la cola
    if(solucion!=NULL)//Si se encontro una solucion se imprime la informacion de la solucion
    {
        printf("¡Solución encontrada!\n");
        printf("Costo del camino (pasos): %d\n",solucion->steps);
        printf("Cantidad de iteraciones (nodos expandidos): %d\n",iteraciones);
        imprimirRuta(&estado_inicial,solucion);
        free_state(solucion);//Se libera la memoria de la solucion
    }
    else//Si no se encontro una solucion se imprime un mensaje de error
    {
        printf("No se encontró solución con BFS.\n");
        printf("Iteraciones realizadas: %d\n",iteraciones);
    }
}

void Astar(State estado_inicial)//Funcion para realizar la busqueda A*
{
    printf("\n==============================\n");
    printf("   Búsqueda A* (Best-First)\n");
    printf("==============================\n");
    printf("¡Este metodo garantiza la solucion mas corta de manera mas eficiente!\n\n");
    int visited[N][N];
    memset(visited,0,sizeof(visited));
    Heap* heap=heap_create();//Se crea un montículo para almacenar los nodos a visitar
    State* init=(State*)malloc(sizeof(State));//Se reserva memoria para el estado inicial
    *init=estado_inicial;//Se copia el estado inicial al nuevo estado
    init->actions=list_create();//Se crea una lista vacia para almacenar las acciones
    int f=init->steps + distancia_L1(init);//Se calcula la funcion de evaluacion f(n) = g(n) + h(n)
    heap_push(heap,init,-f);//Se agrega el estado inicial al montículo con la funcion de evaluacion f(n)
    int iteraciones=0;//Se inicializa el contador de iteraciones en 0
    State* solucion=NULL;//Se inicializa el puntero a la solucion en NULL
    while(heap_top(heap)!=NULL)//Mientras haya nodos en el montículo se continua el bucle
    {
        State* actual=(State*)heap_top(heap);//Se obtiene el nodo actual del montículo
        heap_pop(heap);//Se elimina el nodo actual del montículo
        iteraciones++;//Se incrementa el contador de iteraciones
        if(visited[actual->x][actual->y])//Si el nodo actual ya fue visitado se libera la memoria y se continua
        {
            free_state(actual);
            continue;
        }
        visited[actual->x][actual->y]=1;//Se marca el nodo actual como visitado
        if(is_final_state(actual))//Si el nodo actual es el estado final se guarda la solucion y se sale del bucle
        {
            solucion=actual;
            break;
        }
        for(int accion=1;accion<=4;accion++)//Se itera sobre las acciones posibles
        {
            State* vecino=transition(actual,accion);//Se genera el estado vecino aplicando la accion actual
            if(vecino!=NULL && !visited[vecino->x][vecino->y])//Si el estado vecino no es NULL y no ha sido visitado se agrega al montículo
            {
                int f_vecino=vecino->steps + distancia_L1(vecino);//Se calcula la funcion de evaluacion f(n) = g(n) + h(n) para el estado vecino
                heap_push(heap,vecino,-f_vecino);//Se agrega el estado vecino al montículo con la funcion de evaluacion f(n)
            }
            else
            {
                if(vecino!=NULL)//Si el estado vecino no es NULL se libera la memoria
                {
                    free_state(vecino);
                }
            }
        }
        free_state(actual);//Se libera la memoria del nodo actual
    }
    while(heap_top(heap)!=NULL)//Mientras haya nodos en el montículo se libera la memoria
    {
        State* s=(State*)heap_top(heap);
        heap_pop(heap);
        free_state(s);
    }
    free(heap);//Se libera la memoria del montículo
    if(solucion!=NULL)//Si se encontro una solucion se imprime la informacion de la solucion
    {
        printf("¡Solución encontrada!\n");
        printf("Costo del camino (pasos): %d\n",solucion->steps);
        printf("Cantidad de iteraciones (nodos expandidos): %d\n",iteraciones);
        imprimirRuta(&estado_inicial,solucion);
        free_state(solucion);
    }
    else//Si no se encontro una solucion se imprime un mensaje de error
    {
        printf("No se encontró solución con A*.\n");
        printf("Iteraciones realizadas: %d\n",iteraciones);
    }
}

int main() 
{//funcion principal
    srand(time(NULL));//se inicializa la semilla para generar numeros aleatorios(generar el laberinto)
    int dificultad;//variable para almacenar la dificultad del laberinto
    char buf[64];//Variable para almacenar la entrada del usuario
    while(1)//Bucle para ingresar la dificultad del laberinto
    {
        limpiarPantalla();
        printf("Ingrese la dificultad del laberinto (porcentaje de obstáculos, 0 a 100): ");
        if(fgets(buf,sizeof(buf),stdin)==NULL)//Si la entrada es nula se pasa a la siguiente iteracion del bucle
        {
            continue;
        }
        char* ptr=buf;//Se crea un puntero para iterar sobre la entrada del usuario
        while(*ptr==' ' || *ptr=='\t')//Se salta los espacios en blanco
        {
            ptr++;
        }
        if(*ptr=='\0' || *ptr=='\n')//Si la entrada es vacia se imprime un mensaje de error y se pasa a la siguiente iteracion del bucle
        {
            printf("Entrada inválida. Ingrese un número entre 0 y 100!\n");
            presioneTeclaParaContinuar();
            continue;
        }
        char* endptr;//Se crea un puntero para almacenar el final de la entrada del usuario
        long val=strtol(ptr,&endptr,10);//Se convierte la entrada del usuario a un numero entero
        while(*endptr==' ' || *endptr=='\t')//Se salta los espacios en blanco
        {
            endptr++;
        }
        if(*endptr!='\n' && *endptr!='\0')//Si la entrada no es un numero se imprime un mensaje de error y se pasa a la siguiente iteracion del bucle
        {
            printf("Entrada inválida. Ingrese un número entre 0 y 100!\n");
            presioneTeclaParaContinuar();
            continue;
        }
        if(val<0 || val>100)//Si la entrada no esta en el rango de 0 a 100 se imprime un mensaje de error y se pasa a la siguiente iteracion del bucle
        {
            printf("Ingrese un valor entre 0 y 100!\n");
            presioneTeclaParaContinuar();
            continue;
        }
        dificultad=(int)val;//Se convierte la entrada del usuario a un numero entero y se guarda en la variable dificultad
        break;//Se sale del bucle
    }
    State estado_inicial=crearEstadoInicial(dificultad);//Se crea el estado inicial del laberinto en base a la dificultad ingresada
    printf("\nEstado inicial del laberinto(I=inicio, M=meta, [X]=obstáculo, .=espacio libre):\n"); //Se imprime el estado inicial del laberinto, indicando el significado de cada simbolo
    imprimirEstado(&estado_inicial);//se llama a la funcion imprimirEstado para mostrar el laberinto
    printf("Distancia L1 al inicio: %d\n",distancia_L1(&estado_inicial));//se imprime la distancia desde el inicio hasta la meta
    char opcion;//variable para almacenar la opcion del usuario
    do {//se imprime el menu de opciones
        puts("\n========================================");
        puts("     Escoge método de búsqueda");
        puts("========================================");
        puts("1) Búsqueda en Profundidad (DFS)");
        puts("2) Búsqueda en Anchura (BFS)");
        puts("3) Búsqueda A* (Mejor Primero)");
        puts("4) Salir");
        printf("Ingrese su opción: ");
        scanf(" %c", &opcion);//se escanea la opcion ingresada por el usuario
        switch(opcion) 
        {
            case '1'://si la opcion es 1 se llama a la funcion dfs
                dfs(estado_inicial);
                break;
            case '2'://si la opcion es 2 se llama a la funcion bfs
                bfs(estado_inicial);
                break;
            case '3'://si la opcion es 3 se llama a la funcion Astar
                Astar(estado_inicial);
                break;
            case '4'://si la opcion es 4 se sale del programa
                printf("Saliendo...\n");
                break;
            default://si se ingresa una opcion invalida se imprime un mensaje de error
                printf("Ingrese una opcion valida!\n");
        }
        if(opcion!='4')//si la opcion escogida no es salir, se muestra el laberinto actual
        {
            presioneTeclaParaContinuar();//funcion para pausar la ejecucion y permitir al usuario ver los resultados, continuando cuando se presione una tecla
            limpiarPantalla();//funcion para limpiar la pantalla
            printf("Laberinto actual(I=inicio, M=meta, [X]=obstáculo, .=espacio libre):\n");
            imprimirEstado(&estado_inicial);
        }
    } while(opcion!='4');//si la opcion es 4 se sale del bucle
    int* act=(int*)list_first(estado_inicial.actions);//Se libera la lista de acciones del estado inicial, comenzando por el primer elemento
    while(act!=NULL)//mientras haya acciones en la lista se continua el bucle
    {
        free(act);//se libera la memoria de la accion actual
        act=(int*)list_next(estado_inicial.actions);//se pasa a la siguiente accion en la lista
    }
    list_clean(estado_inicial.actions);//se limpia la lista de acciones
    free(estado_inicial.actions);//se libera la memoria de la lista de acciones
    return 0;//se retorna 0 para indicar que el programa se ejecuto correctamente
}

