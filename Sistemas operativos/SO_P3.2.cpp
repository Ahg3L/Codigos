#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>

#define MEMORIA_TOTAL 3 // kB (1.5 MB)
#define SO 2            // kB reservados para SO
#define NUM_TAREAS 5
#define TAM_MARCO 64 // KB
#define SECUENCIA 8

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
    int estado; // 1 presente, 0 ausente
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

// --- Inicialización y Creación de Estructuras ---
struct nodo_MT *crea_MT(void);
struct nodo_JT *crea_JT();
struct nodo_TMP *crear_nodo_TMP(int idTarea, int pagina, int marco);

// --- Funciones de Inserción (Para gestionar las listas dinámicas) ---
void insertar_tarea(struct nodo_JT **cabeza, struct nodo_JT *nuevo);
void insertar_tmp(struct nodo_TMP **cabeza, struct nodo_TMP *nuevo);

// --- Funciones de Impresión ---
void imprime_MT(struct nodo_MT *cabezera);
void imprimir_JT(struct nodo_JT *cabeza);
void imprimir_TMP(struct nodo_TMP *cabeza);

// --- Lógica del Sistema ---
void validar_tamano();
void asignar_tareas(struct nodo_MT *memoria, struct nodo_JT *tareas, struct nodo_TMP **lista_tmp);
int ejecutar_reemplazo_FIFO(struct nodo_TMP **lista_tmp, int idTarea, int pagina, int marcoElegido);

// --- Limpieza de Memoria (Opcional, pero recomendado) ---
void liberar_lista_JT(struct nodo_JT *cabeza);
void liberar_lista_TMP(struct nodo_TMP *cabeza);

int main()
{

    struct nodo_MT *lista_memoria = NULL;
    struct nodo_JT *lista_tareas = NULL;
    struct nodo_TMP *lista_tmp = NULL;

    validar_tamano();

    lista_memoria = crea_MT();
    lista_tareas = crea_JT();

    printf("\nIniciando asignacion de tareas...\n");
    asignar_tareas(lista_memoria, lista_tareas, &lista_tmp);

    // 5. Impresión de resultados
    imprime_MT(lista_memoria);
    imprimir_JT(lista_tareas);
    imprimir_TMP(lista_tmp);

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

    printf("Marcos Totales = %d / %d\n", m.memoria_kb, TAM_MARCO);
    printf("Marcos Totales = %d\n", m.marcos_totales);
    printf("\nMarcos reservados para SO = %d\n", SO);

    m.marcos_disponibles = m.marcos_totales - SO;

    printf("Marcos disponibles = %d\n", m.marcos_disponibles);
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
            nuevo->Asignacion = 0;
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
            printf("| %-5d | %-7d | 0      |\n",
                   aux->nmarco,
                   aux->locini);
        }
        else
        {
            printf("| %-5d | %-7d | J%-9d |\n",
                   aux->nmarco,
                   aux->locini,
                   aux->Asignacion);
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

        // ID
        nuevo->id = i + 1;
        nuevo->lineas = rand() % 900 + 100;
        nuevo->loc_pmt = (rand() % 46 + 2) * 64;
        nuevo->tam_secuencia = SECUENCIA;
        nuevo->arrsecu = (int *)malloc(sizeof(int) * SECUENCIA);

        // Generar 8 numeros aleatorios entre 0 y 9 para la secuencia de páginas
        for (int j = 0; j < SECUENCIA; j++)
        {
            nuevo->arrsecu[j] = rand() % 10;
        }

        nuevo->sig = NULL;

        // Insertar en lista enlazada
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

struct nodo_TMP *crear_nodo_TMP(int idTarea, int pagina, int marco)
{
    struct nodo_TMP *nuevo = (struct nodo_TMP *)malloc(sizeof(struct nodo_TMP));
    if (nuevo == NULL)
        return NULL;

    nuevo->idTarea = idTarea;
    nuevo->pagina = pagina;
    nuevo->marco = marco;
    nuevo->estado = 1; // Presente
    nuevo->referencia = 1;
    nuevo->modificacion = 0;
    nuevo->sig = NULL;

    return nuevo;
}

void imprimir_TMP(struct nodo_TMP *cabeza)
{
    struct nodo_TMP *aux;
    int tareaActual = -1;

    printf("\n\n============ TABLA DE MAPA DE PAGINAS ============\n");

    aux = cabeza;
    int indice = 0;
    while (aux != NULL)
    {

        // Cuando cambia de tarea, imprime encabezado
        if (aux->idTarea != tareaActual)
        {

            tareaActual = aux->idTarea;
            indice = 0;
            printf("\nTarea: J%d\n", tareaActual);
            printf("----------------------------------------------------------\n");
            printf("%-8s %-8s %-8s %-8s %-8s\n",
                   "Pagina", "Marco", "Estado", "Refer.", "Modifi.");
            printf("----------------------------------------------------------\n");
        }

        printf("%-8d %-8d %-8d %-8d %-8d\n",
               indice,
               aux->marco,
               aux->estado,
               aux->referencia,
               aux->modificacion);
        indice++;

        aux = aux->sig;
    }

    printf("----------------------------------------------------------\n");
}

void asignar_tareas(struct nodo_MT *mt, struct nodo_JT *lista_tareas, struct nodo_TMP **lista_tmp)
{
    int contadorFIFO = SO;
    struct nodo_JT *tarea_actual = lista_tareas;

    while (tarea_actual != NULL)
    {
        // printf("\nProcesando J%d\n", tarea_actual->id);

        for (int j = 0; j < tarea_actual->tam_secuencia; j++)
        {
            int pagina = tarea_actual->arrsecu[j];
            int encontrada = 0;

            // 1. ¿Está en memoria
            struct nodo_TMP *auxT = *lista_tmp;
            while (auxT != NULL)
            {
                if (auxT->idTarea == tarea_actual->id && auxT->pagina == pagina && auxT->estado == 1)
                {
                    auxT->referencia = 1;
                    // printf("Pagina %d ya cargada en marco %d\n", pagina, auxT->marco);
                    encontrada = 1;
                    break;
                }
                auxT = auxT->sig;
            }

            if (!encontrada)
            {
                // printf("FALLO DE PAGINA en Pag %d\n", pagina);

                // 2. Buscar marco libre en MT
                struct nodo_MT *auxM = mt;
                int marcoLibre = -1;
                while (auxM != NULL)
                {
                    if (auxM->estado == 1 && auxM->Asignacion == 0)
                    {
                        marcoLibre = auxM->nmarco;
                        auxM->Asignacion = tarea_actual->id + 1;
                        auxM->estado = 0;
                        break;
                    }
                    auxM = auxM->sig;
                }

                // 3. Si no hay marcos, forzar reemplazo
                if (marcoLibre == -1)
                {
                    marcoLibre = contadorFIFO;
                    ejecutar_reemplazo_FIFO(lista_tmp, tarea_actual->id, pagina, marcoLibre);
                    contadorFIFO = (contadorFIFO + 1) % 48;
                    if (contadorFIFO < SO)
                        contadorFIFO = SO;
                }
                else
                {
                    // Cargar en marco libre (Insertar nodo nuevo)
                    struct nodo_TMP *nuevo = crear_nodo_TMP(tarea_actual->id, pagina, marcoLibre);
                    if (*lista_tmp == NULL)
                        *lista_tmp = nuevo;
                    else
                    {
                        struct nodo_TMP *temp = *lista_tmp;
                        while (temp->sig != NULL)
                            temp = temp->sig;
                        temp->sig = nuevo;
                    }
                }
            }
        }
        tarea_actual = tarea_actual->sig; // Avanzar en la lista de tareas
    }
}

int ejecutar_reemplazo_FIFO(struct nodo_TMP **cabecera_tmp, int idTarea, int pagina, int marcoElegido)
{
    struct nodo_TMP *aux = *cabecera_tmp;

    // 1. Desactivar la página que ocupa el marco actual
    while (aux != NULL)
    {
        if (aux->marco == marcoElegido && aux->estado == 1)
        {
            // printf("  -> Reemplazando J%d, Pag %d\n", aux->idTarea, aux->pagina);
            aux->estado = 0; // Marcamos como ausente
            break;
        }
        aux = aux->sig;
    }

    // 2. Insertar el nuevo nodo de página en la lista

    struct nodo_TMP *nuevo = crear_nodo_TMP(idTarea, pagina, marcoElegido);

    // Insertar al final (puedes reutilizar una función de inserción)
    if (*cabecera_tmp == NULL)
    {
        *cabecera_tmp = nuevo;
    }
    else
    {
        struct nodo_TMP *temp = *cabecera_tmp;
        while (temp->sig != NULL)
            temp = temp->sig;
        temp->sig = nuevo;
    }
    return 1;
}
