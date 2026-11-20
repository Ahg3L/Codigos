#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>

#define MEMORIA_TOTAL 3   // MB
#define SO 2              // MARCOS PARA SO
#define NUM_TAREAS 20
#define TAM_MARCO 64      // KB
#define SECUENCIA 8
#define MARCOS_POR_TAREA 3

/* =======================================================================
   STRUCTS
   ======================================================================= */
struct nodo_MT
{
    int nmarco;
    int locini;
    int Asignacion; // ID de tarea asignada (0 si libre)
    int estado;     // 2=SO, 0=libre
    struct nodo_MT *sig;
};

struct nodo_JT
{
    int id;
    int lineas;
    int loc_pmt;
    int tam_secuencia;
    int *arrsecu;
    int num_paginas;
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
    int tiempo_carga;
};

struct Memoria
{
    int memoria_kb;
    int marcos_totales;
    int marcos_disponibles;
} m;

/* =======================================================================
   VARIABLES GLOBALES
   ======================================================================= */
int fallos_total                         = 0;
int reemplazos_total                     = 0;
int aciertos_total                       = 0;
int reloj_fifo                           = 0;
int fallos_tarea[NUM_TAREAS]             = {0};
int reemplazos_tarea[NUM_TAREAS]         = {0};
int aciertos_tarea[NUM_TAREAS]           = {0};
int indice_secuencia[NUM_TAREAS]         = {0};
int cola_fifo[NUM_TAREAS][MARCOS_POR_TAREA];
int cola_inicio[NUM_TAREAS]              = {0};
int cola_fin[NUM_TAREAS]                 = {0};
int cola_size[NUM_TAREAS]                = {0};

/* =======================================================================
   PROTOTIPOS
   ======================================================================= */
struct nodo_MT  *crea_MT(void);
struct nodo_JT  *crea_JT(void);
struct nodo_TMP *crea_TMP(struct nodo_JT *lista_tareas);

void imprime_MT(struct nodo_MT *cabeza);
void imprimir_JT(struct nodo_JT *cabeza);
void imprimir_TMP(struct nodo_JT *lista_tareas, struct nodo_TMP *tabla);

void validar_tamano(void);
void asignar_tareas(struct nodo_MT *mt, struct nodo_JT *tareas, struct nodo_TMP *lista_tmp);
void fifo(struct nodo_MT *mt, struct nodo_JT *lista_tareas, struct nodo_TMP *tabla, int ciclo_actual);

int  obtener_offset(struct nodo_JT *lista_tareas, int id_buscado);
struct nodo_JT *obtener_tarea(struct nodo_JT *lista_tareas, int id_buscado);

/* =======================================================================
   MAIN
   ======================================================================= */
int main()
{
    struct nodo_MT  *lista_memoria = NULL;
    struct nodo_JT  *lista_tareas  = NULL;
    struct nodo_TMP *tmp           = NULL;

    validar_tamano();

    lista_memoria = crea_MT();
    lista_tareas  = crea_JT();

    imprime_MT(lista_memoria);
    imprimir_JT(lista_tareas);

    tmp = crea_TMP(lista_tareas);
    imprimir_TMP(lista_tareas, tmp);

    asignar_tareas(lista_memoria, lista_tareas, tmp);

    return 0;
}

/* =======================================================================
   VALIDAR TAMAÑO
   ======================================================================= */
void validar_tamano()
{
    printf("\n=========== CONFIGURACION MEMORIA ===========\n");
    printf("\nConversion de MB a KB\n");
    printf("Memoria KB = %d x 1024\n", MEMORIA_TOTAL);

    m.memoria_kb = MEMORIA_TOTAL * 1024;

    printf("Memoria KB = %d KB\n", m.memoria_kb);
    printf("\nCalculo de marcos:\n");

    m.marcos_totales = m.memoria_kb / TAM_MARCO;

    printf("Marcos Totales = %d / %d\n", m.memoria_kb, TAM_MARCO);
    printf("Marcos Totales = %d\n", m.marcos_totales);
    printf("\nMarcos reservados para SO = %d\n", SO);

    m.marcos_disponibles = m.marcos_totales - SO;

    printf("Marcos disponibles = %d\n", m.marcos_disponibles);
    printf("=============================================\n");
}

/* =======================================================================
   CREA MT
   ======================================================================= */
struct nodo_MT *crea_MT(void)
{
    struct nodo_MT *cabeza = NULL;
    struct nodo_MT *nuevo;
    struct nodo_MT *ultimo = NULL;

    for (int i = 0; i < m.marcos_totales; i++)
    {
        nuevo          = (struct nodo_MT *)malloc(sizeof(struct nodo_MT));
        nuevo->nmarco  = i;
        nuevo->locini  = i * TAM_MARCO;
        nuevo->sig     = NULL;

        if (i < SO)
        {
            nuevo->Asignacion = 1;
            nuevo->estado     = 2;
        }
        else
        {
            nuevo->Asignacion = 0;
            nuevo->estado     = 0;
        }

        if (cabeza == NULL)
        {
            cabeza = nuevo;
            ultimo = nuevo;
        }
        else
        {
            ultimo->sig = nuevo;
            ultimo      = nuevo;
        }
    }
    return cabeza;
}

/* =======================================================================
   IMPRIME MT
   ======================================================================= */
void imprime_MT(struct nodo_MT *aux)
{
    printf("\n");
    printf("=============== TABLA MT ===============\n");
    printf("| MARCO | LOC INI | ASIGNACION |\n");
    printf("========================================\n");

    while (aux != NULL)
    {
        if (aux->estado == 2)
            printf("| %-5d | %-7d | SO         |\n", aux->nmarco, aux->locini);
        else if (aux->Asignacion == 0)
            printf("| %-5d | %-7d | 0          |\n", aux->nmarco, aux->locini);
        else
            printf("| %-5d | %-7d | J%-9d |\n", aux->nmarco, aux->locini, aux->Asignacion - 1);

        aux = aux->sig;
    }
    printf("========================================\n");
}

/* =======================================================================
   CREA JT
   ======================================================================= */
struct nodo_JT *crea_JT(void)
{
    struct nodo_JT *cabeza = NULL;
    struct nodo_JT *nuevo  = NULL;
    struct nodo_JT *ultimo = NULL;

    srand(time(NULL));

    for (int i = 0; i < NUM_TAREAS; i++)
    {
        nuevo               = (struct nodo_JT *)malloc(sizeof(struct nodo_JT));
        nuevo->id           = i + 1;
        nuevo->lineas       = rand() % 900 + 100;       // 100 – 999
        nuevo->num_paginas  = nuevo->lineas / 100;       // depende de lineas
        nuevo->loc_pmt      = rand() % 90000 + 10000;
        nuevo->tam_secuencia = SECUENCIA;
        nuevo->arrsecu      = (int *)malloc(sizeof(int) * SECUENCIA);
        nuevo->sig          = NULL;

        for (int j = 0; j < SECUENCIA; j++)
        {
            if (j == 0)
                nuevo->arrsecu[j] = 0;
            else
                nuevo->arrsecu[j] = rand() % nuevo->num_paginas;
        }

        if (cabeza == NULL)
        {
            cabeza = nuevo;
            ultimo = nuevo;
        }
        else
        {
            ultimo->sig = nuevo;
            ultimo      = nuevo;
        }
    }
    return cabeza;
}

/* =======================================================================
   IMPRIME JT
   ======================================================================= */
void imprimir_JT(struct nodo_JT *cabeza)
{
    struct nodo_JT *actual = cabeza;

    printf("\n============================================================\n");
    printf("  JT - Lista de tareas\n");
    printf("============================================================\n");
    printf("  %-6s %-8s %-10s %-9s %s\n",
           "ID", "LINEAS", "LOC PMT", "PAGINAS", "SECUENCIA");
    printf("------------------------------------------------------------\n");

    while (actual != NULL)
    {
        printf("  J%-5d %-8d %-10d %-9d ",
               actual->id, actual->lineas, actual->loc_pmt, actual->num_paginas);

        for (int j = 0; j < actual->tam_secuencia; j++)
            printf("%d ", actual->arrsecu[j]);

        printf("\n");
        actual = actual->sig;
    }
    printf("============================================================\n");
}

/* =======================================================================
   CREA TMP
   ======================================================================= */
struct nodo_TMP *crea_TMP(struct nodo_JT *lista_tareas)
{
    struct nodo_JT *auxJT        = lista_tareas;
    int             paginas_totales = 0;

    while (auxJT != NULL)
    {
        paginas_totales += auxJT->num_paginas;
        auxJT = auxJT->sig;
    }

    struct nodo_TMP *tabla =
        (struct nodo_TMP *)malloc(sizeof(struct nodo_TMP) * paginas_totales);

    if (tabla == NULL)
    {
        printf("Error al reservar memoria para TMP\n");
        return NULL;
    }

    auxJT      = lista_tareas;
    int indice = 0;

    while (auxJT != NULL)
    {
        for (int pagina = 0; pagina < auxJT->num_paginas; pagina++)
        {
            tabla[indice].idTarea      = auxJT->id;
            tabla[indice].pagina       = pagina;
            tabla[indice].marco        = 0;
            tabla[indice].estado       = 0;
            tabla[indice].referencia   = 0;
            tabla[indice].modificacion = 0;
            tabla[indice].tiempo_carga = 0;
            indice++;
        }
        auxJT = auxJT->sig;
    }

    return tabla;
}

/* =======================================================================
   IMPRIME TMP (todas las tareas)
   ======================================================================= */
void imprimir_TMP(struct nodo_JT *lista_tareas, struct nodo_TMP *tabla)
{
    struct nodo_JT *auxJT  = lista_tareas;
    int             offset = 0;

    while (auxJT != NULL)
    {
        printf("\n==============================================");
        printf("\nTABLA DE MAPA DE PAGINAS - J%d", auxJT->id);
        printf("\n==============================================");
        printf("\n%-8s %-10s %-10s %-10s %-10s %-12s",
               "Pag", "Marco", "Estado", "Ref", "Mod", "TiempoCarga");
        printf("\n----------------------------------------------");

        for (int p = 0; p < auxJT->num_paginas; p++)
        {
            int idx = offset + p;
            printf("\n%-8d %-10d %-10d %-10d %-10d %-12d",
                   tabla[idx].pagina,
                   tabla[idx].marco,
                   tabla[idx].estado,
                   tabla[idx].referencia,
                   tabla[idx].modificacion,
                   tabla[idx].tiempo_carga);
        }

        printf("\n");
        offset += auxJT->num_paginas;
        auxJT   = auxJT->sig;
    }
    printf("\n");
}

/* =======================================================================
   AUXILIAR: obtener offset de una tarea en tabla TMP
   ======================================================================= */
int obtener_offset(struct nodo_JT *lista_tareas, int id_buscado)
{
    struct nodo_JT *aux    = lista_tareas;
    int             offset = 0;

    while (aux != NULL)
    {
        if (aux->id == id_buscado)
            return offset;
        offset += aux->num_paginas;
        aux     = aux->sig;
    }
    return -1;
}

/* =======================================================================
   AUXILIAR: obtener nodo JT por id
   ======================================================================= */
struct nodo_JT *obtener_tarea(struct nodo_JT *lista_tareas, int id_buscado)
{
    struct nodo_JT *aux = lista_tareas;

    while (aux != NULL)
    {
        if (aux->id == id_buscado)
            return aux;
        aux = aux->sig;
    }
    return NULL;
}

/* =======================================================================
   ASIGNAR TAREAS
   ======================================================================= */
void asignar_tareas(struct nodo_MT *mt, struct nodo_JT *lista_tareas, struct nodo_TMP *lista_tmp)
{
    int opc = 0;

    while (opc != 1 && opc != 2)
    {
        printf("\nElige el metodo de reemplazo");
        printf("\n1.- FIFO");
        printf("\n2.- LRU");
        printf("\nOpcion: ");
        scanf("%d", &opc);

        if (opc != 1 && opc != 2)
        {
            printf("\nOpcion incorrecta\n");
            getch();
        }
    }

    if (opc == 1)
    {
        // Inicializar todas las estructuras de control
        memset(cola_fifo,        -1, sizeof(cola_fifo));
        memset(cola_inicio,       0, sizeof(cola_inicio));
        memset(cola_fin,          0, sizeof(cola_fin));
        memset(cola_size,         0, sizeof(cola_size));
        memset(indice_secuencia,  0, sizeof(indice_secuencia));

        for (int ciclo = 0; ciclo < SECUENCIA; ciclo++)
        {
            fifo(mt, lista_tareas, lista_tmp, ciclo);
            getch();
        }

        // Resumen final
        printf("\n\n========================================");
        printf("\n        RESUMEN FINAL FIFO");
        printf("\n========================================\n");
        printf("%-8s %-10s %-12s %-10s\n",
               "Tarea", "Fallos", "Reemplazos", "Aciertos");
        printf("----------------------------------------\n");

        struct nodo_JT *aux = lista_tareas;
        while (aux != NULL)
        {
            int id = aux->id - 1;
            printf("J%-7d %-10d %-12d %-10d\n",
                   aux->id,
                   fallos_tarea[id],
                   reemplazos_tarea[id],
                   aciertos_tarea[id]);
            aux = aux->sig;
        }

        printf("----------------------------------------\n");
        printf("%-8s %-10d %-12d %-10d\n",
               "TOTAL", fallos_total, reemplazos_total, aciertos_total);
        printf("========================================\n");
    }
    else if (opc == 2)
    {
        printf("\nLRU aun no implementado\n");
        getch();
    }
}

/* =======================================================================
   FIFO
   ======================================================================= */
void fifo(struct nodo_MT *mt,
          struct nodo_JT *lista_tareas,
          struct nodo_TMP *tabla,
          int ciclo_actual)
{

    printf("\n====================================\n");

    // 1. Preguntar qué tarea procesar
    int id_tarea = 0;
    printf("Ingresa el ID de la tarea a procesar (1-%d): ", NUM_TAREAS);
    scanf("%d", &id_tarea);

    struct nodo_JT *tarea = obtener_tarea(lista_tareas, id_tarea);
    if (tarea == NULL)
    {
        printf("Tarea %d no encontrada.\n", id_tarea);
        return;
    }

    int id     = tarea->id - 1;   // índice 0-based para arreglos globales
    int offset = obtener_offset(lista_tareas, id_tarea);

    if (offset == -1)
    {
        printf("Error: no se encontro offset para tarea %d\n", id_tarea);
        return;
    }

    // 2. Obtener página pedida desde arrsecu con contador propio por tarea
    int idx_seq = indice_secuencia[id];
    if (idx_seq >= tarea->tam_secuencia)
    {
        printf("La tarea J%d ya agoto su secuencia.\n", tarea->id);
        return;
    }

    int pagina_pedida = tarea->arrsecu[idx_seq];
    indice_secuencia[id]++;

    printf("\nJ%d solicita Pagina %d  (secuencia[%d])\n",
           tarea->id, pagina_pedida, idx_seq);

    // Validar rango
    if (pagina_pedida < 0 || pagina_pedida >= tarea->num_paginas)
    {
        printf("Pagina %d invalida para J%d (num_paginas=%d)\n",
               pagina_pedida, tarea->id, tarea->num_paginas);
        return;
    }

    int idx_pagina = offset + pagina_pedida;

    // 3. Limpiar bits de referencia de todas las páginas de esta tarea
    for (int p = 0; p < tarea->num_paginas; p++)
        tabla[offset + p].referencia = 0;

    // 4. Acierto o fallo
    if (tabla[idx_pagina].estado == 1)
    {
        // --- ACIERTO ---
        aciertos_total++;
        aciertos_tarea[id]++;
        tabla[idx_pagina].referencia = 1;

        printf("ACIERTO - Pagina %d ya esta en marco (locini=%d)\n",
               pagina_pedida, tabla[idx_pagina].marco);
    }
    else
    {
        // --- FALLO ---
        fallos_total++;
        fallos_tarea[id]++;

        if (cola_size[id] < MARCOS_POR_TAREA)
        {
            // Hay marco libre para esta tarea
            // J0 -> marcos SO+0, SO+1, SO+2
            // J1 -> marcos SO+3, SO+4, SO+5  etc.
            int marco_base = SO + id * MARCOS_POR_TAREA;
            int slot       = cola_size[id];
            int num_marco  = marco_base + slot;

            // Buscar locini en MT
            struct nodo_MT *aux_mt = mt;
            while (aux_mt != NULL && aux_mt->nmarco != num_marco)
                aux_mt = aux_mt->sig;

            int loc = (aux_mt != NULL) ? aux_mt->locini : num_marco * TAM_MARCO;

            tabla[idx_pagina].marco        = loc;
            tabla[idx_pagina].estado       = 1;
            tabla[idx_pagina].referencia   = 1;

            reloj_fifo++;
            tabla[idx_pagina].tiempo_carga = reloj_fifo;

            // Encolar
            cola_fifo[id][cola_fin[id]] = idx_pagina;
            cola_fin[id]  = (cola_fin[id] + 1) % MARCOS_POR_TAREA;
            cola_size[id]++;

            printf("FALLO -> Marco libre asignado (locini=%d) a Pagina %d\n",
                   loc, pagina_pedida);
        }
        else
        {
            // Marcos llenos: reemplazar el más antiguo (FIFO)
            reemplazos_total++;
            reemplazos_tarea[id]++;

            int idx_viejo   = cola_fifo[id][cola_inicio[id]];
            int marco_viejo = tabla[idx_viejo].marco;
            int pag_vieja   = tabla[idx_viejo].pagina;

            // Desalojar página vieja
            tabla[idx_viejo].estado       = 0;
            tabla[idx_viejo].marco        = 0;
            tabla[idx_viejo].referencia   = 0;
            tabla[idx_viejo].tiempo_carga = 0;

            // Cargar página nueva en ese mismo marco
            tabla[idx_pagina].marco        = marco_viejo;
            tabla[idx_pagina].estado       = 1;
            tabla[idx_pagina].referencia   = 1;

            reloj_fifo++;
            tabla[idx_pagina].tiempo_carga = reloj_fifo;

            // Actualizar cola circular: reemplazar slot, avanzar inicio
            cola_fifo[id][cola_inicio[id]] = idx_pagina;
            cola_inicio[id] = (cola_inicio[id] + 1) % MARCOS_POR_TAREA;

            printf("REEMPLAZO -> Sale Pag %d (locini=%d), entra Pag %d\n",
                   pag_vieja, marco_viejo, pagina_pedida);
        }
    }

    // 5. Estadísticas y cola
    printf("\n--- Estadisticas J%d ---\n", tarea->id);

    printf("Secuencia     : ");
    for (int s = 0; s < tarea->tam_secuencia; s++)
        printf("%d ", tarea->arrsecu[s]);
    printf("\n");

    printf("Indice : %d / %d\n", indice_secuencia[id], tarea->tam_secuencia);
    printf("Fallos        : %d\n",   fallos_tarea[id]);
    printf("Reemplazos    : %d\n",   reemplazos_tarea[id]);
    printf("Aciertos      : %d\n",   aciertos_tarea[id]);
    printf("Reloj FIFO    : %d\n",   reloj_fifo);

    printf("Cola FIFO     : [");
    for (int s = 0; s < MARCOS_POR_TAREA; s++)
    {
        int pos = (cola_inicio[id] + s) % MARCOS_POR_TAREA;
        if (s < cola_size[id])
            printf(" Pag%d(t=%d)",
                   tabla[cola_fifo[id][pos]].pagina,
                   tabla[cola_fifo[id][pos]].tiempo_carga);
        else
            printf(" -");
    }
    printf(" ]\n");

    // 6. TMP solo de la tarea afectada
    printf("\n--- TMP J%d ---\n", tarea->id);
    printf("%-8s %-10s %-10s %-10s %-10s %-12s\n",
           "Pag", "Marco", "Estado", "Ref", "Mod", "TiempoCarga");
    printf("--------------------------------------------------------------\n");

    for (int p = 0; p < tarea->num_paginas; p++)
    {
        int idx = offset + p;
        printf("%-8d %-10d %-10d %-10d %-10d %-12d\n",
               tabla[idx].pagina,
               tabla[idx].marco,
               tabla[idx].estado,
               tabla[idx].referencia,
               tabla[idx].modificacion,
               tabla[idx].tiempo_carga);
    }
}