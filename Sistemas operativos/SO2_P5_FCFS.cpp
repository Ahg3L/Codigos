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

struct nodo_PCB *Plista = NULL;
struct nodo_PCB *Ultimo = NULL;

/* ═════════════════════════════════════════
   STRUCTS
   ═════════════════════════════════════════ */

struct nodo_JT
{
    int tarea;
    int tamano;
    long loc_memo;
    int secuencia[SECUENCIA];
    struct nodo_JT *sig;
};

struct nodo_MMT
{
    int bloque;
    int capacidad;
    int loc_memo;
    int estado;
    struct nodo_MMT *sig;
};

struct nodo_PCB
{
    char proceso[15];
    int t_llegada;
    int ciclos;
    int estado;
    int memoria;
    char cpu_es[5];
    int num_disp;
    int no_arch;
    char tipo[10];
    struct nodo_PCB *sig;
};

/* ═════════════════════════════════════════
   PROTOTIPOS
   ═════════════════════════════════════════ */

struct nodo_JT *crea_JT(void);
struct nodo_MMT *crea_MMT(struct nodo_PCB *lista_pcb);
void crea_PCB(struct nodo_JT *lista_jt);

void asignar_tarea(struct nodo_MMT *memo, struct nodo_PCB *tarea);

void imprimir_JT(struct nodo_JT *lista);
void imprimir_MMT(struct nodo_MMT *lista);
void imprimir_PCB(struct nodo_PCB *lista);
void validar_tamano(void);

/* ═════════════════════════════════════════
   MAIN
   ═════════════════════════════════════════ */

int main()
{
    struct nodo_JT *lista_tareas;
    struct nodo_MMT *lista_memoria;

    srand((unsigned)time(NULL));

    lista_tareas = crea_JT();
    crea_PCB(lista_tareas);           /* PCB debe existir antes que MMT */
    lista_memoria = crea_MMT(Plista); /* MMT se dimensiona según PCB    */

    validar_tamano();

    printf("========================================\n");
    printf("                  (JT)                  \n");
    printf("========================================\n");
    imprimir_JT(lista_tareas);

    printf("\n========================================\n");
    printf("            (MMT) - VACIA               \n");
    printf("========================================\n");
    imprimir_MMT(lista_memoria); /* se ve vacía */

    asignar_tarea(lista_memoria, Plista); /* llenar */

    printf("\n========================================\n");
    printf("            (MMT) - ASIGNADA            \n");
    printf("========================================\n");
    imprimir_MMT(lista_memoria); /* se ve llena */

    printf("\n========================================\n");
    printf("  BLOQUE DE CONTROL DE PROCESOS (PCB)   \n");
    printf("========================================\n");
    imprimir_PCB(Plista);

    getch();

    return 0;
}

/* ═════════════════════════════════════════
   CREA JT
   ═════════════════════════════════════════ */

struct nodo_JT *crea_JT(void)
{
    struct nodo_JT *cabeza;
    struct nodo_JT *ultimo;
    struct nodo_JT *nuevo;
    int i, k, j, tmp;
    int pool[10];

    cabeza = NULL;
    ultimo = NULL;

    for (i = 0; i < NUM_TAREAS; i++)
    {
        nuevo = (struct nodo_JT *)malloc(sizeof(struct nodo_JT));
        if (nuevo == NULL)
        {
            printf("Error: no hay memoria para JT.\n");
            return cabeza;
        }

        nuevo->tarea = i + 1;
        nuevo->tamano = rand() % (999 - 100 + 1) + 100;
        nuevo->loc_memo = rand() % (99999 - 1000 + 1) + 1000;
        nuevo->sig = NULL;

        for (k = 0; k < 10; k++)
            pool[k] = k;

        for (k = 9; k > 0; k--)
        {
            j = rand() % (k + 1);
            tmp = pool[k];
            pool[k] = pool[j];
            pool[j] = tmp;
        }

        for (k = 0; k < SECUENCIA; k++)
            nuevo->secuencia[k] = pool[k];

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

/* ═════════════════════════════════════════
   CREA MMT
   ═════════════════════════════════════════ */

struct nodo_MMT *crea_MMT(struct nodo_PCB *lista_pcb)
{
    struct nodo_MMT *cabeza, *ultimo, *so, *nuevo;
    struct nodo_PCB *pcb_aux;
    int memoria_libre, bloque_num;

    /* Bloque 0: SO */
    so = (struct nodo_MMT *)malloc(sizeof(struct nodo_MMT));
    if (so == NULL)
    {
        printf("Error: no hay memoria para MMT.\n");
        return NULL;
    }

    so->bloque = 0;
    so->capacidad = SO;
    so->loc_memo = 0;
    so->estado = 1;
    so->sig = NULL;

    cabeza = so;
    ultimo = so;

    memoria_libre = MEMORIA_TOTAL - SO;
    bloque_num = 1;
    pcb_aux = lista_pcb;

    while (pcb_aux != NULL && memoria_libre >= pcb_aux->memoria)
    {
        nuevo = (struct nodo_MMT *)malloc(sizeof(struct nodo_MMT));
        if (nuevo == NULL)
        {
            printf("Error: no hay memoria para MMT.\n");
            return cabeza;
        }

        nuevo->bloque = bloque_num++;
        nuevo->capacidad = 0; /* vacío */
        nuevo->loc_memo = 0;  /* vacío */
        nuevo->estado = 0;    /* libre */
        nuevo->sig = NULL;

        memoria_libre -= pcb_aux->memoria;
        ultimo->sig = nuevo;
        ultimo = nuevo;

        pcb_aux = pcb_aux->sig;
    }

    return cabeza;
}

/* ═════════════════════════════════════════
   CREA PCB
   ═════════════════════════════════════════ */

void crea_PCB(struct nodo_JT *lista_jt)
{
    struct nodo_JT *jt;
    struct nodo_PCB *nuevo;
    int i, j;
    int usada[10];
    int pagina;
    int tiempo_actual = 0;

    Plista = NULL;
    Ultimo = NULL;

    jt = lista_jt;

    while (jt != NULL)
    {
        for (i = 0; i < 10; i++)
            usada[i] = 0;

        for (j = 0; j < SECUENCIA; j++)
        {
            pagina = jt->secuencia[j];

            if (pagina < 0 || pagina > 9)
                continue;

            if (usada[pagina] == 0)
            {
                usada[pagina] = 1;

                nuevo = (struct nodo_PCB *)malloc(sizeof(struct nodo_PCB));
                if (nuevo == NULL)
                {
                    printf("Error: no hay memoria para PCB.\n");
                    return;
                }

                sprintf(nuevo->proceso, "J%dP%d", jt->tarea, pagina);

                nuevo->t_llegada = tiempo_actual;
                nuevo->ciclos = 1 + rand() % (10 - 1 + 1);
                nuevo->estado = 1;
                nuevo->memoria = 10 + rand() % (120 - 10 + 1);

                if (rand() % 2 == 1)
                    strcpy(nuevo->cpu_es, "CPU");
                else
                    strcpy(nuevo->cpu_es, "E/S");

                nuevo->num_disp = 1 + rand() % (5 - 1 + 1);
                nuevo->no_arch = 1 + rand() % (10 - 1 + 1);

                if (rand() % 2 == 1)
                    strcpy(nuevo->tipo, "USR");
                else
                    strcpy(nuevo->tipo, "SIS");
                nuevo->sig = NULL;

                tiempo_actual++;

                if (Plista == NULL)
                {
                    Plista = nuevo;
                    Ultimo = nuevo;
                }
                else
                {
                    Ultimo->sig = nuevo;
                    Ultimo = nuevo;
                }
            }
        }

        jt = jt->sig;
    }
}

/* ═════════════════════════════════════════
   ASIGNAR TAREA  (llena MMT desde JT)
   ═════════════════════════════════════════ */

void asignar_tarea(struct nodo_MMT *memo, struct nodo_PCB *tarea)
{
    struct nodo_MMT *mmt_aux;
    struct nodo_PCB *pcb_aux;

    mmt_aux = memo->sig; /* saltar bloque 0 (SO) */
    pcb_aux = tarea;

    while (mmt_aux != NULL && pcb_aux != NULL)
    {
        mmt_aux->capacidad = pcb_aux->memoria;
        mmt_aux->loc_memo = rand() % (MEMORIA_TOTAL - SO) + SO;
        mmt_aux->estado = 1;

        mmt_aux = mmt_aux->sig;
        pcb_aux = pcb_aux->sig;
    }
}

/* ═════════════════════════════════════════
   IMPRIMIR JT
   ═════════════════════════════════════════ */

void imprimir_JT(struct nodo_JT *lista)
{
    struct nodo_JT *aux;
    int j;

    printf("%-8s %-10s %-12s  %s\n",
           "Tarea", "Tamano", "Loc_memo", "Secuencia");
    printf("---------------------------------------------------\n");

    aux = lista;
    while (aux != NULL)
    {
        printf("%-8d %-10d %-12ld  [", aux->tarea, aux->tamano, aux->loc_memo);
        for (j = 0; j < SECUENCIA; j++)
        {
            printf("%d", aux->secuencia[j]);
            if (j < SECUENCIA - 1)
                printf(",");
        }
        printf("]\n");
        aux = aux->sig;
    }
}

/* ═════════════════════════════════════════
   IMPRIMIR MMT
   ═════════════════════════════════════════ */

void imprimir_MMT(struct nodo_MMT *lista)
{
    struct nodo_MMT *aux;
    int total_usado;

    printf("%-8s %-12s %-12s %-8s\n",
           "Bloque", "Capacidad", "Loc_memo", "Estado");
    printf("--------------------------------------------\n");

    total_usado = 0;
    aux = lista;

    while (aux != NULL)
    {
        printf("%-8d %-12d %-12d %-8s\n",
               aux->bloque,
               aux->capacidad,
               aux->loc_memo,
               aux->estado == 1 ? "0" : "1");
        total_usado += aux->capacidad;
        aux = aux->sig;
    }

    printf("--------------------------------------------\n");
    printf("Total en MMT  : %d KB (%.2f MB)\n", total_usado, total_usado / 1024.0);
    printf("Memoria libre : %d KB (%.2f MB)\n",
           MEMORIA_TOTAL - total_usado, (MEMORIA_TOTAL - total_usado) / 1024.0);
}

/* ═════════════════════════════════════════
   IMPRIMIR PCB
   ═════════════════════════════════════════ */

void imprimir_PCB(struct nodo_PCB *lista)
{
    struct nodo_PCB *aux;

    printf("%-10s %-10s %-8s %-8s %-10s %-8s %-10s %-8s %-8s\n",
           "Proceso", "T.Llegada", "Ciclos", "Estado", "Memoria", "CPU/E/S",
           "Num_Disp", "No.Arch", "Tipo");
    printf("--------------------------------------------------------------------------------------\n");

    aux = lista;
    while (aux != NULL)
    {
        printf("%-10s %-10d %-8d %-8d %-10d %-8s %-10d %-8d %-8s\n",
               aux->proceso,
               aux->t_llegada,
               aux->ciclos,
               aux->estado,
               aux->memoria,
               aux->cpu_es,
               aux->num_disp,
               aux->no_arch,
               aux->tipo);
        aux = aux->sig;
    }

    printf("--------------------------------------------------------------------------------\n");
}

/* ═════════════════════════════════════════
   VALIDAR TAMAÑO
   ═════════════════════════════════════════ */

void validar_tamano(void)
{
    printf("\n=========== CONFIGURACION MEMORIA ===========\n");
    printf("Memoria total      = %d MB (%d KB)\n", MEMORIA_TOTAL_MB, MEMORIA_TOTAL);
    printf("SO ocupa           = %d KB\n", SO);
    printf("Memoria disponible = %d KB (%.2f MB)\n",
           MEMORIA_TOTAL - SO, (MEMORIA_TOTAL - SO) / 1024.0);
    printf("=============================================\n\n");
}