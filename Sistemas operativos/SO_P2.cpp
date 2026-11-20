#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>

#define MEMORIA_TOTAL 200
#define SO 20
#define NUM_TAREAS 7

/* Estructura Tabla Memoria */
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

/* Estructura Tabla Tareas */
struct nodo_JT
{
    int ntarea;
    int tamaniot;
    struct nodo_JT *sig;
};

/* PROTOTIPOS CORREGIDOS */
/* Quitamos los punteros extra de imprime_MT para simplificar el llamado */
struct nodo_MT *crea_MT(void);
void imprime_MT(struct nodo_MT *cabezera, struct nodo_MT *pivote, struct nodo_MT *seleccionado);
struct nodo_JT *crear_JT(void);
void imprime_JT(struct nodo_JT *cabezera);
void asignar_tareas(struct nodo_MT *memoria, struct nodo_JT **tareas, int metodo);


/* --------------------
   MAIN
   -------------------- */
/* --------------------
    MAIN
   -------------------- */
int main()
{
    struct nodo_MT *lista_memoria;
    struct nodo_JT *lista_tareas;

    system("cls");
    printf("\n--- ADMINISTRACION DE MEMORIA (NEXT FIT) ---\n");

    /* 1. Crear memoria */
    lista_memoria = crea_MT();

    /* 2. Mostrar memoria inicial (SIN punteros aún) */
    printf("\nTABLA DE MEMORIA INICIAL (VACIA)\n");
    imprime_MT(lista_memoria, NULL, NULL); 

    /* 3. Crear tareas */
    lista_tareas = crear_JT();

    printf("\nTABLA DE TAREAS A PROCESAR\n");
    imprime_JT(lista_tareas);
	
	int ajus = 0;
	printf("\nSelecciona el tipo de ordenamiento 1. Primer ajuste / 2. Mejor ajuste\n");
	scanf("%d", &ajus);
	
    printf("\nPresione una tecla para iniciar la asignacion...");
	getch();

    /* 4. Asignar tareas (Aquí es donde se gestionan los punteros históricos) */
    asignar_tareas(lista_memoria, &lista_tareas, ajus);

    /* 5. Resultados finales */
    printf("\n============================================");
    printf("\nPROCESO FINALIZADO");
    printf("\nESTADO FINAL DE LA MEMORIA:\n");
    imprime_MT(lista_memoria, NULL, NULL);

    printf("\nTAREAS QUE NO PUDIERON ASIGNARSE:\n");
    imprime_JT(lista_tareas);

    printf("\nPresione cualquier tecla para salir...");
    getch();
    return 0;
}

/* --------------------
   CREAR TABLA MEMORIA
   -------------------- */
struct nodo_MT *crea_MT()
{
    struct nodo_MT *inicio = NULL, *actual, *nuevo;
    int restan = MEMORIA_TOTAL - SO;
    int tam, opc;
    int contador = 0;
    int direccion_actual = SO;

    /* Crear bloque del SO */
    nuevo = (struct nodo_MT *)malloc(sizeof(struct nodo_MT));
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

    while (restan > 0)
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
            continue;
        }

        nuevo = (struct nodo_MT *)malloc(sizeof(struct nodo_MT));
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

    /* 🔥 AQUÍ SE HACE CIRCULAR */
    actual->sig = inicio->sig;

    return inicio;
}

/* --------------------
   IMPRIMIR MEMORIA (CORREGIDO)
   -------------------- */

void imprime_MT(struct nodo_MT *cabezera,
                struct nodo_MT *flecha,
                struct nodo_MT *asterisco)
{
    // Empezamos desde el bloque después del SO (memoria->sig)
    struct nodo_MT *temp = cabezera->sig;

    printf("\n[->] Bloque anterior modificado \n [*] Bloque recien modificado\n");
    printf("B Bloque  Cap  LocIni  LocFin  Acc  Est  FI\n");
    printf("------------------------------------------------\n");
    
    do
    {
        /* Lógica de punteros:
           1. Si ambos coinciden: ->*
           2. Si es la flecha:    ->
           3. Si es el asterisco:  * 
        */
        if (flecha != NULL && asterisco != NULL && temp == flecha && temp == asterisco)
            printf("->*");
        else if (flecha != NULL && temp == flecha)
            printf("-> ");
        else if (asterisco != NULL && temp == asterisco)
            printf("*  ");
        else
            printf("   ");

        printf("B%-4d %-4d %-6d %-6d %-4d %-4d %-4d\n",
               temp->nbloque,
               temp->tamaniob,
               temp->localidad,
               temp->locfin,
               temp->Asignacion,
               temp->estado,
               temp->fi);

        temp = temp->sig;

    } while (temp != cabezera->sig);
}

/* --------------------
   CREAR TABLA TAREAS
   -------------------- */
struct nodo_JT *crear_JT()
{
    struct nodo_JT *inicio = NULL, *actual, *nuevo;
    int i; /* Declaracion fuera del for para Borland */

    srand((unsigned)time(NULL));
    int arreglo_tareas [] = {0, 45, 15,30,20,10,10,55};

    for (i = 1; i <= NUM_TAREAS; i++)
    {
        nuevo = (struct nodo_JT *)malloc(sizeof(struct nodo_JT));
        nuevo->ntarea = i;
        nuevo->tamaniot = arreglo_tareas[i];             //(rand() % 41) + 5;
        nuevo->sig = NULL;

        if (inicio == NULL) {
            inicio = nuevo;
            actual = nuevo;
        } else {
            actual->sig = nuevo;
            actual = nuevo;
        }
    }
    return inicio;
}

void imprime_JT(struct nodo_JT *cabezera)
{
    struct nodo_JT *aux;
    if(cabezera == NULL) {
        printf("No hay tareas en la lista.\n");
        return;
    }
    printf("\nTarea   Tamano(KB)\n");
    for (aux = cabezera; aux != NULL; aux = aux->sig)
    {
        printf("J%-6d %-6d\n", aux->ntarea, aux->tamaniot);
    }
}

/* --------------------
   ASIGNAR TAREAS (CON MEMORIA HISTÓRICA)
   -------------------- */
/* --------------------
   ASIGNAR TAREAS (NEXT FIT + BEST FIT CON HISTORIAL)
   -------------------- */
void asignar_tareas(struct nodo_MT *memoria, struct nodo_JT **tareas, int metodo)
{
    if (memoria == NULL || *tareas == NULL)
        return;

    struct nodo_JT *actual = *tareas;
    struct nodo_JT *anterior_tarea = NULL;

    /* Punteros de control histórico para la impresión visual */
    struct nodo_MT *puntero_anterior = NULL; // Flecha (->)
    struct nodo_MT *puntero_actual = NULL;   // Asterisco (*)
    
    /* Puntero para Next Fit (se mantiene entre tareas si fuera necesario) */
    struct nodo_MT *pivote = memoria->sig; 

    while (actual != NULL)
    {
        struct nodo_MT *seleccionado = NULL;
        int mejor_espacio = 10000; // Valor alto inicial para Best Fit

        if (metodo == 1) // --- LÓGICA NEXT FIT ---
        {
            struct nodo_MT *aux_NF = pivote;
            do {
                if (aux_NF->estado == 0 && aux_NF->tamaniob >= actual->tamaniot) {
                    seleccionado = aux_NF;
                    pivote = seleccionado->sig; // Actualiza para la siguiente tarea
                    break;
                }
                aux_NF = aux_NF->sig;
            } while (aux_NF != pivote);
        }
        else if (metodo == 2) // --- LÓGICA BEST FIT ---
        {
            struct nodo_MT *aux_BF = memoria->sig;
            // En Best Fit recorremos toda la lista circular una vez
            do {
                if (aux_BF->estado == 0 && aux_BF->tamaniob >= actual->tamaniot) {
                    int espacio_sobrante = aux_BF->tamaniob - actual->tamaniot;
                    if (seleccionado == NULL || espacio_sobrante < mejor_espacio) {
                        seleccionado = aux_BF;
                        mejor_espacio = espacio_sobrante;
                    }
                }
                aux_BF = aux_BF->sig;
            } while (aux_BF != memoria->sig);
        }

        /* SI SE ENCONTRÓ UN BLOQUE (Sea por el método que sea) */
        if (seleccionado != NULL)
        {
            /* Actualización de rastro visual */
            if (puntero_actual == NULL) {
                puntero_actual = seleccionado;
                puntero_anterior = seleccionado;
            } else {
                puntero_anterior = puntero_actual;
                puntero_actual = seleccionado;
            }

            /* Asignación física en memoria */
            seleccionado->estado = 1;
            seleccionado->Asignacion = actual->ntarea;
            seleccionado->fi = seleccionado->tamaniob - actual->tamaniot;

            printf("\n>>> Metodo %d: Asignando Tarea %d en Bloque %d\n", 
                   metodo, actual->ntarea, seleccionado->nbloque);

            /* Imprimir estado con la flecha y el asterisco */
            imprime_MT(memoria, puntero_anterior, puntero_actual);

            /* Eliminar tarea de la lista JT */
            if (anterior_tarea == NULL) {
                *tareas = actual->sig;
                free(actual);
                actual = *tareas;
            } else {
                anterior_tarea->sig = actual->sig;
                free(actual);
                actual = anterior_tarea->sig;
            }
        }
        else
        {
            printf("\n!!! No hay espacio para Tarea %d por Metodo %d\n", 
                   actual->ntarea, metodo);
            anterior_tarea = actual;
            actual = actual->sig;
        }
    }
}

