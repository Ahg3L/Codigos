#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>

#define MEMORIA_TOTAL_MB 2
#define MEMORIA_TOTAL (MEMORIA_TOTAL_MB * 1024)
#define SO 200
#define NUM_TAREAS 20
#define TAM_MARCO 64
#define SECUENCIA 8

// ─────────────────────────────────────────
// STRUCTS
// ─────────────────────────────────────────

struct nodo_MMT
{
    int bloque;
    int capacidad;
    int loc_memo;
    int estado;
    struct nodo_MMT *sig;
};

struct nodo_JT
{
    int segmento;
    int tamano;
    long loc_memo;
    int num_paginas;
    int secuencia[SECUENCIA];
    struct nodo_JT *sig;
};

struct nodo_SMT
{
    int tarea;
    int segmento;
    int tamano;
    int loc;
    struct nodo_SMT *sig;
};

// ─────────────────────────────────────────
// PROTOTIPOS
// ─────────────────────────────────────────

struct nodo_MMT *crea_MMT(struct nodo_SMT *lista_smt);
struct nodo_JT *crea_JT(void);
struct nodo_SMT *crea_SMT(struct nodo_JT *lista);
void imprimir_JT(struct nodo_JT *lista);
void imprimir_SMT(struct nodo_SMT *lista);
void imprimir_MMT(struct nodo_MMT *lista);
void asignar_tarea(struct nodo_MMT *memo, struct nodo_JT *tarea, struct nodo_SMT *segme);
void validar_tamano();

// ─────────────────────────────────────────
// MAIN
// ─────────────────────────────────────────

int main()
{
    srand(time(NULL));

    struct nodo_JT *lista_tareas = crea_JT();
    struct nodo_SMT *lista_smt = crea_SMT(lista_tareas);
    struct nodo_MMT *lista_memoria = crea_MMT(lista_smt);

    validar_tamano();

    printf("========================================\n");
    printf("                   (MMT)              \n");
    printf("========================================\n");
    imprimir_MMT(lista_memoria);

    printf("\n");
    printf("========================================\n");
    printf("                   (SMT)           \n");
    printf("========================================\n");
    imprimir_SMT(lista_smt);
    printf("========================================\n");
    printf("                   (JT)                 \n");
    printf("========================================\n");
    imprimir_JT(lista_tareas);

    printf("\n");

    asignar_tarea(lista_memoria, lista_tareas, lista_smt);

    printf("========================================\n");
    printf("                   (SMT)           \n");
    printf("========================================\n");
    imprimir_SMT(lista_smt);

    printf("\n");
    printf("========================================\n");
    printf("                   (MMT)              \n");
    printf("========================================\n");
    imprimir_MMT(lista_memoria);
    getch();

    return 0;
}

// ─────────────────────────────────────────
// CREA JT
// ─────────────────────────────────────────

struct nodo_JT *crea_JT(void)
{
    struct nodo_JT *cabeza = NULL, *ultimo = NULL;

    for (int i = 0; i < NUM_TAREAS; i++)
    {
        struct nodo_JT *nuevo = (struct nodo_JT *)malloc(sizeof(struct nodo_JT));

        nuevo->segmento = i + 1;
        nuevo->tamano = rand() % (999 - 100) + 100; // 100–999
        nuevo->loc_memo = rand() % (99999 - 1000) + 1000;

        nuevo->num_paginas = rand() % (9 - 1) + 1;
        nuevo->sig = NULL;

        if (!cabeza)
            cabeza = ultimo = nuevo;
        else
        {
            ultimo->sig = nuevo;
            ultimo = nuevo;
        }
    }
    return cabeza;
}

// ─────────────────────────────────────────
// CREA SMT
// ─────────────────────────────────────────

struct nodo_SMT *crea_SMT(struct nodo_JT *lista)
{
    struct nodo_SMT *cabeza = NULL, *ultimo = NULL;

    int contador_tarea = 1;
    for (struct nodo_JT *aux = lista; aux != NULL; aux = aux->sig)
    {

        int tam_segmento = 0;

        for (int s = 0; s < aux->num_paginas; s++)
        {
            struct nodo_SMT *nuevo = (struct nodo_SMT *)malloc(sizeof(struct nodo_SMT));

            nuevo->tarea = contador_tarea;
            nuevo->segmento = s + 1;
            nuevo->tamano = tam_segmento;
            nuevo->loc = 0;
            nuevo->sig = NULL;

            if (!cabeza)
                cabeza = ultimo = nuevo;
            else
            {
                ultimo->sig = nuevo;
                ultimo = nuevo;
            }
        }
        contador_tarea++;
    }
    return cabeza;
}

// ─────────────────────────────────────────
// CREA MMT
// ─────────────────────────────────────────

struct nodo_MMT *crea_MMT(struct nodo_SMT *lista_smt)
{
    struct nodo_MMT *cabeza = NULL, *ultimo = NULL;

    // Bloque 0: SO
    struct nodo_MMT *so = (struct nodo_MMT *)malloc(sizeof(struct nodo_MMT));
    so->bloque = 0;
    so->capacidad = SO;
    so->loc_memo = 0;
    so->estado = 1;
    so->sig = NULL;
    cabeza = ultimo = so;
    int conteo = SO;
    int memoria_libre = MEMORIA_TOTAL - SO;
    int num_bloque = 1;

    struct nodo_SMT *aux = lista_smt;

    while (aux != NULL)
    {
        int tarea_actual = aux->tarea;

        // Suma el tamaño total de la tarea
        int tam_tarea = 0;
        struct nodo_SMT *tmp = aux;
        while (tmp != NULL && tmp->tarea == tarea_actual)
        {
            tam_tarea += tmp->tamano;
            tmp = tmp->sig;
        }

        if (tam_tarea <= memoria_libre)
        {
            // Cabe → copia cada segmento como bloque
            while (aux != NULL && aux->tarea == tarea_actual)
            {
                struct nodo_MMT *nuevo = (struct nodo_MMT *)malloc(sizeof(struct nodo_MMT));

                nuevo->bloque = num_bloque++;
                nuevo->capacidad = 0;
                nuevo->loc_memo = 0;
                nuevo->estado = 0;
                nuevo->sig = NULL;

                ultimo->sig = nuevo;
                ultimo = nuevo;
                conteo += aux->tamano;
                memoria_libre -= aux->tamano;
                aux = aux->sig;
            }
        }
        else
        {
            // No cabe → salta la tarea completa
            while (aux != NULL && aux->tarea == tarea_actual)
                aux = aux->sig;
        }
    }

    return cabeza;
}

// ─────────────────────────────────────────
// IMPRIMIR JT
// ─────────────────────────────────────────

void imprimir_JT(struct nodo_JT *lista)
{
    printf("%-8s %-10s %-12s\n",
           "Tarea", "Tamano", "Loc_memo");
    printf("------------------------------------------------------------\n");

    for (struct nodo_JT *aux = lista; aux != NULL; aux = aux->sig)
    {
        printf("%-8d %-10d %-12ld\n",
               aux->segmento, aux->tamano, aux->loc_memo);

        printf("\n");
    }
}

// ─────────────────────────────────────────
// IMPRIMIR SMT
// ─────────────────────────────────────────

void imprimir_SMT(struct nodo_SMT *lista)
{
    printf("%-8s %-10s %-10s %-10s\n",
           "Tarea", "Segmento", "Tamano", "Loc");
    printf("----------------------------------------\n");

    int tarea_anterior = -1;
    for (struct nodo_SMT *aux = lista; aux != NULL; aux = aux->sig)
    {
        if (aux->tarea != tarea_anterior && tarea_anterior != -1)
            printf("----------------------------------------\n");

        if (aux->tarea != tarea_anterior)
            printf("%-8d %-10d %-10d %-10d\n",
                   aux->tarea, aux->segmento, aux->tamano, aux->loc);
        else
            printf("%-8s %-10d %-10d %-10d\n",
                   "", aux->segmento, aux->tamano, aux->loc);

        tarea_anterior = aux->tarea;
    }
}

// ─────────────────────────────────────────
// IMPRIMIR MMT
// ─────────────────────────────────────────

void imprimir_MMT(struct nodo_MMT *lista)
{
    printf("%-8s %-12s %-12s %-8s\n",
           "Bloque", "Capacidad", "Loc_memo", "Estado");
    printf("--------------------------------------------\n");

    int total_usado = 0;
    for (struct nodo_MMT *aux = lista; aux != NULL; aux = aux->sig)
    {
        printf("%-8d %-12d %-12d %-8s\n",
               aux->bloque,
               aux->capacidad,
               aux->loc_memo,
               aux->estado == 1 ? "0" : "1");
        total_usado += aux->capacidad;
    }

    printf("--------------------------------------------\n");
    printf("Total en MMT  : %d KB (%.2f MB)\n", total_usado, total_usado / 1024.0);
    printf("Memoria libre : %d KB (%.2f MB)\n", MEMORIA_TOTAL - total_usado, (MEMORIA_TOTAL - total_usado) / 1024.0);
}

void validar_tamano()
{
    printf("\n=========== CONFIGURACION MEMORIA ===========\n");

    printf("Memoria total          = %d MB (%d KB)\n", MEMORIA_TOTAL_MB, MEMORIA_TOTAL);
    printf("SO ocupa               = %d KB\n", SO);
    printf("Memoria disponible     = %d KB (%.2f MB)\n", MEMORIA_TOTAL - SO, (MEMORIA_TOTAL - SO) / 1024.0);
    printf("=============================================\n");
}

void asignar_tarea(struct nodo_MMT *memo, struct nodo_JT *tarea, struct nodo_SMT *segme)
{
    // ── 1. LLENAR SMT ────────────────────────────────────────────────────────
    // Recorremos la JT para saber el tamano total de cada tarea.
    // Luego distribuimos ese tamano entre los segmentos de la SMT de forma
    // aleatoria, garantizando que la suma sea EXACTAMENTE igual al tamano de la tarea.

    struct nodo_JT *jt_aux = tarea;

    struct nodo_SMT *smt_aux = segme;

    while (jt_aux != NULL && smt_aux != NULL)
    {
        int tarea_id = smt_aux->tarea;
        int tam_total = jt_aux->tamano; // tamano de la tarea en JT
        int num_segs = jt_aux->num_paginas;

        // Contar cuantos nodos SMT pertenecen a esta tarea
        int conteo = 0;
        struct nodo_SMT *tmp = smt_aux;
        while (tmp != NULL && tmp->tarea == tarea_id)
        {
            conteo++;
            tmp = tmp->sig;
        }

        // Distribuir tam_total entre los segmentos aleatoriamente
        // Tecnica: por cada segmento menos el ultimo, asignar un valor
        // aleatorio entre 1 y (restante - (segmentos_que_faltan - 1))
        // para que siempre quede al menos 1 para cada uno que siga.
        int restante = tam_total;
        int segs_restantes = conteo;

        while (smt_aux != NULL && smt_aux->tarea == tarea_id)
        {
            int tam_seg;
            if (segs_restantes == 1)
            {
                // Ultimo segmento: toma todo lo que queda
                tam_seg = restante;
            }
            else
            {
                // Rango: al menos 1, como maximo dejar 1 por cada segmento restante
                int max_posible = restante - (segs_restantes - 1);
                if (max_posible < 1)
                    max_posible = 1;
                tam_seg = rand() % max_posible + 1;
            }

            smt_aux->tamano = tam_seg;
            smt_aux->loc = rand() % (99999 - 1000) + 1000; // localidad aleatoria

            restante -= tam_seg;
            segs_restantes--;
            smt_aux = smt_aux->sig;
        }

        jt_aux = jt_aux->sig; // siguiente tarea en JT
    }

    // ── 2. LLENAR MMT ────────────────────────────────────────────────────────
    // La MMT tiene un nodo por cada segmento de las tareas que cupieron en memoria
    // (mas el nodo 0 del SO). Recorremos la MMT (saltando el bloque 0 del SO) y
    // la SMT en paralelo para heredar capacidad y loc_memo de cada segmento.

    struct nodo_MMT *mmt_aux = memo->sig; // saltar bloque 0 (SO)
    struct nodo_SMT *smt2 = segme;

    while (mmt_aux != NULL && smt2 != NULL)
    {
        // Heredar del segmento SMT correspondiente
        mmt_aux->capacidad = smt2->tamano;
        mmt_aux->loc_memo = smt2->loc;
        mmt_aux->estado = 0; // ocupado

        mmt_aux = mmt_aux->sig;
        smt2 = smt2->sig;
    }
}