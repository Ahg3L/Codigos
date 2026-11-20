#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* ==================================================================
   CONFIGURACION DE VARIABLES (Modifica estos valores para el ejercicio)
   ================================================================== */
// MEMORIA
#define MEMORIA_MB           5
#define MEMORIA_KB           0

// MARCO DE PAGINA
#define MARCO_PAGINA_B       0
#define MARCO_PAGINA_KB      10

// SISTEMA OPERATIVO
#define SO_LINEAS            0
#define SO_MARCOS            0
#define SO_CAPACIDAD_KB      20

// TAREAS
#define NUM_TAREAS           5

/* ==================================================================
   ESTRUCTURAS DE DATOS 
   ================================================================== */
// 1. Tabla de Mapa de Memoria (MMT)
struct nodo_MMT {
    int marco;
    float loc_inicio; // Usamos float en caso de tener medios KB (ej. 512 B = 0.5 KB)
    int estado;       // 1 = Ocupado, 0 = Libre
    struct nodo_MMT *sig;
};

// 3. Tabla de Mapa de Paginas (PMT)
struct nodo_PMT {
    int pagina;
    int marco_fisico;
    struct nodo_PMT *sig;
};

// 2. Tabla de Tareas (JT)
struct nodo_JT {
    int ntarea;
    int lineas;
    struct nodo_PMT **localidad_pmt;
    struct nodo_JT *sig;
};

/* ==================================================================
   VARIABLES GLOBALES
   ================================================================== */
struct nodo_MMT *PMMT = NULL, *QMMT = NULL;
struct nodo_JT *PJT = NULL, *QJT = NULL;
struct nodo_PMT *arreglo_PMT[100]; 

float memoria_total_kb = 0;
float cap_marco_kb = 0;
int marcos_so = 0;
int total_marcos = 0;

/* ==================================================================
   FUNCIONES AUXILIARES
   ================================================================== */
int lineas_es_unico(int lineas, int *arreglo_lineas, int limite) {
    for (int i = 0; i < limite; i++) {
        if (arreglo_lineas[i] == lineas) return 0;
    }
    return 1;
}

// Funcion techo manual para floats
int techo(float num) {
    int parte_entera = (int)num;
    if (num > parte_entera) return parte_entera + 1;
    return parte_entera;
}

/* ==================================================================
   LOGICA PRINCIPAL
   ================================================================== */
void realizar_calculos_conversion() {
    printf("======================================================\n");
    printf("   CALCULOS DE CONVERSION\n");
    printf("======================================================\n");

    // 1. Calculo de Memoria
    if (MEMORIA_MB > 0) {
        memoria_total_kb = MEMORIA_MB * 1024.0;
        printf("- Memoria ingresada: %d MB\n  Conversion: %d MB * 1024 = %.2f KB\n", MEMORIA_MB, MEMORIA_MB, memoria_total_kb);
    } else if (MEMORIA_KB > 0) {
        memoria_total_kb = MEMORIA_KB;
        printf("- Memoria ingresada: %d KB (No requiere conversion)\n", MEMORIA_KB);
    }

    // 2. Calculo de Marco de Pagina
    if (MARCO_PAGINA_B > 0) {
        cap_marco_kb = MARCO_PAGINA_B / 1024.0;
        printf("- Marco ingresado: %d B\n  Conversion: %d B / 1024 = %.2f KB\n", MARCO_PAGINA_B, MARCO_PAGINA_B, cap_marco_kb);
    } else if (MARCO_PAGINA_KB > 0) {
        cap_marco_kb = MARCO_PAGINA_KB;
        printf("- Marco ingresado: %d KB (No requiere conversion)\n", MARCO_PAGINA_KB);
    }

    // Calculo total de marcos (Memoria / Capacidad Marco)
    total_marcos = (int)(memoria_total_kb / cap_marco_kb);
    printf("\n>> Total de Marcos en Memoria: %.2f KB / %.2f KB = %d marcos\n", memoria_total_kb, cap_marco_kb, total_marcos);

    // 3. Calculo del S.O.
    if (SO_MARCOS > 0) {
        marcos_so = SO_MARCOS;
        float tam_so_kb = marcos_so * cap_marco_kb;
        printf("\n- S.O. ingresado: %d marcos\n  Conversion a tamano: %d marcos * %.2f KB = %.2f KB\n", SO_MARCOS, SO_MARCOS, cap_marco_kb, tam_so_kb);
    } else if (SO_LINEAS > 0) {
        float tam_so_kb = SO_LINEAS; // Asumiendo 1 linea = 1 KB
        marcos_so = techo(tam_so_kb / cap_marco_kb);
        printf("\n- S.O. ingresado: %d lineas (Asumiendo 1 Linea = 1 KB)\n  Conversion a marcos: %d KB / %.2f KB = %d marcos\n", SO_LINEAS, SO_LINEAS, cap_marco_kb, marcos_so);
    } else if (SO_CAPACIDAD_KB > 0) {
        marcos_so = techo(SO_CAPACIDAD_KB / cap_marco_kb);
        printf("\n- S.O. ingresado: %d KB de capacidad\n  Conversion a marcos: %d KB / %.2f KB = %d marcos\n", SO_CAPACIDAD_KB, SO_CAPACIDAD_KB, cap_marco_kb, marcos_so);
    }
}

void generar_tablas() {
    // 1. CREAR MMT (Mapa de Memoria)
    float loc_actual = 0.0;
    for (int i = 0; i < total_marcos; i++) {
        struct nodo_MMT *nuevo = (struct nodo_MMT *)malloc(sizeof(struct nodo_MMT));
        nuevo->marco = i;
        nuevo->loc_inicio = loc_actual;
        
        // Estado: 1 Ocupado (si es del S.O.), 0 Libre (si esta disponible)
        if (i < marcos_so) {
            nuevo->estado = 1; 
        } else {
            nuevo->estado = 0;
        }
        nuevo->sig = NULL;
        
        if (PMMT == NULL) { PMMT = nuevo; QMMT = PMMT; } 
        else { QMMT->sig = nuevo; QMMT = nuevo; }
        
        loc_actual += cap_marco_kb;
    }
    
    // 2. CREAR JT Y PMTs (Simulacion de tareas)
    int marcos_libres_inicio = marcos_so;
    int *lineas_usadas = (int*)malloc(NUM_TAREAS * sizeof(int));
    
    for (int i = 0; i < NUM_TAREAS; i++) {
        int lineas_rand;
        do {
            lineas_rand = (rand() % 901) + 100; // 100 a 1000
        } while (!lineas_es_unico(lineas_rand, lineas_usadas, i));
        lineas_usadas[i] = lineas_rand;
        
        int pags_req = techo((float)lineas_rand / cap_marco_kb);
        
        struct nodo_JT *nuevoJT = (struct nodo_JT *)malloc(sizeof(struct nodo_JT));
        nuevoJT->ntarea = i + 1;
        nuevoJT->lineas = lineas_rand;
        nuevoJT->localidad_pmt = &arreglo_PMT[i];
        nuevoJT->sig = NULL;
        
        if (PJT == NULL) { PJT = nuevoJT; QJT = PJT; } 
        else { QJT->sig = nuevoJT; QJT = nuevoJT; }
        
        arreglo_PMT[i] = NULL;
        struct nodo_PMT *Q_pmt = NULL;
        
        if (marcos_libres_inicio + pags_req <= total_marcos) {
            for (int p = 0; p < pags_req; p++) {
                struct nodo_PMT *nuevoPMT = (struct nodo_PMT *)malloc(sizeof(struct nodo_PMT));
                nuevoPMT->pagina = p;
                nuevoPMT->marco_fisico = marcos_libres_inicio;
                nuevoPMT->sig = NULL;
                
                if (arreglo_PMT[i] == NULL) { arreglo_PMT[i] = nuevoPMT; Q_pmt = nuevoPMT; } 
                else { Q_pmt->sig = nuevoPMT; Q_pmt = nuevoPMT; }
                
                // Actualizar a estado 1 (Ocupado) en MMT
                struct nodo_MMT *aux_mmt = PMMT;
                while (aux_mmt != NULL) {
                    if (aux_mmt->marco == marcos_libres_inicio) {
                        aux_mmt->estado = 1; 
                        break;
                    }
                    aux_mmt = aux_mmt->sig;
                }
                marcos_libres_inicio++;
            }
        }
    }
    free(lineas_usadas);
}

void imprimir_tablas() {
    printf("\n======================================================\n");
    printf("   MARCOS DEL SISTEMA OPERATIVO\n");
    printf("======================================================\n");
    printf("El S.O. esta ocupando desde el marco 0 hasta el %d\n", marcos_so - 1);

    printf("\n======================================================\n");
    printf("   1. TABLA DE ADMINISTRACION DE MEMORIA (MMT)\n");
    printf("======================================================\n");
    printf("Marco\tLoc. Inicio(KB)\tEstado\n");
    printf("------------------------------------------------------\n");
    struct nodo_MMT *aux_mmt = PMMT;
    while (aux_mmt != NULL) {
        printf("%d\t%.2f\t\t%d\n", aux_mmt->marco, aux_mmt->loc_inicio, aux_mmt->estado);
        aux_mmt = aux_mmt->sig;
    }

    printf("\n======================================================\n");
    printf("   2. TABLA DE TAREAS (JT)\n");
    printf("======================================================\n");
    printf("Tarea\tLineas\tLocalidad PMT\n");
    printf("------------------------------------------------------\n");
    struct nodo_JT *aux_jt = PJT;
    while (aux_jt != NULL) {
        printf("T%d\t%d\t%p\n", aux_jt->ntarea, aux_jt->lineas, (void*)aux_jt->localidad_pmt);
        aux_jt = aux_jt->sig;
    }

    printf("\n======================================================\n");
    printf("   3. TABLAS DE MAPA DE PAGINAS (PMT) POR TAREA\n");
    printf("======================================================\n");
    aux_jt = PJT;
    while (aux_jt != NULL) {
        int index = aux_jt->ntarea - 1;
        printf("\n--- PMT de la Tarea %d ---\n", aux_jt->ntarea);
        if (arreglo_PMT[index] == NULL) {
            printf("  [Sin asignar marcos por falta de memoria]\n");
        } else {
            printf("  Pagina\tMarco Localidad\n");
            struct nodo_PMT *aux_pmt = arreglo_PMT[index];
            while (aux_pmt != NULL) {
                printf("  %d\t\t%d\n", aux_pmt->pagina, aux_pmt->marco_fisico);
                aux_pmt = aux_pmt->sig;
            }
        }
        aux_jt = aux_jt->sig;
    }
}

int main() {
    srand((unsigned)time(NULL));

    realizar_calculos_conversion();
    generar_tablas();
    imprimir_tablas();

    return 0;
}