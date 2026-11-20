#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <string.h>

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

// TAREAS Y CPU
#define NUM_TAREAS           20
#define MIN_CICLOS           5
#define MAX_CICLOS           10

// PAGINACION POR DEMANDA Y FIFO
#define SECUENCIA            8
#define MARCOS_ASIGNADOS     3 // Marcos destinados para simular el FIFO

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
    char nombre_clave[10]; 
    int lineas;
    int secuencia[SECUENCIA];
    struct nodo_PMT **localidad_pmt;
    int localidad_pmt_simulada; 
    struct nodo_JT *sig;
};

struct nodo_auxiliar {
    int pagina;
};

// BLOQUE DE CONTROL DE PROCESO (PCB)
struct PCB {
    char nombre_clave[10];   
    int tiempo_llegada;      
    int ciclos_cpu;          
    int estado;              // 1=Nuevo/Listo, 2=Listo, 3=Ejecucion, 4=Bloqueado, 5=Terminado
    int memoria_kb;          
    int num_archivos;        
    int tipo_operacion;      
    int num_recursos;        
    int tipo_proceso;        
    
    // Tiempos FCFS
    int tiempo_inicio;
    int tiempo_finalizacion; 
    int tiempo_espera;       
    int tiempo_retorno;      
};

/* ==================================================================
   VARIABLES GLOBALES
   ================================================================== */
struct nodo_MMT *PMMT = NULL, *QMMT = NULL;
struct nodo_JT *PJT = NULL, *QJT = NULL;
struct nodo_PMT *arreglo_PMT[100]; 
struct PCB lista_tareas[NUM_TAREAS]; // Arreglo Global de PCBs

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
   LOGICA PRINCIPAL DE MEMORIA Y TAREAS
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
    float loc_actual = 0.0;
    for (int i = 0; i < total_marcos; i++) {
        struct nodo_MMT *nuevo = (struct nodo_MMT *)malloc(sizeof(struct nodo_MMT));
        nuevo->marco = i;
        nuevo->loc_inicio = loc_actual;
        nuevo->estado = (i < marcos_so) ? 1 : 0;
        nuevo->sig = NULL;
        
        if (PMMT == NULL) { PMMT = nuevo; QMMT = PMMT; } 
        else { QMMT->sig = nuevo; QMMT = nuevo; }
        loc_actual += cap_marco_kb;
    }
    
    int *lineas_usadas = (int*)malloc(NUM_TAREAS * sizeof(int));
    int tiempo_llegada_acumulado = 0;
    
    int current_job = 1;
    int current_process = 0;
    
    for (int i = 0; i < NUM_TAREAS; i++) {
        int lineas_rand;
        do {
            lineas_rand = (rand() % 901) + 100;
        } while (!lineas_es_unico(lineas_rand, lineas_usadas, i));
        lineas_usadas[i] = lineas_rand;
        
        sprintf(lista_tareas[i].nombre_clave, "J%dP%d", current_job, current_process);
        
        lista_tareas[i].tiempo_llegada = tiempo_llegada_acumulado;
        tiempo_llegada_acumulado += rand() % 4; 
        lista_tareas[i].ciclos_cpu = (rand() % (MAX_CICLOS - MIN_CICLOS + 1)) + MIN_CICLOS;
        
        // ESTADO INICIAL 1 (LISTO / NUEVO) PARA TODOS LOS PROCESOS
        lista_tareas[i].estado = 1; 
        
        lista_tareas[i].memoria_kb = lineas_rand; 
        lista_tareas[i].num_archivos = rand() % 15;
        lista_tareas[i].tipo_operacion = rand() % 2;
        lista_tareas[i].num_recursos = rand() % 5 + 1;
        lista_tareas[i].tipo_proceso = rand() % 2;
        lista_tareas[i].tiempo_inicio = 0;
        lista_tareas[i].tiempo_finalizacion = 0;
        lista_tareas[i].tiempo_espera = 0;
        lista_tareas[i].tiempo_retorno = 0;
        
        current_process++;
        if (rand() % 2 == 0) { 
            current_job++;
            current_process = 0;
        }

        int pags_req = techo((float)lineas_rand / cap_marco_kb);
        
        struct nodo_JT *nuevoJT = (struct nodo_JT *)malloc(sizeof(struct nodo_JT));
        strcpy(nuevoJT->nombre_clave, lista_tareas[i].nombre_clave); 
        nuevoJT->lineas = lineas_rand;
        nuevoJT->localidad_pmt = &arreglo_PMT[i];
        nuevoJT->localidad_pmt_simulada = (rand() % 90000) + 8000;
        
        int max_pag = pags_req;
        if (max_pag > 10) max_pag = 10; 

        nuevoJT->secuencia[0] = 0; 
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

void procesar_secuencias(int *marcos_libres) {
    int marco_disponible = marcos_so; 
    struct nodo_JT *aux_jt = PJT;
    int index = 0;
    while (aux_jt != NULL) {
        int marcos_asignados_tarea = 0; 
        for (int s = 0; s < SECUENCIA; s++) {
            int pagina_solicitada = aux_jt->secuencia[s];
            struct nodo_PMT *aux_pmt = arreglo_PMT[index];
            while (aux_pmt != NULL && aux_pmt->pagina != pagina_solicitada) aux_pmt = aux_pmt->sig;
            
            if (aux_pmt != NULL && aux_pmt->estado == 0) { 
                if (*marcos_libres > 0 && marcos_asignados_tarea < MARCOS_ASIGNADOS) {
                    aux_pmt->marco_fisico = marco_disponible++;
                    aux_pmt->estado = 1;
                    aux_pmt->referencia = 0; 
                    aux_pmt->modificacion = 0; 
                    (*marcos_libres)--;
                    marcos_asignados_tarea++;
                }
            } 
        }

        struct nodo_PMT *aux_pmt = arreglo_PMT[index];
        while (aux_pmt != NULL && marcos_asignados_tarea < MARCOS_ASIGNADOS && *marcos_libres > 0) {
            if (aux_pmt->estado == 0) {
                aux_pmt->marco_fisico = marco_disponible++;
                aux_pmt->estado = 1;
                aux_pmt->referencia = 0; 
                aux_pmt->modificacion = 0; 
                (*marcos_libres)--;
                marcos_asignados_tarea++;
            }
            aux_pmt = aux_pmt->sig;
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
    printf("   2. TABLA DE TAREAS Y PROCESOS (JT)\n");
    printf("======================================================\n");
    printf("Proceso\tLineas\tLocalidad_Pmt\tSecuencia\n");
    printf("------------------------------------------------------\n");
    struct nodo_JT *aux_jt = PJT;
    while (aux_jt != NULL) {
        printf("%s\t%d\t%d\t\t", aux_jt->nombre_clave, aux_jt->lineas, aux_jt->localidad_pmt_simulada);
        for(int s = 0; s < SECUENCIA; s++){
            printf("%d ", aux_jt->secuencia[s]);
        }
        printf("\n");
        aux_jt = aux_jt->sig;
    }

    printf("\n======================================================\n");
    printf("   3. TABLAS DE MAPA DE PAGINAS (PMT) POR PROCESO\n");
    printf("======================================================\n");
    aux_jt = PJT;
    int index = 0;
    while (aux_jt != NULL) {
        printf("\n--- PMT del Proceso %s ---\n", aux_jt->nombre_clave);
        printf("  Pagina\tMarco\tEstado\tReferencia\tModificacion\n");
        struct nodo_PMT *aux_pmt = arreglo_PMT[index];
        while (aux_pmt != NULL) {
            if (aux_pmt->marco_fisico == -1) {
                printf("  %d\t\t-\t%d\t%d\t\t%d\n", aux_pmt->pagina, aux_pmt->estado, aux_pmt->referencia, aux_pmt->modificacion);
            } else {
                printf("  %d\t\t%d\t%d\t%d\t\t%d\n", aux_pmt->pagina, aux_pmt->marco_fisico, aux_pmt->estado, aux_pmt->referencia, aux_pmt->modificacion);
            }
            aux_pmt = aux_pmt->sig;
        }
        aux_jt = aux_jt->sig;
        index++;
    }
}

// ==================================================================
// SIMULACION FIFO 
// ==================================================================
void simular_FIFO(int id_array) {
    struct nodo_JT *tarea = PJT;
    int current_index = 0;
    while(tarea != NULL && current_index != id_array) {
        tarea = tarea->sig;
        current_index++;
    }
    if(tarea == NULL) return;

    int marcos_fisicos[MARCOS_ASIGNADOS];
    int idx = 0;
    struct nodo_PMT *p_temp = arreglo_PMT[id_array];
    while(p_temp != NULL && idx < MARCOS_ASIGNADOS) {
        if(p_temp->marco_fisico != -1) {
            marcos_fisicos[idx] = p_temp->marco_fisico;
            idx++;
        }
        p_temp = p_temp->sig;
    }

    if (idx < MARCOS_ASIGNADOS) return;

    printf("\n======================================================\n");
    printf("   ALGORITMO DE REEMPLAZO FIFO - PROCESO %s\n", tarea->nombre_clave);
    printf("======================================================\n");
    printf("Tiempo\tSecuencia\tLista Auxiliar\tFallo\tReemplazo\n");
    printf("------------------------------------------------------\n");

    struct nodo_auxiliar lista_aux[MARCOS_ASIGNADOS]; 
    for (int j = 0; j < MARCOS_ASIGNADOS; j++) lista_aux[j].pagina = -1; 

    int puntero_fifo = 0, total_fallos = 0, marcos_ocupados = 0;
    char notificaciones[SECUENCIA][150];
    int cont_notif = 0;

    for (int i = 0; i < SECUENCIA; i++) {
        int pagina_actual = tarea->secuencia[i];
        int hit = 0, hubo_fallo = 0, reemplazo_old = -1;

        for (int j = 0; j < MARCOS_ASIGNADOS; j++) {
            if (lista_aux[j].pagina == pagina_actual) { hit = 1; break; }
        }

        if (!hit) {
            hubo_fallo = 1;
            total_fallos++;
            if (marcos_ocupados < MARCOS_ASIGNADOS) {
                lista_aux[marcos_ocupados].pagina = pagina_actual;
                marcos_ocupados++;
            } else {
                reemplazo_old = lista_aux[puntero_fifo].pagina;
                lista_aux[puntero_fifo].pagina = pagina_actual;
                puntero_fifo = (puntero_fifo + 1) % MARCOS_ASIGNADOS;
                sprintf(notificaciones[cont_notif], "En el Tiempo %d se llamo a la funcion de reemplazo: Salio P%d, Entro P%d.", i + 1, reemplazo_old, pagina_actual);
                cont_notif++;
            }
        }

        char str_aux[3][5];
        for(int j=0; j<3; j++) {
            if(lista_aux[j].pagina == -1) sprintf(str_aux[j], " -");
            else sprintf(str_aux[j], "%2d", lista_aux[j].pagina);
        }

        printf("  %d\t   P%d\t\t[%s, %s, %s]\t  %d\t", i + 1, pagina_actual, str_aux[0], str_aux[1], str_aux[2], hubo_fallo);
        if (reemplazo_old != -1) printf("P%d -> P%d\n", reemplazo_old, pagina_actual);
        else printf("  -\n");
    }
    printf("------------------------------------------------------\n");
}

/* ==================================================================
   IMPRESION DINAMICA DE METRICAS DEL PCB 
   ================================================================== */
void imprimir_metricas_FCFS(const char* titulo) {
    float suma_espera = 0, suma_retorno = 0;
    
    printf("\n=================================================================================================================================\n");
    printf("                                      BLOQUE DE CONTROL DE PROCESOS (PCB) - %s                                     \n", titulo);
    printf("=================================================================================================================================\n");
    printf(" Proceso | T.llegada | Ciclos | Estado | Memoria | CPU/E/S | Num_Disp | No.Arch |  Tipo   | T.Ini | T.Fin | Espera | Retorno |\n");
    printf("---------------------------------------------------------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < NUM_TAREAS; i++) {
        printf("  %-6s |    %02d     |   %02d   |    %d   | %04d KB |   %-3s   |    %02d    |   %02d    | %-7s |  %03d  |  %03d  |  %03d   |   %03d   |\n",
               lista_tareas[i].nombre_clave, lista_tareas[i].tiempo_llegada, lista_tareas[i].ciclos_cpu,
               lista_tareas[i].estado, lista_tareas[i].memoria_kb, lista_tareas[i].tipo_operacion == 0 ? "CPU" : "E/S",
               lista_tareas[i].num_recursos, lista_tareas[i].num_archivos, lista_tareas[i].tipo_proceso == 0 ? "Sistema" : "Usuario",
               lista_tareas[i].tiempo_inicio, lista_tareas[i].tiempo_finalizacion, lista_tareas[i].tiempo_espera, lista_tareas[i].tiempo_retorno);
               
        suma_espera += lista_tareas[i].tiempo_espera;
        suma_retorno += lista_tareas[i].tiempo_retorno;
    }
    printf("---------------------------------------------------------------------------------------------------------------------------------\n");
    
    // Solo imprimimos los promedios al final
    if (strcmp(titulo, "ESTADO FINAL") == 0) {
        printf("\nMetricas Finales FCFS (Para %d Procesos):\n", NUM_TAREAS);
        printf("-> Tiempo de Espera Promedio : %.2f ms\n", suma_espera / NUM_TAREAS);
        printf("-> Tiempo de Retorno Promedio: %.2f ms\n", suma_retorno / NUM_TAREAS);
        printf("=================================================================================================================================\n\n");
    }
}

/* ==================================================================
   LOGICA DEL PLANIFICADOR DE CPU (FCFS) CON CICLO DE ESTADOS
   ================================================================== */
void ejecutar_FCFS() {
    int reloj_global = 0;
    
    printf("\n\n=================================================================================\n");
    printf("   4. INICIANDO PROCESADOR (ALGORITMO FCFS) Y CICLO DE ESTADOS\n");
    printf("=================================================================================\n");
    
    for (int i = 0; i < NUM_TAREAS; i++) {
        if (reloj_global < lista_tareas[i].tiempo_llegada) {
            reloj_global = lista_tareas[i].tiempo_llegada;
        }
        
        lista_tareas[i].tiempo_inicio = reloj_global;
        lista_tareas[i].tiempo_espera = lista_tareas[i].tiempo_inicio - lista_tareas[i].tiempo_llegada;
        
        // --- CAMBIO AL ESTADO 3 (EJECUCION) ---
        lista_tareas[i].estado = 3;
        
        printf("\n[Reloj: %02d ms] -> Entra Proceso %s (Ciclos: %d ms)\n", reloj_global, lista_tareas[i].nombre_clave, lista_tareas[i].ciclos_cpu);
        printf("   >> PCB ACTUALIZADO: Estado cambia de 1 (Listo) -> 3 (Ejecucion)\n");
               
        for (int c = 1; c <= lista_tareas[i].ciclos_cpu; c++) { reloj_global++; }
        
        simular_FIFO(i);
        
        lista_tareas[i].tiempo_finalizacion = reloj_global;
        lista_tareas[i].tiempo_retorno = lista_tareas[i].tiempo_finalizacion - lista_tareas[i].tiempo_llegada;
        
        // --- CAMBIO AL ESTADO 5 (TERMINADO) ---
        lista_tareas[i].estado = 5;
        
        printf("\n[Reloj: %02d ms] -> Proceso %s Finalizado.\n", reloj_global, lista_tareas[i].nombre_clave);
        printf("   >> PCB ACTUALIZADO: Estado cambia de 3 (Ejecucion) -> 5 (Terminado)\n");
    }
}

/* ==================================================================
   PROGRAMA PRINCIPAL
   ================================================================== */
int main() {
    srand((unsigned)time(NULL));

    // 1. Fase de Memoria Inicial 
    realizar_calculos_conversion();
    generar_tablas(); 
    int marcos_libres_iniciales = total_marcos - marcos_so;
    procesar_secuencias(&marcos_libres_iniciales); 
    imprimir_tablas();

    // 2. Impresión del PCB en estado VIRGEN (Todos en estado 1)
    imprimir_metricas_FCFS("ESTADO INICIAL (EN COLA DE LISTOS)");

    // 3. Fase de Procesador (Actualiza a Estado 3 y luego a 5)
    ejecutar_FCFS();
    
    // 4. Impresión del PCB FINAL (Todos en estado 5 y tiempos calculados)
    imprimir_metricas_FCFS("ESTADO FINAL (TERMINADOS)");

    return 0;
}