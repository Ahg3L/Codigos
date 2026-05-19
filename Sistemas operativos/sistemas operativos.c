#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MEMORIA_TOTAL 200
#define SO 20
#define NUM_TAREAS 4

// Estructura Tabla Memoria
struct nodo_MT
{
    int nbloque;
    int tamaniob;
    int localidad;
    int locfin;
    int Asignacion;
    int estado;
    int fi;
    struct nodo_MT *sig;
};

// --------------------
// Estructura Tabla Tareas
// --------------------
struct nodo_JT
{
    int ntarea;
    int tamaniot;
    struct nodo_JT *sig;
};

// Prototipos
struct nodo_MT *crea_MT(void);
void imprime_MT(struct nodo_MT *);
struct nodo_JT *crear_JT(void);
void imprime_JT(struct nodo_JT *);
void asignar_tareas(struct nodo_MT *memoria, struct nodo_JT **tareas, int metodo);

// --------------------
// MAIN
// --------------------
int main()
{

    struct nodo_MT *lista_memoria;
    struct nodo_JT *lista_tareas;
    system("cls");
    printf("\n--- ADMINISTRACION DE MEMORIA ---\n");

    // Crear tabla memoria
    lista_memoria = crea_MT();

    printf("\nTABLA DE MEMORIA INICIAL\n");
    imprime_MT(lista_memoria);

    // Crear tabla tareas
    lista_tareas = crear_JT();

    printf("\nTABLA DE TAREAS\n");
    imprime_JT(lista_tareas);

    // Asignar tareas
    asignar_tareas(lista_memoria, &lista_tareas, 1); // 1 para primer ajuste

    printf("\nTABLA DE MEMORIA FINAL\n");
    imprime_MT(lista_memoria);
    imprime_JT(lista_tareas);

    return 0;
}

// --------------------
// CREAR TABLA MEMORIA
// --------------------
struct nodo_MT *crea_MT()
{
    struct nodo_MT *inicio = NULL, *actual, *nuevo;

    int restan = MEMORIA_TOTAL - SO;
    int tam, opc;
    int contador = 0;
    int direccion_actual = SO;

    // Bloque del sistema operativo
    nuevo = malloc(sizeof(struct nodo_MT));
    nuevo->nbloque = contador++;
    nuevo->tamaniob = SO;
    nuevo->localidad = 0;
    nuevo->locfin = SO - 1;
    nuevo->Asignacion = 0;
    nuevo->estado = 1;
    nuevo->fi = 0;
    nuevo->sig = NULL;

    inicio = nuevo;
    actual = nuevo;

    printf("\nMemoria restante %d KB", restan);

    // Cambio de while por for
    // Se mantiene la condición de 'restan > 0'
    for (; restan > 0;)
    {
        printf("\nDesea crear otra particion? (1=si 0=no): ");
        scanf("%d", &opc);

        if (opc == 0)
            break;

        printf("Tamano del bloque: ");
        scanf("%d", &tam);

        if (tam > restan)
        {
            printf("No hay espacio suficiente\n");
            printf("\nMemoria restante %d KB", restan);
            continue;
        }

        nuevo = malloc(sizeof(struct nodo_MT));
        nuevo->nbloque = contador++;
        nuevo->tamaniob = tam;
        nuevo->localidad = direccion_actual;
        nuevo->locfin = direccion_actual + tam - 1;
        nuevo->Asignacion = 0;
        nuevo->estado = 0;
        nuevo->fi = 0;
        nuevo->sig = NULL;

        actual->sig = nuevo;
        actual = nuevo;

        restan -= tam;
        direccion_actual += tam;
    }

    return inicio;
}

// --------------------
// IMPRIMIR MEMORIA
// --------------------
void imprime_MT(struct nodo_MT *cabezera)
{
    struct nodo_MT *aux;

    printf("\nBloque  Cap(KB)  Loc.Ini  Loc.Fin  Acceso  Estado  FI\n");

    // Recorrido de la lista ligada usando for
    for (aux = cabezera; aux != NULL; aux = aux->sig)
    {
        printf("B%-6d %-8d %-8d %-8d %-7d %-7d %-7d\n",
               aux->nbloque,
               aux->tamaniob,
               aux->localidad,
               aux->locfin,
               aux->Asignacion,
               aux->estado,
               aux->fi);
    }
}

// --------------------
// CREAR TABLA TAREAS
// --------------------
struct nodo_JT *crear_JT()
{

    struct nodo_JT *inicio = NULL, *actual, *nuevo;

    srand(time(NULL));

    for (int i = 1; i <= NUM_TAREAS; i++)
    {

        nuevo = malloc(sizeof(struct nodo_JT));

        nuevo->ntarea = i;
        nuevo->tamaniot = (rand() % 41) + 5;
        nuevo->sig = NULL;

        if (inicio == NULL)
        {
            inicio = nuevo;
            actual = nuevo;
        }
        else
        {
            actual->sig = nuevo;
            actual = nuevo;
        }
    }

    return inicio;
}

// --------------------
// IMPRIMIR TAREAS
// --------------------
void imprime_JT(struct nodo_JT *cabezera)
{
    struct nodo_JT *aux;

    printf("\nTarea   Tamano(KB)\n");

    // Estructura: (Inicio; Condición; Avance)
    for (aux = cabezera; aux != NULL; aux = aux->sig)
    {
        printf("J%-6d %-6d\n",
               aux->ntarea,
               aux->tamaniot);
    }
}

// --------------------
// ASIGNAR TAREAS
// --------------------

void asignar_tareas(struct nodo_MT *memoria,
                    struct nodo_JT **tareas,
                    int metodo)
{
    struct nodo_JT *actual = *tareas;
    struct nodo_JT *anterior = NULL;

    while (actual != NULL)
    {
        struct nodo_MT *bloque = memoria->sig;
        struct nodo_MT *seleccionado = NULL;

        int mejor_espacio = 0;

        while (bloque != NULL)
        {
            if (bloque->estado == 0 &&
                bloque->tamaniob >= actual->tamaniot)
            {
                if (metodo == 1) // FIRST FIT
                {
                    seleccionado = bloque;
                    break;
                }
                else if (metodo == 2) // BEST FIT
                {
                    int espacio = bloque->tamaniob - actual->tamaniot;

                    if (seleccionado == NULL ||
                        espacio < mejor_espacio)
                    {
                        seleccionado = bloque;
                        mejor_espacio = espacio;
                    }
                }
                else if (metodo == 3) // WORST FIT
                {
                    int espacio = bloque->tamaniob - actual->tamaniot;

                    if (seleccionado == NULL ||
                        espacio > mejor_espacio)
                    {
                        seleccionado = bloque;
                        mejor_espacio = espacio;
                    }
                }
            }

            bloque = bloque->sig;
        }

        if (seleccionado != NULL)
        {
            seleccionado->estado = 1;
            seleccionado->Asignacion = actual->ntarea;
            seleccionado->fi = seleccionado->tamaniob - actual->tamaniot;

            // Eliminar tarea de la lista
            if (anterior == NULL)
            {
                *tareas = actual->sig;
                free(actual);
                actual = *tareas;
            }
            else
            {
                anterior->sig = actual->sig;
                free(actual);
                actual = anterior->sig;
            }
        }
        else
        {
            anterior = actual;
            actual = actual->sig;
        }
    }
}