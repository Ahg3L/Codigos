#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>

#define MEMORIA_TOTAL 3 // MB
#define SO 2            // MARCOS PARA SO
#define NUM_TAREAS 5
#define TAM_MARCO 64 // KB
#define SECUENCIA 8
#define MARCOS_POR_TAREA 3

struct nodo_MT
{
    int nmarco;
    int locini;
    int Asignacion; // ID de tarea asignada (0 si libre)
    int estado;     // 2=SO, 1=Libre, 0=Ocupado
    struct nodo_MT *sig;
};

struct nodo_JT
{
    int id;
    int lineas;
    long loc_pmt;
    int *arrsecu;
    int tam_secuencia;
    struct nodo_JT *sig;
};

struct nodo_TMP
{
    int idTarea;
    int pagina;
    int marco;
    int estado;
    int referencia;
    int modificacion;
    struct nodo_TMP *sig;
};

struct Memoria
{
    int memoria_kb;
    int marcos_totales;
    int marcos_disponibles;
} m;

/* =======================================================================
   PROTOTIPOS DE FUNCIONES
   ======================================================================= */

struct nodo_MT *crea_MT(void);
struct nodo_JT *crea_JT();
struct nodo_TMP *crea_TMP(void);

// --- Funciones de Impresión ---
void imprime_MT(struct nodo_MT *cabezera);
void imprimir_JT(struct nodo_JT *cabeza);
void imprimir_TMP(struct nodo_TMP *tabla);

// --- Lógica del Sistema ---
void validar_tamano();
void asignar_tareas(struct nodo_MT *memoria, struct nodo_JT *tareas, struct nodo_TMP **lista_tmp);

int main()
{
    struct nodo_MT *lista_memoria = NULL;
    struct nodo_JT *lista_tareas = NULL;
    struct nodo_TMP *tmp = NULL;

    validar_tamano();

    lista_memoria = crea_MT();
    lista_tareas = crea_JT();

    imprime_MT(lista_memoria);
    imprimir_JT(lista_tareas);
    tmp = crea_TMP();
    imprimir_TMP(tmp);

    return 0;
}

void validar_tamano()
{
    printf("\n=========== CONFIGURACION MEMORIA ===========\n");
    printf("\nConversion de MB a KB\n");
    printf("Memoria KB = %d x 1024\n", MEMORIA_TOTAL);

    m.memoria_kb = MEMORIA_TOTAL * 1024;

    printf("Memoria KB = %d KB\n", m.memoria_kb);
    printf("\nCalculo de marcos:\n");

    m.marcos_totales = m.memoria_kb / TAM_MARCO;

    printf("Marcos Totales = %d / %d\n",
           m.memoria_kb,
           TAM_MARCO);

    printf("Marcos Totales = %d\n", m.marcos_totales);
    printf("\nMarcos reservados para SO = %d\n", SO);

    m.marcos_disponibles = m.marcos_totales - SO;

    printf("Marcos disponibles = %d\n",
           m.marcos_disponibles);
    printf("=============================================\n");
}

struct nodo_MT *crea_MT(void)
{
    struct nodo_MT *cabeza = NULL;
    struct nodo_MT *nuevo;
    struct nodo_MT *ultimo = NULL;

    for (int i = 0; i < m.marcos_totales; i++)
    {
        nuevo = (struct nodo_MT *)malloc(sizeof(struct nodo_MT));

        nuevo->nmarco = i;

        nuevo->locini = i * TAM_MARCO;

        nuevo->sig = NULL;

        // MARCOS DEL SO
        if (i < SO)
        {
            nuevo->Asignacion = 1;
            nuevo->estado = 2;
        }
        else
        {
            nuevo->Asignacion = 0;
            nuevo->estado = 0;
        }

        if (cabeza == NULL)
        {
            cabeza = nuevo;
            ultimo = nuevo;
        }
        else
        {
            ultimo->sig = nuevo;
            ultimo = nuevo;
        }
    }

    return cabeza;
}

void imprime_MT(struct nodo_MT *aux)
{
    printf("\n");
    printf("=============== TABLA MT ===============\n");

    printf("| MARCO | LOC INI | ASIGNACION |\n");
    printf("========================================\n");

    while (aux != NULL)
    {
        if (aux->estado == 2)
        {
            printf("| %-5d | %-7d | SO         |\n",
                   aux->nmarco,
                   aux->locini);
        }
        else if (aux->Asignacion == 0)
        {
            printf("| %-5d | %-7d | 0          |\n",
                   aux->nmarco,
                   aux->locini);
        }
        else
        {
            printf("| %-5d | %-7d | J%-9d |\n",
                   aux->nmarco,
                   aux->locini,
                   aux->Asignacion - 1);
        }

        aux = aux->sig;
    }

    printf("========================================\n");
}

struct nodo_JT *crea_JT(void)
{
    struct nodo_JT *cabeza = NULL;
    struct nodo_JT *nuevo = NULL;
    struct nodo_JT *ultimo = NULL;

    srand(time(NULL));

    for (int i = 0; i < NUM_TAREAS; i++)
    {
        nuevo = (struct nodo_JT *)malloc(sizeof(struct nodo_JT));
        nuevo->id = i + 1;
        nuevo->lineas = rand() % 900 + 100;
        nuevo->loc_pmt = rand() % 90000 + 10000;
        nuevo->tam_secuencia = SECUENCIA;
        nuevo->arrsecu = (int *)malloc(sizeof(int) * SECUENCIA);

        for (int j = 0; j < SECUENCIA; j++)
        {
            nuevo->arrsecu[j] = rand() % 10;
        }

        nuevo->sig = NULL;

        if (cabeza == NULL)
        {
            cabeza = nuevo;
            ultimo = nuevo;
        }
        else
        {
            ultimo->sig = nuevo;
            ultimo = nuevo;
        }
    }

    return cabeza;
}

void imprimir_JT(struct nodo_JT *cabeza)
{
    struct nodo_JT *actual = cabeza;
    int i = 0;

    printf("\n=================== JT (Lista Enlazada) ===================\n");
    printf("| TAREA | LINEAS | LOC PMT | SECUENCIA |\n");
    printf("===========================================================\n");

    while (actual != NULL)
    {
        printf("| J%-4d | %-7d | %-7ld | ",
               actual->id,
               actual->lineas,
               actual->loc_pmt);

        // Imprimir el arreglo dinámico usando su tamaño propio
        for (int j = 0; j < actual->tam_secuencia; j++)
        {
            printf("%d ", actual->arrsecu[j]);
        }

        printf("|\n");

        actual = actual->sig; // Mover al siguiente nodo
        i++;
    }

    printf("===========================================================\n");
}

struct nodo_TMP *crea_TMP(void)
{
    int total_entradas = NUM_TAREAS * MARCOS_POR_TAREA;

    struct nodo_TMP *tabla = (struct nodo_TMP *)malloc(sizeof(struct nodo_TMP) * total_entradas);

    int indice = 0;

    for (int pagina = 0; pagina < MARCOS_POR_TAREA; pagina++)
    {
        for (int tarea = 1; tarea <= NUM_TAREAS; tarea++)
        {
            tabla[indice].idTarea = tarea;
            tabla[indice].pagina = pagina;

            tabla[indice].marco = -1;

            tabla[indice].estado = 0;
            tabla[indice].referencia = 0;
            tabla[indice].modificacion = 0;

            indice++;
        }
    }

    return tabla;
}

void imprimir_TMP(struct nodo_TMP *tabla)
{
    int total_entradas = NUM_TAREAS * MARCOS_POR_TAREA;

    // El ciclo va desde 1 hasta MARCOS_POR_TAREA (1, 2, 3...)
    for (int ciclo = 1; ciclo <= MARCOS_POR_TAREA; ciclo++)
    {
        printf("\n\n============ TABLA DE MAPA DE PAGINAS ============\n");
        printf("CICLO %d\n", ciclo);

        for (int tarea = 1; tarea <= NUM_TAREAS; tarea++)
        {
            printf("\nTarea: J%d\n", tarea);
            printf("----------------------------------------------------------\n");
            printf("%-8s %-8s %-8s %-8s %-8s\n",
                   "Pagina",
                   "Marco",
                   "Estado",
                   "Refer.",
                   "Modifi.");
            printf("----------------------------------------------------------\n");

            // Imprime desde la página 0 hasta la página actual del ciclo
            for (int pag = 0; pag < ciclo; pag++)
            {
                // NUEVA FÓRMULA: Se adapta exactamente a cómo llenaste el arreglo en crea_TMP
                int indice = (pag * NUM_TAREAS) + (tarea - 1);

                printf("%-8d %-8d %-8d %-8d %-8d\n",
                       tabla[indice].pagina,
                       tabla[indice].marco,
                       tabla[indice].estado,
                       tabla[indice].referencia,
                       tabla[indice].modificacion);
            }
        }

        printf("\nPresione cualquier tecla para continuar...");
        getch(); // Se añade el paréntesis y punto y coma faltantes
    }
}

void asignar_tareas(struct nodo_MT *mt, struct nodo_JT *lista_tareas, struct nodo_TMP **lista_tmp)
{

    // fifo o LRU
}
