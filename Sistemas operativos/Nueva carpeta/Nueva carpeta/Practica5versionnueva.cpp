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

// MARCO DE PAGINA Y S.O.
#define MARCO_PAGINA_B       0
#define MARCO_PAGINA_KB      64
#define SO_LINEAS            0
#define SO_MARCOS            2
#define SO_CAPACIDAD_KB      0

// TAREAS Y CPU
#define NUM_TAREAS           20
#define MIN_CICLOS           5   // Modificado: minimo 5 ciclos
#define MAX_CICLOS           10  // Modificado: maximo 10 ciclos

// PAGINACION POR DEMANDA 
#define SECUENCIA            8
#define MARCOS_ASIGNADOS     3

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
    int ntarea;
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
    char nombre_clave[15];   
    int tiempo_llegada;      
    int ciclos_cpu;
    int ciclos_restantes;    
    int estado;              // 1=Nuevo, 2=Listo, 3=Ejec, 4=Bloq, 5=Terminado
    int memoria_kb;          
    int num_archivos;        
    int tipo_operacion;      
    int num_recursos;        
    int tipo_proceso;        
    
    // Tiempos para metricas
    int tiempo_inicio;
    int tiempo_finalizacion; 
    int tiempo_retorno;      
    int tiempo_espera;       // Nuevo: Tiempo de espera añadido
};

/* ==================================================================
   VARIABLES GLOBALES
   ================================================================== */
struct nodo_MMT *PMMT = NULL, *QMMT = NULL;
struct nodo_JT *PJT = NULL, *QJT = NULL;
struct nodo_PMT *arreglo_PMT[100]; 

struct PCB arreglo_procesos[200]; 
int total_procesos_generados = 0;

float memoria_total_kb = 0;
float cap_marco_kb = 0;
int marcos_so = 0;
int total_marcos = 0;

/* ==================================================================
   FUNCIONES AUXILIARES
   ================================================================== */
int lineas_es_unico(int lineas, int *arreglo_lineas, int limite) {
    int i;
    for (i = 0; i < limite; i++) {
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
   Fase 1: CALCULOS Y TABLAS DE MEMORIA
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
    int i, s, p;
    struct nodo_MMT *nuevo_mmt;
    int *lineas_usadas;
    int lineas_rand, pags_req, max_pag;
    struct nodo_JT *nuevoJT;
    struct nodo_PMT *Q_pmt, *nuevoPMT;

    for (i = 0; i < total_marcos; i++) {
        nuevo_mmt = (struct nodo_MMT *)malloc(sizeof(struct nodo_MMT));
        nuevo_mmt->marco = i; nuevo_mmt->loc_inicio = loc_actual;
        nuevo_mmt->estado = (i < marcos_so) ? 1 : 0; nuevo_mmt->sig = NULL;
        if (PMMT == NULL) { PMMT = nuevo_mmt; QMMT = PMMT; } 
        else { QMMT->sig = nuevo_mmt; QMMT = nuevo_mmt; }
        loc_actual += cap_marco_kb;
    }
    
    lineas_usadas = (int*)malloc(NUM_TAREAS * sizeof(int));
    for (i = 0; i < NUM_TAREAS; i++) {
        do { lineas_rand = (rand() % 801) + 200; } while (!lineas_es_unico(lineas_rand, lineas_usadas, i));
        lineas_usadas[i] = lineas_rand;

        pags_req = techo((float)lineas_rand / cap_marco_kb);
        nuevoJT = (struct nodo_JT *)malloc(sizeof(struct nodo_JT));
        sprintf(nuevoJT->nombre_clave, "T%d", i + 1);
        nuevoJT->ntarea = i + 1;
        nuevoJT->lineas = lineas_rand;
        nuevoJT->localidad_pmt = &arreglo_PMT[i];
        nuevoJT->localidad_pmt_simulada = (rand() % 90000) + 8000;
        
        max_pag = (pags_req > 10) ? 10 : pags_req;
        nuevoJT->secuencia[0] = 0; 
        for(s = 1; s < SECUENCIA; s++) nuevoJT->secuencia[s] = rand() % max_pag; 

        nuevoJT->sig = NULL;
        if (PJT == NULL) { PJT = nuevoJT; QJT = PJT; } 
        else { QJT->sig = nuevoJT; QJT = nuevoJT; }
        
        arreglo_PMT[i] = NULL;
        Q_pmt = NULL;
        for (p = 0; p < pags_req; p++) {
            nuevoPMT = (struct nodo_PMT *)malloc(sizeof(struct nodo_PMT));
            nuevoPMT->pagina = p; nuevoPMT->marco_fisico = -1; nuevoPMT->estado = 0;        
            nuevoPMT->referencia = 0; nuevoPMT->modificacion = 0; nuevoPMT->sig = NULL;
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
    int marcos_asignados_tarea, s, pagina_solicitada;
    struct nodo_PMT *aux_pmt;

    while (aux_jt != NULL) {
        marcos_asignados_tarea = 0; 
        for (s = 0; s < SECUENCIA; s++) {
            pagina_solicitada = aux_jt->secuencia[s];
            aux_pmt = arreglo_PMT[index];
            while (aux_pmt != NULL && aux_pmt->pagina != pagina_solicitada) aux_pmt = aux_pmt->sig;
            if (aux_pmt != NULL && aux_pmt->estado == 0 && *marcos_libres > 0 && marcos_asignados_tarea < MARCOS_ASIGNADOS) { 
                aux_pmt->marco_fisico = marco_disponible++;
                aux_pmt->estado = 1; (*marcos_libres)--; marcos_asignados_tarea++;
            } 
        }
        
        aux_pmt = arreglo_PMT[index];
        while (aux_pmt != NULL && marcos_asignados_tarea < MARCOS_ASIGNADOS && *marcos_libres > 0) {
            if (aux_pmt->estado == 0) {
                aux_pmt->marco_fisico = marco_disponible++;
                aux_pmt->estado = 1; (*marcos_libres)--; marcos_asignados_tarea++;
            }
            aux_pmt = aux_pmt->sig;
        }
        aux_jt = aux_jt->sig; index++;
    }
}

void imprimir_tablas() {
    struct nodo_MMT *aux_mmt = PMMT;
    struct nodo_JT *aux_jt = PJT;
    struct nodo_PMT *aux_pmt;
    int index = 0, s;

    printf("\n======================================================\n");
    printf("   1. TABLA DE ADMINISTRACION DE MEMORIA (MMT INICIAL)\n");
    printf("======================================================\n");
    printf("Marco\tLoc.Inicio(kb)\tEstado\n");
    printf("------------------------------------------------------\n");
    while (aux_mmt != NULL) {
        printf("%d\t%.2f\t\t%d\n", aux_mmt->marco, aux_mmt->loc_inicio, aux_mmt->estado);
        aux_mmt = aux_mmt->sig;
    }

    printf("\n======================================================\n");
    printf("   2. TABLA DE TAREAS (JT INICIAL)\n");
    printf("======================================================\n");
    printf("Tarea\tLineas\tLoc_Pmt/Smt\tSecuencia\n");
    printf("------------------------------------------------------\n");
    while (aux_jt != NULL) {
        printf("%s\t%d\t%d\t\t", aux_jt->nombre_clave, aux_jt->lineas, aux_jt->localidad_pmt_simulada);
        for(s = 0; s < SECUENCIA; s++) printf("%d ", aux_jt->secuencia[s]);
        printf("\n");
        aux_jt = aux_jt->sig;
    }

    printf("\n======================================================\n");
    printf("   3. TABLAS DE MAPA (PMT / SMT INICIAL) POR TAREA\n");
    printf("======================================================\n");
    aux_jt = PJT;
    while (aux_jt != NULL) {
        printf("\n--- PMT/SMT de la Tarea %s ---\n", aux_jt->nombre_clave);
        printf("  Pagina\tMarco\tEstado\tReferencia\tModificacion\n");
        aux_pmt = arreglo_PMT[index];
        while (aux_pmt != NULL) {
            if (aux_pmt->marco_fisico == -1) printf("  %d\t\t-\t%d\t%d\t\t%d\n", aux_pmt->pagina, aux_pmt->estado, aux_pmt->referencia, aux_pmt->modificacion);
            else printf("  %d\t\t%d\t%d\t%d\t\t%d\n", aux_pmt->pagina, aux_pmt->marco_fisico, aux_pmt->estado, aux_pmt->referencia, aux_pmt->modificacion);
            aux_pmt = aux_pmt->sig;
        }
        aux_jt = aux_jt->sig;
        index++;
    }
}

/* ==================================================================
   Fase 2: PLANIFICADOR DE TAREAS (Extrae de la Secuencia)
   ================================================================== */
struct PCB* planificacion_de_tareas(int *out_num_procesos) {
    int num_proc = 0;
    int tiempo_llegada_acumulado = 0;
    struct nodo_JT *aux_jt = PJT;
    int s, u, i, j;

    printf("\n=========================================================================\n");
    printf("   PLANIFICADOR DE TAREAS (EXTRACCION DIRECTA DESDE LA SECUENCIA JT) \n");
    printf("=========================================================================\n");

    while (aux_jt != NULL) {
        int unicas[SECUENCIA];
        int cont_unicas = 0;

        for (s = 0; s < SECUENCIA; s++) {
            int pag = aux_jt->secuencia[s];
            int existe = 0;
            for (u = 0; u < cont_unicas; u++) {
                if (unicas[u] == pag) { existe = 1; break; }
            }
            if (!existe) {
                unicas[cont_unicas] = pag;
                cont_unicas++;
            }
        }

        for (i = 0; i < cont_unicas - 1; i++) {
            for (j = i + 1; j < cont_unicas; j++) {
                if (unicas[i] > unicas[j]) {
                    int temp = unicas[i];
                    unicas[i] = unicas[j];
                    unicas[j] = temp;
                }
            }
        }

        printf(" -> %s: Paginas que seran procesadas: ", aux_jt->nombre_clave);
        if (cont_unicas == 0) {
            printf("Ninguna\n");
        } else {
            for (u = 0; u < cont_unicas; u++) {
                printf("P%d ", unicas[u]);

                sprintf(arreglo_procesos[num_proc].nombre_clave, "%sP%d", aux_jt->nombre_clave, unicas[u]);
                arreglo_procesos[num_proc].tiempo_llegada = tiempo_llegada_acumulado;
                tiempo_llegada_acumulado += rand() % 3; // Mantener un orden natural de llegada

                // Limites modificados a min 5 y max 10
                arreglo_procesos[num_proc].ciclos_cpu = (rand() % (MAX_CICLOS - MIN_CICLOS + 1)) + MIN_CICLOS;
                arreglo_procesos[num_proc].ciclos_restantes = arreglo_procesos[num_proc].ciclos_cpu;
                arreglo_procesos[num_proc].estado = 1; 
                
                arreglo_procesos[num_proc].memoria_kb = MARCO_PAGINA_KB; 
                arreglo_procesos[num_proc].num_archivos = rand() % 5;
                arreglo_procesos[num_proc].tipo_operacion = rand() % 2;
                arreglo_procesos[num_proc].num_recursos = (rand() % 3) + 1;
                arreglo_procesos[num_proc].tipo_proceso = rand() % 2;

                arreglo_procesos[num_proc].tiempo_inicio = -1;
                arreglo_procesos[num_proc].tiempo_finalizacion = 0;
                arreglo_procesos[num_proc].tiempo_retorno = 0;
                arreglo_procesos[num_proc].tiempo_espera = 0; // Inicializamos tiempo espera

                num_proc++;
            }
            printf("\n");
        }
        aux_jt = aux_jt->sig;
    }

    *out_num_procesos = num_proc;
    printf("\n>> [!] El Planificador genero %d sub-procesos a partir de las secuencias.\n", num_proc);
    
    return arreglo_procesos; 
}

/* ==================================================================
   IMPRESION DEL PCB (Actualizado con T.Espera)
   ================================================================== */
void imprimir_PCB(struct PCB* array_pcb, int num_proc, const char* titulo) {
    float suma_retorno = 0;
    float suma_espera = 0;
    int i, t_ini;
    
    printf("\n==========================================================================================================================================\n");
    printf("                                            BLOQUE DE CONTROL DE PROCESOS (PCB) - %s                                      \n", titulo);
    printf("==========================================================================================================================================\n");
    printf(" Proceso | T.Lleg. | Ciclos | Rest | Estado | Memoria | CPU/E/S | Num_Disp | No.Arch |  Tipo   | T.Ini | T.Fin | T.Retorno | T.Espera |\n");
    printf("------------------------------------------------------------------------------------------------------------------------------------------\n");
    
    for (i = 0; i < num_proc; i++) {
        t_ini = (array_pcb[i].tiempo_inicio == -1) ? 0 : array_pcb[i].tiempo_inicio;
        
        printf("  %-6s |   %03d   |  %03d   |  %03d |    %d   | %04d KB |   %-3s   |    %02d    |   %02d    | %-7s |  %03d  |  %03d  |    %03d    |   %03d    |\n",
               array_pcb[i].nombre_clave, array_pcb[i].tiempo_llegada, array_pcb[i].ciclos_cpu, array_pcb[i].ciclos_restantes,
               array_pcb[i].estado, array_pcb[i].memoria_kb, array_pcb[i].tipo_operacion == 0 ? "CPU" : "E/S",
               array_pcb[i].num_recursos, array_pcb[i].num_archivos, array_pcb[i].tipo_proceso == 0 ? "Sistema" : "Usuario",
               t_ini, array_pcb[i].tiempo_finalizacion, array_pcb[i].tiempo_retorno, array_pcb[i].tiempo_espera);
               
        suma_retorno += array_pcb[i].tiempo_retorno;
        suma_espera += array_pcb[i].tiempo_espera;
    }
    printf("------------------------------------------------------------------------------------------------------------------------------------------\n");
    
    if (strcmp(titulo, "ESTADO FINAL") == 0) {
        printf("\nMetricas Finales FCFS (First Come, First Served):\n");
        printf("-> Tiempo de Retorno Promedio: %.2f ms\n", suma_retorno / num_proc);
        printf("-> Tiempo de Espera Promedio : %.2f ms\n", suma_espera / num_proc);
        printf("==========================================================================================================================================\n\n");
    }
}

/* ==================================================================
   SIMULACION FIFO (Llamada tras procesamiento CPU)
   ================================================================== */
void simular_FIFO(int id_tarea_real) {
    struct nodo_JT *tarea = PJT;
    int current_index = 0;
    int marcos_fisicos[MARCOS_ASIGNADOS];
    int idx = 0;
    struct nodo_PMT *p_temp;
    struct nodo_auxiliar lista_aux[MARCOS_ASIGNADOS];
    int puntero_fifo = 0, marcos_ocupados = 0;
    int i, j, pagina_actual, hit;
    
    while(tarea != NULL && current_index != id_tarea_real) { 
        tarea = tarea->sig; current_index++; 
    }
    if(tarea == NULL) return;

    p_temp = arreglo_PMT[id_tarea_real];
    while(p_temp != NULL && idx < MARCOS_ASIGNADOS) {
        if(p_temp->marco_fisico != -1) { marcos_fisicos[idx] = p_temp->marco_fisico; idx++; }
        p_temp = p_temp->sig;
    }
    
    if (idx < MARCOS_ASIGNADOS) return; 

    for (j = 0; j < MARCOS_ASIGNADOS; j++) lista_aux[j].pagina = -1; 

    for (i = 0; i < SECUENCIA; i++) {
        pagina_actual = tarea->secuencia[i];
        hit = 0;
        for (j = 0; j < MARCOS_ASIGNADOS; j++) {
            if (lista_aux[j].pagina == pagina_actual) { hit = 1; break; }
        }
        if (!hit) {
            if (marcos_ocupados < MARCOS_ASIGNADOS) {
                lista_aux[marcos_ocupados].pagina = pagina_actual; marcos_ocupados++;
            } else {
                lista_aux[puntero_fifo].pagina = pagina_actual;
                puntero_fifo = (puntero_fifo + 1) % MARCOS_ASIGNADOS;
            }
        }
    }

    p_temp = arreglo_PMT[id_tarea_real];
    while(p_temp != NULL) { 
        p_temp->marco_fisico = -1; p_temp->estado = 0; 
        p_temp->referencia = 0; p_temp->modificacion = 0; 
        p_temp = p_temp->sig; 
    }

    for (j = 0; j < MARCOS_ASIGNADOS; j++) {
        if (lista_aux[j].pagina != -1) {
            p_temp = arreglo_PMT[id_tarea_real];
            while(p_temp != NULL) {
                if(p_temp->pagina == lista_aux[j].pagina) {
                    p_temp->marco_fisico = marcos_fisicos[j]; 
                    p_temp->estado = 1; p_temp->referencia = 1; 
                    p_temp->modificacion = rand() % 2;    
                }
                p_temp = p_temp->sig;
            }
        }
    }
}

/* ==================================================================
   Fase 3: PLANIFICADOR DE CPU (FCFS)
   ================================================================== */
void ejecutar_FCFS(struct PCB* array_pcb, int num_proc) {
    int reloj_global = 0;
    int i, c;
    int id_tarea_real;
    
    printf("\n\n=================================================================================\n");
    printf("   4. INICIANDO PROCESADOR (FCFS - FIRST COME, FIRST SERVED)\n");
    printf("=================================================================================\n");
    
    // FCFS atiende a los procesos estrictamente en orden de llegada. 
    // Debido a que nuestro generador ya los creo en ese orden secuencialmente en el arreglo, 
    // podemos recorrer el arreglo directamente.
    for (i = 0; i < num_proc; i++) {
        // Si el reloj global va mas atrasado que el tiempo de llegada, la CPU debe esperar
        if (reloj_global < array_pcb[i].tiempo_llegada) {
            reloj_global = array_pcb[i].tiempo_llegada;
        }

        // Marcar Inicio de Ejecucion
        array_pcb[i].tiempo_inicio = reloj_global;
        array_pcb[i].estado = 3; // Estado = Ejecutando
        
        printf("\n[Reloj: %03d ms] -> Entra %s (Op: %s, Ciclos Totales a Ejecutar: %d ms)\n", 
               reloj_global, array_pcb[i].nombre_clave, 
               array_pcb[i].tipo_operacion == 0 ? "CPU" : "E/S", 
               array_pcb[i].ciclos_cpu);
        
        // FCFS procesa todos los ciclos sin interrumpir
        for (c = 1; c <= array_pcb[i].ciclos_cpu; c++) {
            reloj_global++;
        }
        
        array_pcb[i].ciclos_restantes = 0;
        printf("   - [Ejecutando...] Consumio %d ms de CPU continuos.\n", array_pcb[i].ciclos_cpu);

        // Simulamos la entrada a memoria por Paginacion de la tarea a la que pertenece
        sscanf(array_pcb[i].nombre_clave, "T%dP%*d", &id_tarea_real);
        simular_FIFO(id_tarea_real - 1); 

        // Finaliza el Proceso
        array_pcb[i].estado = 5; // Estado = Terminado
        array_pcb[i].tiempo_finalizacion = reloj_global;
        
        // Calculo de Tiempos
        array_pcb[i].tiempo_retorno = array_pcb[i].tiempo_finalizacion - array_pcb[i].tiempo_llegada;
        array_pcb[i].tiempo_espera = array_pcb[i].tiempo_inicio - array_pcb[i].tiempo_llegada;
        
        printf("   >> Proceso %s FINALIZADO. (Pasa a Estado 5)\n", array_pcb[i].nombre_clave);
    }
}

/* ==================================================================
   PROGRAMA PRINCIPAL
   ================================================================== */
int main() {
    int marcos_libres_iniciales;
    struct PCB* ptr_procesos;
    
    srand((unsigned)time(NULL));

    // 1. Fase de Memoria Inicial 
    realizar_calculos_conversion();
    generar_tablas(); 
    
    marcos_libres_iniciales = total_marcos - marcos_so;
    procesar_secuencias(&marcos_libres_iniciales); 
    
    imprimir_tablas();
    
    // 2. Extraer SOLO las paginas unicas de la secuencia
    ptr_procesos = planificacion_de_tareas(&total_procesos_generados);
    
    // 3. Imprimir PCB VIRGEN
    imprimir_PCB(ptr_procesos, total_procesos_generados, "ESTADO INICIAL");

    // 4. Ejecutar CPU usando FCFS
    ejecutar_FCFS(ptr_procesos, total_procesos_generados);
    
    // 5. Imprimir PCB ACTUALIZADO
    imprimir_PCB(ptr_procesos, total_procesos_generados, "ESTADO FINAL");

    return 0;
}