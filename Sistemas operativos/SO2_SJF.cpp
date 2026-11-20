#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MEMORIA_TOTAL_MB 2
#define MEMORIA_TOTAL (MEMORIA_TOTAL_MB * 1024)
#define SO 200
#define NUM_TAREAS 20
#define TAM_MARCO 64
#define SECUENCIA 8
#define QUANTUM 3 /* Q = 3 ciclos por turno */

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

/* ── Round Robin ── */
struct nodo_PCB *copiar_lista_pcb(struct nodo_PCB *lista);
void ordenar_por_ciclos(struct nodo_PCB **lista);
void round_robin(struct nodo_PCB *lista);

/* ═════════════════════════════════════════
   MAIN
   ═════════════════════════════════════════ */

int main(void)
{
    struct nodo_JT *lista_tareas;
    struct nodo_MMT *lista_memoria;
    struct nodo_PCB *lista_rr;

    srand((unsigned)time(NULL));

    lista_tareas = crea_JT();
    crea_PCB(lista_tareas);
    lista_memoria = crea_MMT(Plista);

    validar_tamano();

    printf("========================================\n");
    printf("                  (JT)                  \n");
    printf("========================================\n");
    imprimir_JT(lista_tareas);

    printf("\n========================================\n");
    printf("            (MMT) - VACIA               \n");
    printf("========================================\n");
    imprimir_MMT(lista_memoria);

    asignar_tarea(lista_memoria, Plista);

    printf("\n========================================\n");
    printf("            (MMT) - ASIGNADA            \n");
    printf("========================================\n");
    imprimir_MMT(lista_memoria);

    printf("\n========================================\n");
    printf("  BLOQUE DE CONTROL DE PROCESOS (PCB)   \n");
    printf("========================================\n");
    imprimir_PCB(Plista);

    /* ── Round Robin ── */
    printf("\n========================================\n");
    printf("   PLANIFICACION ROUND ROBIN (Q=%d)      \n", QUANTUM);
    printf("========================================\n");

    lista_rr = copiar_lista_pcb(Plista); /* copia para no dañar PCB original */
    ordenar_por_ciclos(&lista_rr);       /* orden: menos ciclos → más ciclos  */
    round_robin(lista_rr);

    return 0;
}

/* ═════════════════════════════════════════
   CREA JT
   ═════════════════════════════════════════ */

struct nodo_JT *crea_JT(void)
{
    struct nodo_JT *cabeza, *ultimo, *nuevo;
    int i, k, j, tmp, pool[10];

    cabeza = ultimo = NULL;

    for (i = 0; i < NUM_TAREAS; i++)
    {
        nuevo = (struct nodo_JT *)malloc(sizeof(struct nodo_JT));
        if (!nuevo)
        {
            printf("Error: sin memoria JT.\n");
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

        if (!cabeza)
        {
            cabeza = ultimo = nuevo;
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

    so = (struct nodo_MMT *)malloc(sizeof(struct nodo_MMT));
    if (!so)
    {
        printf("Error: sin memoria MMT.\n");
        return NULL;
    }
    so->bloque = 0;
    so->capacidad = SO;
    so->loc_memo = 0;
    so->estado = 1;
    so->sig = NULL;
    cabeza = ultimo = so;

    memoria_libre = MEMORIA_TOTAL - SO;
    bloque_num = 1;
    pcb_aux = lista_pcb;

    while (pcb_aux && memoria_libre >= pcb_aux->memoria)
    {
        nuevo = (struct nodo_MMT *)malloc(sizeof(struct nodo_MMT));
        if (!nuevo)
        {
            printf("Error: sin memoria MMT.\n");
            return cabeza;
        }

        nuevo->bloque = bloque_num++;
        nuevo->capacidad = 0;
        nuevo->loc_memo = 0;
        nuevo->estado = 0;
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
    int i, j, usada[10], pagina, tiempo_actual = 0;

    Plista = Ultimo = NULL;
    jt = lista_jt;

    while (jt)
    {
        for (i = 0; i < 10; i++)
            usada[i] = 0;

        for (j = 0; j < SECUENCIA; j++)
        {
            pagina = jt->secuencia[j];
            if (pagina < 0 || pagina > 9)
                continue;
            if (usada[pagina])
                continue;

            usada[pagina] = 1;
            nuevo = (struct nodo_PCB *)malloc(sizeof(struct nodo_PCB));
            if (!nuevo)
            {
                printf("Error: sin memoria PCB.\n");
                return;
            }

            sprintf(nuevo->proceso, "J%dP%d", jt->tarea, pagina);
            nuevo->t_llegada = tiempo_actual++;
            nuevo->ciclos = 1 + rand() % (10 - 1) + 1;
            nuevo->estado = 1;
            nuevo->memoria = 10 + rand() % 111;
            if (rand() % 2)
                strcpy(nuevo->cpu_es, "CPU");
            else
                strcpy(nuevo->cpu_es, "E/S");
            nuevo->num_disp = 1 + rand() % 5;
            nuevo->no_arch = 1 + rand() % 10;
            if (rand() % 2)
                strcpy(nuevo->tipo, "USR");
            else
                strcpy(nuevo->tipo, "SIS");
            nuevo->sig = NULL;

            if (!Plista)
            {
                Plista = Ultimo = nuevo;
            }
            else
            {
                Ultimo->sig = nuevo;
                Ultimo = nuevo;
            }
        }
        jt = jt->sig;
    }
}

/* ═════════════════════════════════════════
   ASIGNAR TAREA
   ═════════════════════════════════════════ */

void asignar_tarea(struct nodo_MMT *memo, struct nodo_PCB *tarea)
{
    struct nodo_MMT *mmt_aux = memo->sig;
    struct nodo_PCB *pcb_aux = tarea;

    while (mmt_aux && pcb_aux)
    {
        mmt_aux->capacidad = pcb_aux->memoria;
        mmt_aux->loc_memo = rand() % (MEMORIA_TOTAL - SO) + SO;
        mmt_aux->estado = 1;
        mmt_aux = mmt_aux->sig;
        pcb_aux = pcb_aux->sig;
    }
}

/* ═════════════════════════════════════════
   COPIAR LISTA PCB  (copia superficial)
   ═════════════════════════════════════════ */

struct nodo_PCB *copiar_lista_pcb(struct nodo_PCB *lista)
{
    struct nodo_PCB *cabeza = NULL, *ultimo = NULL, *aux = lista, *nuevo;

    while (aux)
    {
        nuevo = (struct nodo_PCB *)malloc(sizeof(struct nodo_PCB));
        if (!nuevo)
        {
            printf("Error: sin memoria (copia PCB).\n");
            return cabeza;
        }
        *nuevo = *aux; /* copia todos los campos */
        nuevo->sig = NULL;
        if (!cabeza)
        {
            cabeza = ultimo = nuevo;
        }
        else
        {
            ultimo->sig = nuevo;
            ultimo = nuevo;
        }
        aux = aux->sig;
    }
    return cabeza;
}

/* ═════════════════════════════════════════
   ORDENAR PCB POR CICLOS  (burbuja)
   Ordena de MENOR a MAYOR número de ciclos.
   ═════════════════════════════════════════ */

void ordenar_por_ciclos(struct nodo_PCB **lista)
{
    struct nodo_PCB *i, *j;
    /* intercambio de datos (no de punteros) para no romper la cadena */
    char tmp_proceso[15], tmp_cpu_es[5], tmp_tipo[10];
    int tmp_t_llegada, tmp_ciclos, tmp_estado, tmp_memoria, tmp_num_disp, tmp_no_arch;

    if (!*lista)
        return;

    for (i = *lista; i->sig != NULL; i = i->sig)
    {
        for (j = i->sig; j != NULL; j = j->sig)
        {
            if (i->ciclos > j->ciclos)
            {
                /* intercambiar todos los campos */
                strcpy(tmp_proceso, i->proceso);
                strcpy(i->proceso, j->proceso);
                strcpy(j->proceso, tmp_proceso);
                strcpy(tmp_cpu_es, i->cpu_es);
                strcpy(i->cpu_es, j->cpu_es);
                strcpy(j->cpu_es, tmp_cpu_es);
                strcpy(tmp_tipo, i->tipo);
                strcpy(i->tipo, j->tipo);
                strcpy(j->tipo, tmp_tipo);

                tmp_t_llegada = i->t_llegada;
                i->t_llegada = j->t_llegada;
                j->t_llegada = tmp_t_llegada;
                tmp_ciclos = i->ciclos;
                i->ciclos = j->ciclos;
                j->ciclos = tmp_ciclos;
                tmp_estado = i->estado;
                i->estado = j->estado;
                j->estado = tmp_estado;
                tmp_memoria = i->memoria;
                i->memoria = j->memoria;
                j->memoria = tmp_memoria;
                tmp_num_disp = i->num_disp;
                i->num_disp = j->num_disp;
                j->num_disp = tmp_num_disp;
                tmp_no_arch = i->no_arch;
                i->no_arch = j->no_arch;
                j->no_arch = tmp_no_arch;
            }
        }
    }
}

/* ═════════════════════════════════════════
   ROUND ROBIN
   ─ Imprime la tabla PCB ordenada.
   ─ Simula la línea de tiempo:
       cada turno asigna Q ciclos al proceso
       activo; si le quedan ciclos, vuelve al
       final de la cola en el siguiente turno.
   ─ Al terminar muestra tiempos de retorno y
     tiempo promedio de espera.
   ═════════════════════════════════════════ */

void round_robin(struct nodo_PCB *lista)
{
    /* ── Contar procesos ── */
    struct nodo_PCB *aux;
    int n = 0;
    for (aux = lista; aux; aux = aux->sig)
        n++;

    if (n == 0)
    {
        printf("No hay procesos.\n");
        return;
    }

    /* ── Arreglos de trabajo ── */
    char **nombres = (char **)malloc(n * sizeof(char *));
    int *ciclos_orig = (int *)malloc(n * sizeof(int));
    int *restantes = (int *)malloc(n * sizeof(int));
    int *terminado = (int *)calloc(n, sizeof(int));
    int *t_fin = (int *)malloc(n * sizeof(int));
    int i;

    aux = lista;
    for (i = 0; i < n; i++, aux = aux->sig)
    {
        nombres[i] = aux->proceso;
        ciclos_orig[i] = aux->ciclos;
        restantes[i] = aux->ciclos;
        t_fin[i] = 0;
    }

    /* ── Imprimir PCB ordenada ── */
    printf("PCB ordenada (menor -> mayor ciclos):\n\n");
    imprimir_PCB(lista);

    /* ── Línea de tiempo ── */
    printf("\nQ = %d\n\n", QUANTUM);
    printf("Linea de tiempo:\n");
    printf("------------------------------------------------------\n");

    int tiempo = 0;
    int pendientes = n;
    int vuelta = 0;

    while (pendientes > 0)
    {
        for (i = 0; i < n; i++)
        {
            if (terminado[i])
                continue;

            int ejecutar = (restantes[i] >= QUANTUM) ? QUANTUM : restantes[i];
            restantes[i] -= ejecutar;

            /*
             * Imprimir fila de este turno:
             * columna tiempo | Proceso activo y sus ciclos
             */
            printf("t%-4d| %-8s ejecutando %-2d ciclos  <- (%d ciclos restantes)\n",
                   tiempo, nombres[i], ejecutar, restantes[i]);

            tiempo += ejecutar;

            if (restantes[i] == 0)
            {
                terminado[i] = 1;
                t_fin[i] = tiempo;
                pendientes--;
            }
        }

        vuelta++;
        /* seguridad: evitar bucle infinito */
        if (vuelta > n * 1000)
            break;
    }

    /* ── Tiempos de retorno y espera ── */
    printf("\n-------------------------------------------\n");
    printf("%-10s %-10s %-12s %-10s\n",
           "Proceso", "Ciclos", "T.Retorno", "T.Espera");
    printf("-------------------------------------------\n");

    double suma_espera = 0.0;
    for (i = 0; i < n; i++)
    {
        int t_retorno = t_fin[i] - 0; /* llegada asumida = 0 en RR puro */
        int t_espera = t_retorno - ciclos_orig[i];
        suma_espera += t_espera;
        printf("%-10s %-10d %-12d %-10d\n",
               nombres[i], ciclos_orig[i], t_retorno, t_espera);
    }

    printf("-------------------------------------------\n");
    printf("Tiempo promedio de espera : %.2f ciclos\n", suma_espera / n);

    free(ciclos_orig);
    free(restantes);
    free(terminado);
    free(t_fin);
    free(nombres);
}

/* ═════════════════════════════════════════
   IMPRIMIR JT
   ═════════════════════════════════════════ */

void imprimir_JT(struct nodo_JT *lista)
{
    struct nodo_JT *aux;
    int j;

    printf("%-8s %-10s %-12s  %s\n", "Tarea", "Tamano", "Loc_memo", "Secuencia");
    printf("---------------------------------------------------\n");

    for (aux = lista; aux; aux = aux->sig)
    {
        printf("%-8d %-10d %-12ld  [", aux->tarea, aux->tamano, aux->loc_memo);
        for (j = 0; j < SECUENCIA; j++)
        {
            printf("%d", aux->secuencia[j]);
            if (j < SECUENCIA - 1)
                printf(",");
        }
        printf("]\n");
    }
}

/* ═════════════════════════════════════════
   IMPRIMIR MMT
   ═════════════════════════════════════════ */

void imprimir_MMT(struct nodo_MMT *lista)
{
    struct nodo_MMT *aux;
    int total = 0;

    printf("%-8s %-12s %-12s %-8s\n", "Bloque", "Capacidad", "Loc_memo", "Estado");
    printf("--------------------------------------------\n");

    for (aux = lista; aux; aux = aux->sig)
    {
        printf("%-8d %-12d %-12d %-8s\n",
               aux->bloque, aux->capacidad, aux->loc_memo,
               aux->estado == 1 ? "0" : "1");
        total += aux->capacidad;
    }

    printf("--------------------------------------------\n");
    printf("Total en MMT  : %d KB (%.2f MB)\n", total, total / 1024.0);
    printf("Memoria libre : %d KB (%.2f MB)\n",
           MEMORIA_TOTAL - total, (MEMORIA_TOTAL - total) / 1024.0);
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