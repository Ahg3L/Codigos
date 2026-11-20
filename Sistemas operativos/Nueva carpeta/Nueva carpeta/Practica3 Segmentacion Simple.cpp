#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

/* ==================================================================
   CONFIGURACION DE VARIABLES
   ================================================================== */
#define MEMORIA_MB           2
#define SO_CAPACIDAD_KB      200
#define NUM_TAREAS           20
#define TAREA_MIN_KB         300
#define TAREA_MAX_KB         800

/* ==================================================================
   ESTRUCTURAS DE DATOS 
   ================================================================== */
// 1. Tabla de Mapa de Memoria (MMT)
struct nodo_MMT {
    int id_bloque;
    int capacidad; 
    int loc_inicio; 
    int estado;      // 1 = Ocupado, 0 = Libre
    struct nodo_MMT *sig;
};

// 2. Tabla de Mapa de Segmentos (SMT)
struct nodo_SMT {
    int segmento;
    int limite; // Tamaño del segmento en KB
    int base;   // Dirección física en memoria
    struct nodo_SMT *sig;
};

// 3. Tabla de Tareas (JT)
struct nodo_JT {
    int ntarea;
    int tamanio;
    int num_segmentos;
    int localidad_smt_rand; // Direccion aleatoria simulada (Entero variado)
    struct nodo_JT *sig;
};

/* ==================================================================
   VARIABLES GLOBALES
   ================================================================== */
struct nodo_MMT *PMMT = NULL, *QMMT = NULL;
struct nodo_JT *PJT = NULL, *QJT = NULL;
struct nodo_SMT *arreglo_SMT[100]; 

int memoria_total_kb = 0;

/* ==================================================================
   FUNCIONES AUXILIARES
   ================================================================== */
void reenumerar_MMT() {
    struct nodo_MMT *aux = PMMT;
    int id = 0;
    while(aux != NULL) {
        aux->id_bloque = id++;
        aux = aux->sig;
    }
}

/* ==================================================================
   LOGICA PRINCIPAL
   ================================================================== */
void realizar_calculos_conversion() {
    printf("======================================================\n");
    printf("   CALCULOS DE CONVERSION INICIALES\n");
    printf("======================================================\n");

    memoria_total_kb = MEMORIA_MB * 1024;
    printf("- Memoria ingresada: %d MB\n", MEMORIA_MB);
    printf("  Conversion: %d MB * 1024 = %d KB\n\n", MEMORIA_MB, memoria_total_kb);
    
    // Mostrar calculos del rango y resta del S.O.
    printf("- S.O. ingresado: %d KB de capacidad\n", SO_CAPACIDAD_KB);
    printf("  Rango del S.O.: 0 KB a %d KB\n\n", SO_CAPACIDAD_KB);
    
    printf("- Memoria disponible para procesos:\n");
    printf("  %d KB (Total) - %d KB (S.O.) = %d KB Libres\n", memoria_total_kb, SO_CAPACIDAD_KB, memoria_total_kb - SO_CAPACIDAD_KB);
}

void generar_tablas() {
    // 1. CREAR MMT (Inicialmente el S.O. y un gran hueco de memoria libre)
    struct nodo_MMT *so_block = (struct nodo_MMT *)malloc(sizeof(struct nodo_MMT));
    so_block->capacidad = SO_CAPACIDAD_KB;
    so_block->loc_inicio = 0;
    so_block->estado = 1; // Ocupado
    so_block->sig = NULL;

    struct nodo_MMT *free_block = (struct nodo_MMT *)malloc(sizeof(struct nodo_MMT));
    free_block->capacidad = memoria_total_kb - so_block->capacidad;
    free_block->loc_inicio = so_block->capacidad;
    free_block->estado = 0; // Libre
    free_block->sig = NULL;

    so_block->sig = free_block;
    PMMT = so_block;
    QMMT = free_block;
    reenumerar_MMT();
    
    // Valores variados para simular direcciones aleatorias dispersas
    int direcciones_base[] = {8540, 100000, 9250, 45210, 7890, 12500, 89600, 15420, 3210, 110200};

    // 2. CREAR JT Y SMTs
    for (int i = 0; i < NUM_TAREAS; i++) {
        // Generar tamaño de tarea entre 300 y 800 KB
        int tamanio_tarea = (rand() % (TAREA_MAX_KB - TAREA_MIN_KB + 1)) + TAREA_MIN_KB;
        
        // Generar de 1 a 5 segmentos de forma aleatoria
        int num_segmentos = (rand() % 5) + 1; 

        struct nodo_JT *nuevoJT = (struct nodo_JT *)malloc(sizeof(struct nodo_JT));
        nuevoJT->ntarea = i + 1;
        nuevoJT->tamanio = tamanio_tarea;
        nuevoJT->num_segmentos = num_segmentos;
        
        // Puntero aleatorio entero, tomando una base dispar y sumando un poco de ruido
        nuevoJT->localidad_smt_rand = direcciones_base[rand() % 10] + (rand() % 500); 
        nuevoJT->sig = NULL;

        if (PJT == NULL) { PJT = nuevoJT; QJT = PJT; } 
        else { QJT->sig = nuevoJT; QJT = nuevoJT; }
        
        arreglo_SMT[i] = NULL;
        struct nodo_SMT *Q_smt = NULL;
        
        // Crear SMT y dividir aleatoriamente la tarea en segmentos
        int tam_restante = tamanio_tarea;
        for (int s = 0; s < num_segmentos; s++) {
            struct nodo_SMT *nuevoSMT = (struct nodo_SMT *)malloc(sizeof(struct nodo_SMT));
            nuevoSMT->segmento = s + 1; // Segmentos 1, 2, 3...
            nuevoSMT->base = -1; // -1 Indica que no se ha asignado a memoria
            nuevoSMT->sig = NULL;
            
            // Asignar límites asegurando que los segmentos sumen el tamaño total
            if (s == num_segmentos - 1) {
                nuevoSMT->limite = tam_restante; 
            } else {
                int max_posible = tam_restante - ((num_segmentos - s - 1) * 10);
                nuevoSMT->limite = (rand() % (max_posible - 10 + 1)) + 10;
                tam_restante -= nuevoSMT->limite;
            }
            
            if (arreglo_SMT[i] == NULL) { arreglo_SMT[i] = nuevoSMT; Q_smt = nuevoSMT; } 
            else { Q_smt->sig = nuevoSMT; Q_smt = nuevoSMT; }
        }
    }
}

// Algoritmo: Validar tamaño total de tarea y asignar segmentos consecutivamente
void procesar_segmentos() {
    struct nodo_JT *aux_jt = PJT;
    
    while (aux_jt != NULL) {
        int index = aux_jt->ntarea - 1;
        struct nodo_SMT *aux_smt = arreglo_SMT[index];
        
        // Buscar el bloque libre actual
        struct nodo_MMT *bloque_libre = PMMT;
        while (bloque_libre != NULL && bloque_libre->estado != 0) {
            bloque_libre = bloque_libre->sig;
        }
        
        // Validamos si la TAREA COMPLETA cabe en el bloque libre
        if (bloque_libre != NULL && bloque_libre->capacidad >= aux_jt->tamanio) {
            
            // Si la tarea completa cabe, asignamos sus segmentos uno por uno
            while (aux_smt != NULL) {
                struct nodo_MMT *nuevo_ocupado = (struct nodo_MMT *)malloc(sizeof(struct nodo_MMT));
                nuevo_ocupado->capacidad = aux_smt->limite;
                nuevo_ocupado->loc_inicio = bloque_libre->loc_inicio;
                nuevo_ocupado->estado = 1;
                
                aux_smt->base = nuevo_ocupado->loc_inicio; // Asignamos la base
                
                bloque_libre->loc_inicio += aux_smt->limite;
                bloque_libre->capacidad -= aux_smt->limite;
                
                struct nodo_MMT *ant = PMMT;
                while (ant->sig != bloque_libre && ant != bloque_libre) {
                    ant = ant->sig;
                }
                
                ant->sig = nuevo_ocupado;
                nuevo_ocupado->sig = bloque_libre;
                
                aux_smt = aux_smt->sig;
            }
        } else {
            // SI LA TAREA COMPLETA NO CABE, detenemos la asignacion.
            break; 
        }
        
        aux_jt = aux_jt->sig;
    }
    
    // Limpieza de bloques de 0KB
    struct nodo_MMT *temp = PMMT;
    struct nodo_MMT *ant = NULL;
    while(temp != NULL) {
        if(temp->estado == 0 && temp->capacidad == 0) { 
            if(ant != NULL) ant->sig = temp->sig;
            struct nodo_MMT *borrar = temp;
            temp = temp->sig;
            free(borrar);
        } else {
            ant = temp;
            temp = temp->sig;
        }
    }
    
    reenumerar_MMT();
}

/* ==================================================================
   FUNCIONES DE IMPRESION ORDENADAS
   ================================================================== */
void imprimir_asignacion_so() {
    printf("\n======================================================\n");
    printf("   ASIGNACION DEL S.O. (INICIAL)\n");
    printf("======================================================\n");
    printf("Bloque\tCapacidad(KB)\tLoc.Inicio(KB)\tEstado\n");
    printf("------------------------------------------------------\n");
    if (PMMT != NULL) {
        printf("%d\t%d\t\t%d\t\t%d\n", PMMT->id_bloque, PMMT->capacidad, PMMT->loc_inicio, PMMT->estado);
    }
}

void imprimir_tareas_simple() {
    printf("\n======================================================\n");
    printf("   LISTA DE TAREAS\n");
    printf("======================================================\n");
    printf("Tarea\tTamano\n");
    printf("------------------------------------------------------\n");
    struct nodo_JT *aux_jt = PJT;
    while (aux_jt != NULL) {
        printf("J%d\t%d KB\n", aux_jt->ntarea, aux_jt->tamanio);
        aux_jt = aux_jt->sig;
    }
}

void imprimir_smts(const char* titulo) {
    printf("\n======================================================\n");
    printf("   %s\n", titulo);
    printf("======================================================\n");
    struct nodo_JT *aux_jt = PJT;
    while (aux_jt != NULL) {
        int index = aux_jt->ntarea - 1;
        printf("\n--- SMT de la Tarea J%d ---\n", aux_jt->ntarea);
        printf("  Segmento\tLimite(KB)\tBase(Loc.Fisica)\n");
        struct nodo_SMT *aux_smt = arreglo_SMT[index];
        while (aux_smt != NULL) {
            if (aux_smt->base == -1) {
                printf("  %d\t\t%d\t\t-\n", aux_smt->segmento, aux_smt->limite);
            } else {
                printf("  %d\t\t%d\t\t%d\n", aux_smt->segmento, aux_smt->limite, aux_smt->base);
            }
            aux_smt = aux_smt->sig;
        }
        aux_jt = aux_jt->sig;
    }
}

void imprimir_mmt() {
    printf("\n======================================================\n");
    printf("   TABLA DE ADMINISTRACION DE MEMORIA (MMT FINAL)\n");
    printf("======================================================\n");
    printf("Bloque\tCapacidad(KB)\tLoc.Inicio(KB)\tEstado\n");
    printf("------------------------------------------------------\n");
    struct nodo_MMT *aux_mmt = PMMT;
    while (aux_mmt != NULL) {
        printf("%d\t%d\t\t%d\t\t%d\n", aux_mmt->id_bloque, aux_mmt->capacidad, aux_mmt->loc_inicio, aux_mmt->estado);
        aux_mmt = aux_mmt->sig;
    }
}

void imprimir_jt_completa() {
    printf("\n======================================================\n");
    printf("   TABLA DE TAREAS (JT COMPLETA)\n");
    printf("======================================================\n");
    printf("Tarea\tTamano(KB)\tSegmentos\tLocalidad_SMT\n");
    printf("------------------------------------------------------\n");
    struct nodo_JT *aux_jt = PJT;
    while (aux_jt != NULL) {
        printf("J%d\t%d\t\t%d\t\t%d\n", aux_jt->ntarea, aux_jt->tamanio, aux_jt->num_segmentos, aux_jt->localidad_smt_rand);
        aux_jt = aux_jt->sig;
    }
}

/* ==================================================================
   MAIN
   ================================================================== */
int main() {
    srand((unsigned)time(NULL));

    // 1. Calculos
    realizar_calculos_conversion();
    
    // 2. Generar estructuras vacias y listas de segmentos
    generar_tablas();
    
    // 3. Imprimir el bloque del S.O.
    imprimir_asignacion_so();
    
    // 4. Imprimir la tabla simple de tareas
    imprimir_tareas_simple();
    
    // 5. Imprimir las SMT RECIEN CREADAS (con base '-')
    imprimir_smts("TABLAS DE MAPA DE SEGMENTOS (SMT) SIN ASIGNAR");
    
    // 6. Ejecutar el algoritmo de asignacion a memoria
    procesar_segmentos();
    
    // 7. Imprimir la MMT despues de asignar
    imprimir_mmt();
    
    // 8. Imprimir la JT completa (con punteros SMT aleatorios y enteros)
    imprimir_jt_completa();
    
    // 9. Imprimir las SMT Finales (algunas con base y las no asignadas con '-')
    imprimir_smts("TABLAS DE MAPA DE SEGMENTOS (SMT) ASIGNADAS A MEMORIA");

    return 0;
}