/* =====================================================================
   SIMULADOR DE ADMINISTRACION DE MEMORIA + PLANIFICADOR ROUND ROBIN
   =====================================================================
   Version compatible con Borland C++ / Turbo C++ (compiladores viejos).

   Dos cosas que estos compiladores exigen y que un compilador moderno
   (gcc/g++ actuales) deja pasar sin problema:

     1) Todo malloc() debe llevar un CAST explicito al tipo del puntero.
        En C moderno el compilador lo hace solo; en C++ (y en el modo
        estricto de Borland) hay que escribirlo tu mismo:
              nuevo = (struct nodo_MMT *) malloc(sizeof(struct nodo_MMT));

     2) TODAS las variables de una funcion (o de un bloque { }) se
        deben declarar al PRINCIPIO de ese bloque, antes de cualquier
        instruccion. No se puede declarar una variable "a la mitad"
        de una funcion como en C moderno. Por eso en este archivo
        veras que arriba de cada funcion aparecen TODAS las variables
        que se van a usar, aunque se usen mas abajo.

   La logica del programa es identica a la version anterior; solo
   cambia el ORDEN en el que se escriben las declaraciones.

   IDEA GENERAL DEL PROGRAMA:
   1) FASE DE MEMORIA: se calculan los marcos de memoria (MMT), se
      generan 20 tareas con sus paginas necesarias (JT + PMT), y se
      asignan marcos fisicos iniciales a algunas paginas.
   2) FASE DE PLANIFICACION: de la secuencia de referencias de cada
      tarea se sacan las paginas UNICAS y por cada una se crea un
      proceso (PCB). Ej: T3 con paginas {0,1,4} genera T3P0, T3P1, T3P4.
   3) FASE DE EJECUCION: los PCB corren con Round Robin. Cada vez que
      un proceso corre se simula FIFO para actualizar que paginas de
      su tarea estan en memoria fisica.

   ESTADOS DEL PCB:
        1 = Nuevo   2 = Listo   3 = Ejecutando   4 = Bloqueado   5 = Terminado
   ===================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <conio.h> /* para getch() al final del programa */

/* =====================================================================
   1. CONFIGURACION
   ===================================================================== */
#define MEMORIA_MB 3
#define MEMORIA_KB 0

#define MARCO_B 0
#define MARCO_KB 64

#define SO_MARCOS 2
#define SO_LINEAS 0
#define SO_KB 0

#define NUM_TAREAS 20
#define MIN_CICLOS 5
#define MAX_CICLOS 12
#define QUANTUM 5

#define TAM_SECUENCIA 8
#define MARCOS_POR_TAREA 3

#define ST_NUEVO 1
#define ST_LISTO 2
#define ST_EJECUTANDO 3
#define ST_BLOQUEADO 4
#define ST_TERMINADO 5

/* =====================================================================
   2. ESTRUCTURAS DE DATOS
   ===================================================================== */
struct nodo_MMT
{
    int marco;
    float direccion_inicio;
    int ocupado_por_SO;
    struct nodo_MMT *sig;
};

struct nodo_PMT
{
    int pagina;
    int marco_fisico;
    int presente;
    int bit_referencia;
    int bit_modificacion;
    struct nodo_PMT *sig;
};

struct nodo_JT
{
    char nombre[10];
    int lineas;
    int direccion_simulada;
    int secuencia[TAM_SECUENCIA];
    struct nodo_PMT *pmt;
    struct nodo_JT *sig;
};

struct PCB
{
    char nombre[15];
    int tarea_origen;
    int tiempo_llegada;
    int ciclos_cpu;
    int ciclos_restantes;
    int estado;
    int es_ES;
    int tiempo_inicio;
    int tiempo_fin;
    int tiempo_retorno;
};

/* =====================================================================
   3. VARIABLES GLOBALES
   ===================================================================== */
struct nodo_MMT *mmt_inicio = NULL, *mmt_final = NULL;
struct nodo_JT *jt_inicio = NULL, *jt_final = NULL;
struct nodo_PMT *pmt_de_tarea[NUM_TAREAS];

struct PCB procesos[NUM_TAREAS * TAM_SECUENCIA];
int total_procesos = 0;

float memoria_total_kb = 0;
float marco_kb = 0;
int marcos_totales = 0;
int marcos_de_SO = 0;

/* =====================================================================
   4. FUNCIONES AUXILIARES
   ===================================================================== */
int redondear_arriba(float num)
{
    int entero;
    entero = (int)num;
    if (num > entero)
        return entero + 1;
    return entero;
}

int es_valor_unico(int valor, int *usados, int cuantos)
{
    int i;
    for (i = 0; i < cuantos; i++)
    {
        if (usados[i] == valor)
            return 0;
    }
    return 1;
}

void linea(char c, int n)
{
    int i;
    for (i = 0; i < n; i++)
        putchar(c);
    putchar('\n');
}

/* =====================================================================
   5. FASE 1: CALCULOS DE MEMORIA
   ===================================================================== */
void calcular_memoria(void)
{
    /* -- sin variables locales -- */
    printf("\n");
    linea('=', 60);
    printf("  CALCULOS DE MEMORIA\n");
    linea('=', 60);

    memoria_total_kb = (MEMORIA_MB > 0) ? (float)MEMORIA_MB * 1024.0f : (float)MEMORIA_KB;
    printf("  Memoria total       : %.2f KB\n", memoria_total_kb);

    marco_kb = (MARCO_B > 0) ? (float)MARCO_B / 1024.0f : (float)MARCO_KB;
    printf("  Tamano de marco     : %.2f KB\n", marco_kb);

    marcos_totales = (int)(memoria_total_kb / marco_kb);
    printf("  Marcos totales      : %.2f / %.2f = %d marcos\n",
           memoria_total_kb, marco_kb, marcos_totales);

    if (SO_MARCOS > 0)
    {
        marcos_de_SO = SO_MARCOS;
    }
    else if (SO_LINEAS > 0)
    {
        marcos_de_SO = redondear_arriba((float)SO_LINEAS / marco_kb);
    }
    else
    {
        marcos_de_SO = redondear_arriba((float)SO_KB / marco_kb);
    }
    printf("  Marcos del S.O.     : %d\n", marcos_de_SO);
}

/* =====================================================================
   6. FASE 1: CREAR TABLAS (MMT, JT, PMT)
   ===================================================================== */
void crear_MMT(void)
{
    int i;
    float direccion;
    struct nodo_MMT *nuevo;

    direccion = 0.0f;
    for (i = 0; i < marcos_totales; i++)
    {
        nuevo = (struct nodo_MMT *)malloc(sizeof(struct nodo_MMT));
        nuevo->marco = i;
        nuevo->direccion_inicio = direccion;
        nuevo->ocupado_por_SO = (i < marcos_de_SO) ? 1 : 0;
        nuevo->sig = NULL;

        if (mmt_inicio == NULL)
        {
            mmt_inicio = nuevo;
            mmt_final = nuevo;
        }
        else
        {
            mmt_final->sig = nuevo;
            mmt_final = nuevo;
        }
        direccion += marco_kb;
    }
}

struct nodo_PMT *crear_PMT(int num_paginas)
{
    struct nodo_PMT *inicio, *final, *nuevo;
    int p;

    inicio = NULL;
    final = NULL;
    for (p = 0; p < num_paginas; p++)
    {
        nuevo = (struct nodo_PMT *)malloc(sizeof(struct nodo_PMT));
        nuevo->pagina = p;
        nuevo->marco_fisico = -1;
        nuevo->presente = 0;
        nuevo->bit_referencia = 0;
        nuevo->bit_modificacion = 0;
        nuevo->sig = NULL;

        if (inicio == NULL)
        {
            inicio = nuevo;
            final = nuevo;
        }
        else
        {
            final->sig = nuevo;
            final = nuevo;
        }
    }
    return inicio;
}

void crear_JT_y_PMTs(void)
{
    int lineas_usadas[NUM_TAREAS];
    int i, s, paginas_necesarias, tope_pagina, lineas_rand;
    struct nodo_JT *nuevo;

    for (i = 0; i < NUM_TAREAS; i++)
    {
        do
        {
            lineas_rand = (rand() % 801) + 200;
        } while (!es_valor_unico(lineas_rand, lineas_usadas, i));
        lineas_usadas[i] = lineas_rand;

        paginas_necesarias = redondear_arriba((float)lineas_rand / marco_kb);

        nuevo = (struct nodo_JT *)malloc(sizeof(struct nodo_JT));
        sprintf(nuevo->nombre, "T%d", i + 1);
        nuevo->lineas = lineas_rand;
        nuevo->direccion_simulada = (rand() % 90000) + 8000;

        tope_pagina = (paginas_necesarias > 10) ? 10 : paginas_necesarias;
        nuevo->secuencia[0] = 0;
        for (s = 1; s < TAM_SECUENCIA; s++)
        {
            nuevo->secuencia[s] = rand() % tope_pagina;
        }

        nuevo->sig = NULL;
        if (jt_inicio == NULL)
        {
            jt_inicio = nuevo;
            jt_final = nuevo;
        }
        else
        {
            jt_final->sig = nuevo;
            jt_final = nuevo;
        }

        pmt_de_tarea[i] = crear_PMT(paginas_necesarias);
    }
}

/* =====================================================================
   7. FASE 1: ASIGNAR MARCOS FISICOS INICIALES
   ===================================================================== */
void asignar_marcos_iniciales(int *marcos_libres)
{
    int siguiente_marco_libre, indice_tarea, asignados_a_esta_tarea, s;
    struct nodo_JT *tarea;
    struct nodo_PMT *pagina;

    siguiente_marco_libre = marcos_de_SO;
    tarea = jt_inicio;
    indice_tarea = 0;

    while (tarea != NULL)
    {
        asignados_a_esta_tarea = 0;

        /* 1) prioridad: paginas que aparecen en la secuencia de referencias */
        for (s = 0; s < TAM_SECUENCIA && asignados_a_esta_tarea < MARCOS_POR_TAREA && *marcos_libres > 0; s++)
        {
            int pag_buscada;
            pag_buscada = tarea->secuencia[s];
            pagina = pmt_de_tarea[indice_tarea];
            while (pagina != NULL && pagina->pagina != pag_buscada)
            {
                pagina = pagina->sig;
            }
            if (pagina != NULL && pagina->presente == 0)
            {
                pagina->marco_fisico = siguiente_marco_libre++;
                pagina->presente = 1;
                (*marcos_libres)--;
                asignados_a_esta_tarea++;
            }
        }

        /* 2) rellenar con las primeras paginas libres si aun sobran marcos */
        pagina = pmt_de_tarea[indice_tarea];
        while (pagina != NULL && asignados_a_esta_tarea < MARCOS_POR_TAREA && *marcos_libres > 0)
        {
            if (pagina->presente == 0)
            {
                pagina->marco_fisico = siguiente_marco_libre++;
                pagina->presente = 1;
                (*marcos_libres)--;
                asignados_a_esta_tarea++;
            }
            pagina = pagina->sig;
        }

        tarea = tarea->sig;
        indice_tarea++;
    }
}

/* =====================================================================
   8. IMPRESION DE TABLAS
   ===================================================================== */
void imprimir_MMT(void)
{
    struct nodo_MMT *m;

    m = mmt_inicio;
    printf("\n");
    linea('=', 60);
    printf("  1. TABLA DE MEMORIA (MMT)\n");
    linea('=', 60);
    printf("  %-8s %-16s %-10s\n", "Marco", "Direccion(KB)", "Ocupado");
    linea('-', 60);
    while (m != NULL)
    {
        printf("  %-8d %-16.2f %-10s\n", m->marco, m->direccion_inicio,
               m->ocupado_por_SO ? "S.O." : "libre");
        m = m->sig;
    }
}

void imprimir_JT(void)
{
    struct nodo_JT *t;
    int s;

    t = jt_inicio;
    printf("\n");
    linea('=', 60);
    printf("  2. TABLA DE TAREAS (JT)\n");
    linea('=', 60);
    printf("  %-6s %-8s %-12s %s\n", "Tarea", "Lineas", "Dir.simulada", "Secuencia de paginas");
    linea('-', 60);
    while (t != NULL)
    {
        printf("  %-6s %-8d %-12d ", t->nombre, t->lineas, t->direccion_simulada);
        for (s = 0; s < TAM_SECUENCIA; s++)
        {
            printf("%d ", t->secuencia[s]);
        }
        printf("\n");
        t = t->sig;
    }
}

void imprimir_PMTs(void)
{
    struct nodo_JT *t;
    struct nodo_PMT *p;
    int indice;
    char marco_txt[8];

    t = jt_inicio;
    indice = 0;
    printf("\n");
    linea('=', 60);
    printf("  3. TABLAS DE PAGINAS (PMT) POR TAREA\n");
    linea('=', 60);
    while (t != NULL)
    {
        p = pmt_de_tarea[indice];
        printf("\n  --- PMT de %s ---\n", t->nombre);
        printf("  %-8s %-8s %-10s %-6s %-6s\n", "Pagina", "Marco", "Presente", "Ref", "Mod");
        while (p != NULL)
        {
            if (p->marco_fisico == -1)
            {
                strcpy(marco_txt, "-");
            }
            else
            {
                sprintf(marco_txt, "%d", p->marco_fisico);
            }
            printf("  %-8d %-8s %-10s %-6d %-6d\n",
                   p->pagina, marco_txt,
                   p->presente ? "si" : "no", p->bit_referencia, p->bit_modificacion);
            p = p->sig;
        }
        t = t->sig;
        indice++;
    }
}

/* =====================================================================
   9. FASE 2: GENERAR PROCESOS (PCB) DESDE LAS TAREAS
   ===================================================================== */
void generar_procesos_desde_tareas(void)
{
    struct nodo_JT *t;
    struct PCB *nuevo;
    int tiempo_llegada, indice_tarea;
    int paginas_unicas[TAM_SECUENCIA];
    int cuantas_unicas, i, j, pag, ya_existe, tmp;

    t = jt_inicio;
    tiempo_llegada = 0;
    indice_tarea = 0;

    printf("\n");
    linea('=', 60);
    printf("  4. PLANIFICADOR: GENERACION DE PROCESOS (PCB)\n");
    linea('=', 60);

    while (t != NULL)
    {
        cuantas_unicas = 0;

        /* sacar las paginas unicas de la secuencia, en orden de aparicion */
        for (i = 0; i < TAM_SECUENCIA; i++)
        {
            pag = t->secuencia[i];
            ya_existe = 0;
            for (j = 0; j < cuantas_unicas; j++)
            {
                if (paginas_unicas[j] == pag)
                {
                    ya_existe = 1;
                    break;
                }
            }
            if (!ya_existe)
            {
                paginas_unicas[cuantas_unicas] = pag;
                cuantas_unicas++;
            }
        }

        /* ordenarlas de menor a mayor (burbuja) */
        for (i = 0; i < cuantas_unicas - 1; i++)
        {
            for (j = i + 1; j < cuantas_unicas; j++)
            {
                if (paginas_unicas[i] > paginas_unicas[j])
                {
                    tmp = paginas_unicas[i];
                    paginas_unicas[i] = paginas_unicas[j];
                    paginas_unicas[j] = tmp;
                }
            }
        }

        printf("  %s genera procesos: ", t->nombre);
        for (i = 0; i < cuantas_unicas; i++)
        {
            nuevo = &procesos[total_procesos];

            sprintf(nuevo->nombre, "%sP%d", t->nombre, paginas_unicas[i]);
            nuevo->tarea_origen = indice_tarea;
            nuevo->tiempo_llegada = tiempo_llegada;
            tiempo_llegada += rand() % 3;

            nuevo->ciclos_cpu = (rand() % (MAX_CICLOS - MIN_CICLOS + 1)) + MIN_CICLOS;
            nuevo->ciclos_restantes = nuevo->ciclos_cpu;
            nuevo->estado = ST_NUEVO;
            nuevo->es_ES = rand() % 2;
            nuevo->tiempo_inicio = -1;
            nuevo->tiempo_fin = 0;
            nuevo->tiempo_retorno = 0;

            printf("%s ", nuevo->nombre);
            total_procesos++;
        }
        printf("\n");

        t = t->sig;
        indice_tarea++;
    }
    printf("\n  >> Total de procesos generados: %d\n", total_procesos);
}

/* =====================================================================
   10. IMPRESION DEL PCB
   ===================================================================== */
void imprimir_PCB(const char *titulo)
{
    int i, t_ini;
    float suma_retorno;
    struct PCB *p;

    suma_retorno = 0;

    printf("\n");
    linea('=', 95);
    printf("  BLOQUE DE CONTROL DE PROCESOS (PCB) -- %s\n", titulo);
    linea('=', 95);
    printf("  %-6s %-6s %-6s %-6s %-10s %-4s %-6s %-6s %-8s\n",
           "Proc", "Lleg", "Cicl", "Rest", "Estado", "E/S", "T.Ini", "T.Fin", "Retorno");
    linea('-', 95);

    for (i = 0; i < total_procesos; i++)
    {
        p = &procesos[i];
        t_ini = (p->tiempo_inicio == -1) ? 0 : p->tiempo_inicio;
        printf("  %-6s %-6d %-6d %-6d %-10d %-4s %-6d %-6d %-8d\n",
               p->nombre, p->tiempo_llegada, p->ciclos_cpu, p->ciclos_restantes,
               p->estado, p->es_ES ? "si" : "no", t_ini, p->tiempo_fin, p->tiempo_retorno);
        suma_retorno += p->tiempo_retorno;
    }
    linea('-', 95);

    if (strcmp(titulo, "ESTADO FINAL") == 0)
    {
        printf("  Quantum usado: %d ms\n", QUANTUM);
        printf("  Tiempo de retorno promedio: %.2f ms\n", suma_retorno / total_procesos);
    }
}

/* =====================================================================
   11. SIMULACION FIFO DE REEMPLAZO DE PAGINAS
   ===================================================================== */
void simular_FIFO(int indice_tarea)
{
    struct nodo_JT *tarea;
    struct nodo_PMT *p;
    int i, j;
    int marcos[MARCOS_POR_TAREA];
    int cuantos_marcos;
    int pagina_en_cola[MARCOS_POR_TAREA];
    int ocupados, puntero;
    int pag, hit;

    tarea = jt_inicio;
    for (i = 0; i < indice_tarea && tarea != NULL; i++)
    {
        tarea = tarea->sig;
    }
    if (tarea == NULL)
        return;

    /* recuperar los marcos fisicos que ya tiene asignados esta tarea */
    cuantos_marcos = 0;
    p = pmt_de_tarea[indice_tarea];
    while (p != NULL && cuantos_marcos < MARCOS_POR_TAREA)
    {
        if (p->marco_fisico != -1)
        {
            marcos[cuantos_marcos] = p->marco_fisico;
            cuantos_marcos++;
        }
        p = p->sig;
    }
    if (cuantos_marcos < MARCOS_POR_TAREA)
        return;

    /* cola FIFO de que paginas estan actualmente en esos marcos */
    ocupados = 0;
    puntero = 0;
    for (j = 0; j < MARCOS_POR_TAREA; j++)
    {
        pagina_en_cola[j] = -1;
    }

    for (i = 0; i < TAM_SECUENCIA; i++)
    {
        pag = tarea->secuencia[i];
        hit = 0;
        for (j = 0; j < MARCOS_POR_TAREA; j++)
        {
            if (pagina_en_cola[j] == pag)
            {
                hit = 1;
                break;
            }
        }
        if (!hit)
        {
            if (ocupados < MARCOS_POR_TAREA)
            {
                pagina_en_cola[ocupados] = pag;
                ocupados++;
            }
            else
            {
                pagina_en_cola[puntero] = pag;
                puntero = (puntero + 1) % MARCOS_POR_TAREA;
            }
        }
    }

    /* limpiar la PMT y volver a marcar solo las paginas que quedaron en la cola FIFO */
    p = pmt_de_tarea[indice_tarea];
    while (p != NULL)
    {
        p->marco_fisico = -1;
        p->presente = 0;
        p->bit_referencia = 0;
        p->bit_modificacion = 0;
        p = p->sig;
    }
    for (j = 0; j < MARCOS_POR_TAREA; j++)
    {
        if (pagina_en_cola[j] == -1)
            continue;
        p = pmt_de_tarea[indice_tarea];
        while (p != NULL)
        {
            if (p->pagina == pagina_en_cola[j])
            {
                p->marco_fisico = marcos[j];
                p->presente = 1;
                p->bit_referencia = 1;
                p->bit_modificacion = rand() % 2;
            }
            p = p->sig;
        }
    }
}

/* =====================================================================
   12. FASE 3: PLANIFICADOR DE CPU - ROUND ROBIN
   ===================================================================== */
void ejecutar_round_robin(void)
{
    int reloj, terminados, ronda;
    int alguien_corrio, desbloqueados, i, rebanada;
    struct PCB *p;

    reloj = 0;
    terminados = 0;
    ronda = 1;

    printf("\n");
    linea('=', 80);
    printf("  5. EJECUCION ROUND ROBIN (QUANTUM = %d ms)\n", QUANTUM);
    linea('=', 80);

    while (terminados < total_procesos)
    {
        alguien_corrio = 0;

        printf("\n  -- Ronda %d --\n", ronda);
        ronda++;

        for (i = 0; i < total_procesos; i++)
        {
            p = &procesos[i];
            if (reloj < p->tiempo_llegada)
                continue;
            if (p->estado != ST_NUEVO && p->estado != ST_LISTO)
                continue;

            if (p->estado == ST_NUEVO)
                p->tiempo_inicio = reloj;
            p->estado = ST_EJECUTANDO;

            rebanada = (p->ciclos_restantes >= QUANTUM) ? QUANTUM : p->ciclos_restantes;
            printf("  [t=%03d] %s corre %d ms (restaban %d)\n",
                   reloj, p->nombre, rebanada, p->ciclos_restantes);

            reloj += rebanada;
            p->ciclos_restantes -= rebanada;

            simular_FIFO(p->tarea_origen);

            if (p->ciclos_restantes <= 0)
            {
                p->estado = ST_TERMINADO;
                p->tiempo_fin = reloj;
                p->tiempo_retorno = p->tiempo_fin - p->tiempo_llegada;
                terminados++;
                printf("        -> %s TERMINO\n", p->nombre);
            }
            else if (p->es_ES)
            {
                p->estado = ST_BLOQUEADO;
                printf("        -> %s pasa a BLOQUEADO (E/S)\n", p->nombre);
            }
            else
            {
                p->estado = ST_LISTO;
                printf("        -> %s vuelve a LISTO (fin de quantum)\n", p->nombre);
            }
            alguien_corrio = 1;
        }

        desbloqueados = 0;
        for (i = 0; i < total_procesos; i++)
        {
            if (procesos[i].estado == ST_BLOQUEADO)
            {
                procesos[i].estado = ST_LISTO;
                desbloqueados++;
            }
        }
        if (desbloqueados > 0)
        {
            printf("  [!] %d proceso(s) terminaron su E/S y vuelven a LISTO\n", desbloqueados);
        }

        if (!alguien_corrio && terminados < total_procesos)
        {
            reloj++;
        }
    }
}

/* =====================================================================
   13. MAIN
   ===================================================================== */
int main(void)
{
    int marcos_libres;

    srand((unsigned)time(NULL));

    /* --- Fase 1: memoria --- */
    calcular_memoria();
    crear_MMT();
    crear_JT_y_PMTs();

    marcos_libres = marcos_totales - marcos_de_SO;
    asignar_marcos_iniciales(&marcos_libres);

    imprimir_MMT();
    imprimir_JT();
    imprimir_PMTs();

    /* --- Fase 2: generar procesos a partir de las tareas --- */
    generar_procesos_desde_tareas();
    imprimir_PCB("ESTADO INICIAL");

    /* --- Fase 3: ejecutar Round Robin (usa FIFO internamente) --- */
    ejecutar_round_robin();
    imprimir_PCB("ESTADO FINAL");

    printf("\n\nPresiona una tecla para salir...");
    getch();

    return 0;
}