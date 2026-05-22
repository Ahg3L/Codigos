#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define SO_RESERVA 20 // 20 KB reservados para el S.O.
#define NUM_TAREAS 20
#define TAM_MARCO 10 // 10 KB por marco

// --------------------------------------------------
// Estructura para el Registro de la PMT
// --------------------------------------------------
struct registro_PMT
{
    int pagina;
    int marco;
};

// --------------------------------------------------
// Estructura Tabla Tareas (Job Table - JT)
// --------------------------------------------------
struct nodo_JT
{
    int ntarea;
    int tamaniot;
    char estado[10]; // "..", "Asig.", "Espera", "Term."
    int cant_paginas;
    struct registro_PMT *tabla_pmt; // Arreglo de estructuras dinámico
    struct nodo_JT *sig;
};

// --------------------------------------------------
// Estructura Tabla Memoria (Memory Header Table - MHT)
// --------------------------------------------------
struct nodo_MT
{
    int nmarco;
    int loc_ini;
    int loc_fin;
    int estado;   // 0 = Libre, 1 = Ocupado
    int id_tarea; // ID de la tarea (-1 si es S.O., 0 si está Libre)
    int n_pagina; // Qué página específica está alojada aquí
    int fi;       // Fragmentación Interna en este marco específico
    struct nodo_MT *sig;
};

// Prototipos de funciones
struct nodo_MT *crea_MMT(int MEMORIA_TOTAL);
struct nodo_JT *crear_JT(int MEMORIA_TOTAL);
void imprime_JT(struct nodo_JT *);
void imprime_MHT(struct nodo_MT *);
void imprime_PMTs(struct nodo_JT *);
void asignar_paginacion(struct nodo_MT *, struct nodo_JT *);
void liberar_tarea(struct nodo_MT *, struct nodo_JT *, int);
int validar_tamano(char[], int, char[]);

// --------------------------------------------------
// MAIN
// --------------------------------------------------
int main()
{

    struct nodo_MT *lista_memoria;
    struct nodo_JT *lista_tareas;

    int ciclo = 1;
    int opcion_liberar;

    char entrada[50];
    int valor;
    char unidad[3];

    int MEMORIA_TOTAL = validar_tamano(entrada, valor, unidad);

    lista_memoria = crea_MMT(MEMORIA_TOTAL);
    lista_tareas = crear_JT(MEMORIA_TOTAL);

    while (1)
    {
        printf("\n\n*******************************************************\n");
        printf("                       CICLO %d\n", ciclo);
        printf("*******************************************************\n");

        // 1. Lógica de asignación por paginación
        asignar_paginacion(lista_memoria, lista_tareas);

        // 2. Impresión de las 3 tablas con el formato exacto de tu libreta
        imprime_JT(lista_tareas);
        imprime_MHT(lista_memoria);
        imprime_PMTs(lista_tareas);

        // Validar si ya terminaron todas las tareas para romper el ciclo
        int todas_terminadas = 1;
        struct nodo_JT *aux_t = lista_tareas;
        while (aux_t != NULL)
        {
            if (strcmp(aux_t->estado, "Term.") != 0)
            {
                todas_terminadas = 0;
                break;
            }
            aux_t = aux_t->sig;
        }

        if (todas_terminadas)
        {
            printf("\n[!] SIMULACION TERMINADA: Todas las tareas han sido procesadas.\n");
            break;
        }

        // 3. Interacción dinámica por ciclo
        printf("\nAccion del ciclo:\n");
        printf(" -> Ingresa el numero de la Tarea a finalizar (ej. 3 para J3)\n");
        printf(" -> Ingresa '0' para avanzar al siguiente ciclo: ");
        scanf("%d", &opcion_liberar);

        if (opcion_liberar > 0)
        {
            liberar_tarea(lista_memoria, lista_tareas, opcion_liberar);
        }

        ciclo++;
    }

    // Liberación de memoria dinámica
    struct nodo_JT *aux_j = lista_tareas, *sig_j;
    while (aux_j != NULL)
    {
        sig_j = aux_j->sig;
        free(aux_j->tabla_pmt);
        free(aux_j);
        aux_j = sig_j;
    }
    struct nodo_MT *aux_m = lista_memoria, *sig_m;
    while (aux_m != NULL)
    {
        sig_m = aux_m->sig;
        free(aux_m);
        aux_m = sig_m;
    }

    return 0;
}

// --------------------------------------------------
// CREAR TABLA DE MEMORIA (MHT)
// --------------------------------------------------
struct nodo_MT *crea_MMT(int MEMORIA_TOTAL)
{
    struct nodo_MT *inicio = NULL, *actual = NULL, *nuevo;
    int num_marcos = MEMORIA_TOTAL / TAM_MARCO;

    for (int i = 0; i < num_marcos; i++)
    {
        nuevo = (struct nodo_MT *)malloc(sizeof(struct nodo_MT));
        nuevo->nmarco = i;
        nuevo->loc_ini = i * TAM_MARCO;
        nuevo->loc_fin = ((i + 1) * TAM_MARCO) - 1;
        nuevo->fi = 0;
        nuevo->sig = NULL;

        // Cargar el S.O. en los primeros marcos
        if (nuevo->loc_ini < SO_RESERVA)
        {
            nuevo->estado = 1;
            nuevo->id_tarea = -1;
            nuevo->n_pagina = -1;
        }
        else
        {
            nuevo->estado = 0;
            nuevo->id_tarea = 0;
            nuevo->n_pagina = -1;
        }

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

// --------------------------------------------------
// CREAR TABLA DE TAREAS (JT) ALEATORIAS
// --------------------------------------------------
struct nodo_JT *crear_JT(int MEMORIA_TOTAL)
{
    struct nodo_JT *inicio = NULL, *actual = NULL, *nuevo;
    srand(time(NULL));

    for (int i = 1; i <= NUM_TAREAS; i++)
    {
        nuevo = (struct nodo_JT *)malloc(sizeof(struct nodo_JT));
        nuevo->ntarea = i;
        nuevo->tamaniot = (rand() % 41) + 5; // Rango [5 - 45] KB
        strcpy(nuevo->estado, "..");

        // Cálculo de páginas necesarias (Techo de la división)
        nuevo->cant_paginas = (nuevo->tamaniot + TAM_MARCO - 1) / TAM_MARCO;

        // Instanciar el Arreglo de Estructuras dinámico para la PMT
        // Debería quedar así:
        nuevo->tabla_pmt = (struct registro_PMT *)malloc(nuevo->cant_paginas * sizeof(struct registro_PMT));

        for (int j = 0; j < nuevo->cant_paginas; j++)
        {
            nuevo->tabla_pmt[j].pagina = j;
            nuevo->tabla_pmt[j].marco = -1;
        }
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

// --------------------------------------------------
// IMPRIMIR JT
// --------------------------------------------------
void imprime_JT(struct nodo_JT *cabezera)
{
    struct nodo_JT *aux = cabezera;

    printf("\n==================================================\n");
    printf("               TABLA DE TAREAS (JT)               \n");
    printf("==================================================\n");
    printf("-> Rango de tamanios configurado: [5 - 45] KB\n\n");
    printf("Tarea     Tamanio(KB)     No. Paginas     Estado\n");
    printf("--------------------------------------------------\n");

    while (aux != NULL)
    {
        printf("J%-8d %-15d %-15d %-s\n",
               aux->ntarea, aux->tamaniot, aux->cant_paginas, aux->estado);
        aux = aux->sig;
    }
}

// --------------------------------------------------
// IMPRIMIR MHT (Con cálculo de FI por Marco)
// --------------------------------------------------
void imprime_MHT(struct nodo_MT *cabezera)
{
    struct nodo_MT *aux = cabezera;

    printf("\n========================================================================\n");
    printf("                         TABLA DE MEMORIA (MHT)                         \n");
    printf("========================================================================\n");
    printf("Marco   Tam(KB)   Loc.Ini   Loc.Fin   Estado   Tarea Asig.   No.Pag   F.I.\n");
    printf("------------------------------------------------------------------------\n");

    while (aux != NULL)
    {
        printf("M%-6d %-9d %-9d %-9d %-8d ",
               aux->nmarco, TAM_MARCO, aux->loc_ini, aux->loc_fin, aux->estado);

        if (aux->id_tarea == -1)
        {
            printf("%-13s %-8s %-4d\n", "[S.O.]", "-", 0);
        }
        else if (aux->estado == 1)
        {
            char id_txt[10];
            sprintf(id_txt, "J%d", aux->id_tarea);
            printf("%-13s P%-7d %-4d KB\n", id_txt, aux->n_pagina, aux->fi);
        }
        else
        {
            printf("%-13s %-8s %-4d\n", "[Libre]", "-", 0);
        }
        aux = aux->sig;
    }
}

// --------------------------------------------------
// IMPRIMIR PMTs
// --------------------------------------------------
void imprime_PMTs(struct nodo_JT *tareas)
{
    struct nodo_JT *aux_t = tareas;
    int registradas = 0;

    printf("\n==================================================\n");
    printf("         TABLAS DE MAPEO DE PAGINAS (PMT)         \n");
    printf("==================================================\n");

    while (aux_t != NULL)
    {
        if (strcmp(aux_t->estado, "Asig.") == 0)
        {
            registradas = 1;
            printf("\n[ PMT - Tarea J%d ]\n", aux_t->ntarea);
            printf("No. Pagina   ->   No. Marco\n");
            printf("---------------------------\n");
            for (int i = 0; i < aux_t->cant_paginas; i++)
            {
                printf("    P%-12d M%d\n", aux_t->tabla_pmt[i].pagina, aux_t->tabla_pmt[i].marco);
            }
        }
        aux_t = aux_t->sig;
    }
    if (!registradas)
    {
        printf("(No hay mapas PMT activos en este ciclo)\n");
    }
}

// --------------------------------------------------
// ASIGNACIÓN NO CONTIGUA (Paginación)
// --------------------------------------------------
void asignar_paginacion(struct nodo_MT *memoria, struct nodo_JT *tareas)
{
    struct nodo_JT *tarea = tareas;

    while (tarea != NULL)
    {
        if (strcmp(tarea->estado, "..") == 0 || strcmp(tarea->estado, "Espera") == 0)
        {

            // 1. Contar marcos libres totales
            int marcos_libres = 0;
            struct nodo_MT *aux_m = memoria;
            while (aux_m != NULL)
            {
                if (aux_m->estado == 0)
                    marcos_libres++;
                aux_m = aux_m->sig;
            }

            // 2. Si hay suficiente espacio disperso, se asigna
            if (marcos_libres >= tarea->cant_paginas)
            {
                int pag_actual = 0;
                struct nodo_MT *m = memoria;

                // Calcular la fragmentación interna total que sufrirá su ÚLTIMA página
                int fi_total = (tarea->cant_paginas * TAM_MARCO) - tarea->tamaniot;

                while (m != NULL && pag_actual < tarea->cant_paginas)
                {
                    if (m->estado == 0)
                    {
                        m->estado = 1;
                        m->id_tarea = tarea->ntarea;
                        m->n_pagina = pag_actual;

                        // Si es la última página de la tarea, le asignamos el residuo de F.I.
                        if (pag_actual == tarea->cant_paginas - 1)
                        {
                            m->fi = fi_total;
                        }
                        else
                        {
                            m->fi = 0;
                        }

                        // Guardar la dirección en el arreglo de estructuras de la PMT
                        tarea->tabla_pmt[pag_actual].marco = m->nmarco;
                        pag_actual++;
                    }
                    m = m->sig;
                }
                strcpy(tarea->estado, "Asig.");
                printf("-> EVENTO: Tarea J%d cargada en RAM (%d paginas asignadas).\n", tarea->ntarea, tarea->cant_paginas);
            }
            else
            {
                strcpy(tarea->estado, "Espera");
            }
        }
        tarea = tarea->sig;
    }
}

// --------------------------------------------------
// LIBERAR TAREA
// --------------------------------------------------
void liberar_tarea(struct nodo_MT *memoria, struct nodo_JT *tareas, int id_buscar)
{
    struct nodo_JT *t = tareas;
    int encontrada = 0;

    while (t != NULL)
    {
        if (t->ntarea == id_buscar && strcmp(t->estado, "Asig.") == 0)
        {
            strcpy(t->estado, "Term.");
            encontrada = 1;
            break;
        }
        t = t->sig;
    }

    if (encontrada)
    {
        struct nodo_MT *m = memoria;
        while (m != NULL)
        {
            if (m->id_tarea == id_buscar)
            {
                m->estado = 0;
                m->id_tarea = 0;
                m->n_pagina = -1;
                m->fi = 0; // Se limpia la F.I. al vaciar el marco
            }
            m = m->sig;
        }
        printf("\n[!] EVENTO: Tarea J%d finalizada con exito. Marcos liberados.\n", id_buscar);
    }
    else
    {
        printf("\n[!] ERROR: La Tarea J%d no se encuentra ejecutandose en la memoria RAM.\n", id_buscar);
    }
}

int validar_tamano(char entrada[], int valor, char unidad[])
{
    int MEMORIA_TOTAL = 0;

    while (1)
    {
        printf("Ingrese el tamano de la MEMORIA TOTAL\n");
        printf("Sintaxis valida: 200KB, 17 MB, 512kb, etc.\n");
        printf("Entrada: ");

        fgets(entrada, 50, stdin);

        // Eliminar salto de linea
        entrada[strcspn(entrada, "\n")] = '\0';

        // Leer numero + unidad
        if (sscanf(entrada, "%d %2s", &valor, unidad) == 2)
        {
            // Convertir unidad a mayusculas
            unidad[0] = toupper(unidad[0]);
            unidad[1] = toupper(unidad[1]);

            // KB -> BYTES
            if (strcmp(unidad, "KB") == 0)
            {
                MEMORIA_TOTAL = valor;
                break;
            }

            // MB -> BYTES
            else if (strcmp(unidad, "MB") == 0)
            {
                MEMORIA_TOTAL = valor * 1024;
                break;
            }
        }

        printf("\n[!] ERROR: Entrada invalida.\n");
        printf("Use formatos como:\n");
        printf("200KB\n");
        printf("17 MB\n");
        printf("512kb\n\n");
    }

    printf("\nMemoria total en Bytes: %d Bytes\n", MEMORIA_TOTAL * 1024);

    // Validar que la memoria sea mayor a la reserva del S.O.
    if (MEMORIA_TOTAL < SO_RESERVA)
    {
        printf("[!] ERROR: La memoria total no puede ser menor que la reserva del S.O. (%d Bytes).\n", SO_RESERVA);
        return -1;
    }

    return MEMORIA_TOTAL;
}