#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <string.h> // Necesario para sprintf

/* ==================================================================
   CONFIGURACION DE VARIABLES
   ================================================================== */
// MEMORIA
#define MEMORIA_MB           3
#define MEMORIA_KB           0

// MARCO DE PAGINA
#define MARCO_PAGINA_B       0
#define MARCO_PAGINA_KB      64

// SISTEMA OPERATIVO                                            
#define SO_LINEAS            0
#define SO_MARCOS            2
#define SO_CAPACIDAD_KB      0

// TAREAS
#define NUM_TAREAS           20

// PAGINACION POR DEMANDA (ACTUALIZADO)
#define SECUENCIA            8
#define MARCOS_ASIGNADOS     3 // Limite de marcos por tarea

/* ==================================================================
   ESTRUCTURAS DE DATOS 
   ================================================================== */
struct nodo_MMT {
    int marco;
    float loc_inicio; 
    int estado;
    struct nodo_MMT *sig;
};

struct nodo_PMT {
    int pagina;
    int marco_fisico;
    int estado;        
    int referencia;    
    int modificacion;  
    struct nodo_PMT *sig;
};

struct nodo_JT {
    char nombre_clave[10]; // Agregado para sincro
    int ntarea;
    int lineas;
    int secuencia[SECUENCIA];
    struct nodo_PMT **localidad_pmt;
    int localidad_pmt_simulada; // Agregado para evitar Hexadecimal
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

    if (MEMORIA_MB > 0) {
        memoria_total_kb = MEMORIA_MB * 1024.0;
        printf("- Memoria ingresada: %d MB\n  Conversion: %d MB * 1024 = %.2f KB\n", MEMORIA_MB, MEMORIA_MB, memoria_total_kb);
    } else if (MEMORIA_KB > 0) {
        memoria_total_kb = MEMORIA_KB;
        printf("- Memoria ingresada: %d KB (No requiere conversion)\n", MEMORIA_KB);
    }

    if (MARCO_PAGINA_B > 0) {
        cap_marco_kb = MARCO_PAGINA_B / 1024.0;
        printf("- Marco ingresado: %d B\n  Conversion: %d B / 1024 = %.2f KB\n", MARCO_PAGINA_B, MARCO_PAGINA_B, cap_marco_kb);
    } else if (MARCO_PAGINA_KB > 0) {
        cap_marco_kb = MARCO_PAGINA_KB;
        printf("- Marco ingresado: %d KB (No requiere conversion)\n", MARCO_PAGINA_KB);
    }

    total_marcos = (int)(memoria_total_kb / cap_marco_kb);
    printf("\n>> Total de Marcos en Memoria: %.2f KB / %.2f KB = %d marcos\n", memoria_total_kb, cap_marco_kb, total_marcos);

    if (SO_MARCOS > 0) {
        marcos_so = SO_MARCOS;
        float tam_so_kb = marcos_so * cap_marco_kb;
        printf("\n- S.O. ingresado: %d marcos\n  Conversion a tamano: %d marcos * %.2f KB = %.2f KB\n", SO_MARCOS, SO_MARCOS, cap_marco_kb, tam_so_kb);
    } else if (SO_LINEAS > 0) {
        float tam_so_kb = SO_LINEAS; 
        marcos_so = techo(tam_so_kb / cap_marco_kb);
        printf("\n- S.O. ingresado: %d lineas (Asumiendo 1 Linea = 1 KB)\n  Conversion a marcos: %d KB / %.2f KB = %d marcos\n", SO_LINEAS, SO_LINEAS, cap_marco_kb, marcos_so);
    } else if (SO_CAPACIDAD_KB > 0) {
        marcos_so = techo(SO_CAPACIDAD_KB / cap_marco_kb);
        printf("\n- S.O. ingresado: %d KB de capacidad\n  Conversion a marcos: %d KB / %.2f KB = %d marcos\n", SO_CAPACIDAD_KB, SO_CAPACIDAD_KB, cap_marco_kb, marcos_so);
    }
}

void generar_tablas() {
    // 1. CREAR MMT
    float loc_actual = 0.0;
    for (int i = 0; i < total_marcos; i++) {
        struct nodo_MMT *nuevo = (struct nodo_MMT *)malloc(sizeof(struct nodo_MMT));
        nuevo->marco = i;
        nuevo->loc_inicio = loc_actual;
        
        if (i < marcos_so) { nuevo->estado = 1; } 
        else { nuevo->estado = 0; }
        
        nuevo->sig = NULL;
        if (PMMT == NULL) { PMMT = nuevo; QMMT = PMMT; } 
        else { QMMT->sig = nuevo; QMMT = nuevo; }
        loc_actual += cap_marco_kb;
    }
    
    // 2. CREAR JT Y PMTs
    int *lineas_usadas = (int*)malloc(NUM_TAREAS * sizeof(int));
    
    for (int i = 0; i < NUM_TAREAS; i++) {
        int lineas_rand;
        do {
            lineas_rand = (rand() % 801) + 200; // Ajustado a logica original
        } while (!lineas_es_unico(lineas_rand, lineas_usadas, i));
        lineas_usadas[i] = lineas_rand;
        
        int pags_req = techo((float)lineas_rand / cap_marco_kb);
        
        struct nodo_JT *nuevoJT = (struct nodo_JT *)malloc(sizeof(struct nodo_JT));
        sprintf(nuevoJT->nombre_clave, "T%d", i + 1); // Agregado Nombre
        nuevoJT->ntarea = i + 1;
        nuevoJT->lineas = lineas_rand;
        nuevoJT->localidad_pmt = &arreglo_PMT[i];
        nuevoJT->localidad_pmt_simulada = (rand() % 90000) + 8000; // Puntero Simulado
        
        // Generar secuencia logicamente controlada
        int max_pag = (pags_req > 10) ? 10 : pags_req;
        nuevoJT->secuencia[0] = 0; // Obligar a cargar pagina 0 primero
        for(int s = 1; s < SECUENCIA; s++) {
            nuevoJT->secuencia[s] = rand() % max_pag; 
        }

        nuevoJT->sig = NULL;
        if (PJT == NULL) { PJT = nuevoJT; QJT = PJT; } 
        else { QJT->sig = nuevoJT; QJT = nuevoJT; }
        
        arreglo_PMT[i] = NULL;
        struct nodo_PMT *Q_pmt = NULL;
        
        for (int p = 0; p < pags_req; p++) {
            struct nodo_PMT *nuevoPMT = (struct nodo_PMT *)malloc(sizeof(struct nodo_PMT));
            nuevoPMT->pagina = p;
            nuevoPMT->marco_fisico = -1; 
            nuevoPMT->estado = 0;        
            nuevoPMT->referencia = 0;
            nuevoPMT->modificacion = 0;
            nuevoPMT->sig = NULL;
            
            if (arreglo_PMT[i] == NULL) { arreglo_PMT[i] = nuevoPMT; Q_pmt = nuevoPMT; } 
            else { Q_pmt->sig = nuevoPMT; Q_pmt = nuevoPMT; }
        }
    }
    free(lineas_usadas);
}

// Lógica de asignación por demanda ACTUALIZADA (Uso de Puntero para Marcos y Limite de Asignacion)
void procesar_secuencias(int *marcos_libres) {
    int marco_disponible = marcos_so; 
    struct nodo_JT *aux_jt = PJT;
    int index = 0;
    
    while (aux_jt != NULL) {
        int marcos_asignados_tarea = 0; // Control de limite por tarea
        
        for (int s = 0; s < SECUENCIA; s++) {
            int pagina_solicitada = aux_jt->secuencia[s];
            
            struct nodo_PMT *aux_pmt = arreglo_PMT[index];
            while (aux_pmt != NULL && aux_pmt->pagina != pagina_solicitada) {
                aux_pmt = aux_pmt->sig;
            }
            
            if (aux_pmt != NULL) {
                if (aux_pmt->estado == 0 && *marcos_libres > 0 && marcos_asignados_tarea < MARCOS_ASIGNADOS) { 
                    // FALLO DE PÁGINA: Asignar un marco físico (simulado)
                    aux_pmt->marco_fisico = marco_disponible;
                    marco_disponible++;
                    
                    aux_pmt->estado = 1;
                    aux_pmt->referencia = 1;
                    aux_pmt->modificacion = rand() % 2; 
                    
                    (*marcos_libres)--;
                    marcos_asignados_tarea++;
                } else if (aux_pmt->estado == 1) {
                    // ACIERTO: Ya está cargada, solo se referencia
                    aux_pmt->referencia = 1;
                }
            }
        }
        aux_jt = aux_jt->sig;
        index++;
    }
}

void imprimir_tablas() {
    printf("\n======================================================\n");
    printf("   1. TABLA DE ADMINISTRACION DE MEMORIA (MMT)\n");
    printf("======================================================\n");
    printf("Marco\tLoc.Inicio(kb)\tEstado\n");
    printf("------------------------------------------------------\n");
    struct nodo_MMT *aux_mmt = PMMT;
    while (aux_mmt != NULL) {
        printf("%d\t%.2f\t\t%d\n", aux_mmt->marco, aux_mmt->loc_inicio, aux_mmt->estado);
        aux_mmt = aux_mmt->sig;
    }

    printf("\n======================================================\n");
    printf("   2. TABLA DE TAREAS (JT)\n");
    printf("======================================================\n");
    printf("Tarea\tLineas\tLoc_Pmt_Simulada\tSecuencia\n");
    printf("------------------------------------------------------\n");
    struct nodo_JT *aux_jt = PJT;
    while (aux_jt != NULL) {
        // ACTUALIZADO: Impresion de localidad simulada y nombre clave
        printf("%s\t%d\t%d\t\t\t", aux_jt->nombre_clave, aux_jt->lineas, aux_jt->localidad_pmt_simulada);
        for(int s = 0; s < SECUENCIA; s++){
            printf("%d ", aux_jt->secuencia[s]);
        }
        printf("\n");
        aux_jt = aux_jt->sig;
    }

    printf("\n======================================================\n");
    printf("   3. TABLAS DE MAPA DE PAGINAS (PMT) POR TAREA\n");
    printf("======================================================\n");
    aux_jt = PJT;
    int index = 0;
    while (aux_jt != NULL) {
        printf("\n--- PMT de la Tarea %s ---\n", aux_jt->nombre_clave);
        printf("  Pagina\tMarco\tEstado\tReferencia\tModificacion\n");
        struct nodo_PMT *aux_pmt = arreglo_PMT[index];
        while (aux_pmt != NULL) {
            if (aux_pmt->marco_fisico == -1) {
                printf("  %d\t\t-\t%d\t%d\t\t%d\n", 
                       aux_pmt->pagina,  
                       aux_pmt->estado, 
                       aux_pmt->referencia, 
                       aux_pmt->modificacion);
            } else {
                printf("  %d\t\t%d\t%d\t%d\t\t%d\n", 
                       aux_pmt->pagina, 
                       aux_pmt->marco_fisico, 
                       aux_pmt->estado, 
                       aux_pmt->referencia, 
                       aux_pmt->modificacion);
            }
            aux_pmt = aux_pmt->sig;
        }
        aux_jt = aux_jt->sig;
        index++;
    }
}

int main() {
    srand((unsigned)time(NULL));

    realizar_calculos_conversion();
    generar_tablas();
    
    // Paso de marcos iniciales por puntero
    int marcos_libres_iniciales = total_marcos - marcos_so;
    procesar_secuencias(&marcos_libres_iniciales);
    
    imprimir_tablas();

    return 0;
}