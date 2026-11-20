#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

#define SO 20
#define MEMORIA_MAX 200
#define NUM_TAREAS 20

/* ==================================================================
   ESTRUCTURAS DE DATOS 
   ================================================================== */
struct nodo_MT {
    int nbloque;
    int tamaniob;
    int localidad;
    int locfin;
    int acceso;     
    int estado;     
    int fi;         
    struct nodo_MT *sig;
};

struct nodo_JT {
    int ntarea;
    int tamanio;
    int estado;     
    struct nodo_JT *sig;
};

/* ==================================================================
   PROTOTIPOS DE FUNCIONES
   ================================================================== */
void crea_MT(void);
void crea_JT(void);
void imprime_MT_inicial(void);
void imprime_JT_inicial(void);
void imprime_MT_resultado(void);
void imprime_JT_resultado(void);

void primer_ajuste(void);
void imprime_MT_paso(struct nodo_MT *pivote, struct nodo_MT *auxiliar);

/* ==================================================================
   VARIABLES GLOBALES (APUNTADORES)
   ================================================================== */
struct nodo_MT *PMT = NULL, *QMT = NULL, *NuevoMT = NULL, *AuxMT = NULL;
struct nodo_JT *PJT = NULL, *QJT = NULL, *NuevoJT = NULL;

/* ==================================================================
   IMPLEMENTACION: CREACION DE TABLAS
   ================================================================== */
void crea_MT(void) {
    int r, capacidad;
    int mem_disponible = MEMORIA_MAX - SO; 
    int loc_actual = 0; 
    int n_bloque = 0; 

    // Bloque 0 (S.O.)
    PMT = (struct nodo_MT *)malloc(sizeof(struct nodo_MT));
    PMT->nbloque = n_bloque;
    PMT->tamaniob = SO;          
    PMT->localidad = loc_actual; 
    PMT->locfin = loc_actual + SO - 1; 
    PMT->acceso = -1; 
    PMT->estado = 1;  
    PMT->fi = 0;
    PMT->sig = NULL;
    QMT = PMT;

    loc_actual += SO; 
    n_bloque++;       

    printf("\n--- CONFIGURACION DE LA MEMORIA ---\n");
    printf("Vas a ingresar una particion? (1=Si, 0=No): ");
    scanf("%d", &r);

    while (mem_disponible != 0 && r == 1) {
        printf("\nMemoria restante: %d KB\n", mem_disponible);
        printf("De cuanto quieres tu bloque?: ");
        scanf("%d", &capacidad);

        if (capacidad <= mem_disponible) {
            NuevoMT = (struct nodo_MT *)malloc(sizeof(struct nodo_MT));
            NuevoMT->nbloque = n_bloque;
            NuevoMT->tamaniob = capacidad;
            NuevoMT->localidad = loc_actual;
            NuevoMT->locfin = loc_actual + capacidad - 1;
            NuevoMT->acceso = 0; 
            NuevoMT->estado = 0; 
            NuevoMT->fi = 0;
            NuevoMT->sig = NULL;
            
            QMT->sig = NuevoMT;
            QMT = NuevoMT;

            loc_actual += capacidad;
            mem_disponible -= capacidad;
            n_bloque++;

            if (mem_disponible == 0) {
                printf("\nListo (Se ha asignado toda la memoria)\n");
            } else {
                printf("Quieres otra particion? (1=Si, 0=No): ");
                scanf("%d", &r);
            }
        } else {
            printf("Error: La capacidad supera los %d KB restantes.\n", mem_disponible);
        }
    }

    if (mem_disponible > 0) {
        NuevoMT = (struct nodo_MT *)malloc(sizeof(struct nodo_MT));
        NuevoMT->nbloque = n_bloque;
        NuevoMT->tamaniob = mem_disponible; 
        NuevoMT->localidad = loc_actual;
        NuevoMT->locfin = loc_actual + mem_disponible - 1;
        NuevoMT->acceso = 0; 
        NuevoMT->estado = 0; 
        NuevoMT->fi = 0;
        NuevoMT->sig = NULL;
        
        QMT->sig = NuevoMT;
        QMT = NuevoMT;
        printf("\n[S.O.] Se ha creado un bloque final con los %d KB restantes.\n", mem_disponible);
    }
}

void crea_JT(void) {
    int i;
    for (i = 0; i < NUM_TAREAS; i++) {
        int tamanio_aleatorio = (rand() % 41) + 5; 
        if (PJT == NULL) {
            PJT = (struct nodo_JT *)malloc(sizeof(struct nodo_JT));
            PJT->ntarea = i + 1;
            PJT->tamanio = tamanio_aleatorio;
            PJT->estado = 0; 
            PJT->sig = NULL;
            QJT = PJT;
        } else {
            NuevoJT = (struct nodo_JT *)malloc(sizeof(struct nodo_JT));
            NuevoJT->ntarea = i + 1;
            NuevoJT->tamanio = tamanio_aleatorio;
            NuevoJT->estado = 0; 
            NuevoJT->sig = NULL;
            QJT->sig = NuevoJT;
            QJT = NuevoJT;
        }
    }
}

/* ==================================================================
   ALGORITMO DE ASIGNACION (SIGUIENTE AJUSTE - PASO A PASO)
   ================================================================== */
void primer_ajuste(void) {
    struct nodo_JT *aux_JT = PJT; 
    struct nodo_MT *pivote = NULL; 
    struct nodo_MT *auxiliar;
    
    // Puntero especial para recordar donde se quedo el pivote
    struct nodo_MT *inicio_busqueda = PMT->sig; 
    
    int bloques_libres = 0;
    struct nodo_MT *temp_conteo = PMT->sig;
    while (temp_conteo != NULL) {
        bloques_libres++;
        temp_conteo = temp_conteo->sig;
    }

    printf("\n\nINICIANDO ASIGNACION: SIGUIENTE AJUSTE...\n");
    
    while (aux_JT != NULL && bloques_libres > 0) {
        
        auxiliar = inicio_busqueda; 
        int dio_vuelta = 0; 

        while (auxiliar != NULL) {
            printf("\n\n>>> Evaluando Tarea T%d (%d KB) en Bloque %d <<<\n", aux_JT->ntarea, aux_JT->tamanio, auxiliar->nbloque);
            imprime_MT_paso(pivote, auxiliar);
            printf("Presione Enter para evaluar el bloque marcado con (->) ...");
            getch();

            if (auxiliar->estado == 0 && auxiliar->tamaniob >= aux_JT->tamanio) {
                // Asignacion Exitosa
                auxiliar->estado = 1; 
                auxiliar->acceso = aux_JT->ntarea;
                auxiliar->fi = auxiliar->tamaniob - aux_JT->tamanio; 
                aux_JT->estado = 1; 
                
                // Actualizamos el pivote
                pivote = auxiliar;
                bloques_libres--; 

                // Le decimos al buscador que para la SIGUIENTE tarea empiece despues
                inicio_busqueda = pivote->sig;
                if (inicio_busqueda == NULL) {
                    inicio_busqueda = PMT->sig; 
                }

                // CORRECCION AQUI: Movemos el auxiliar visualmente al siguiente bloque DE INMEDIATO
                auxiliar = inicio_busqueda;

                printf("\n[EXITO] La Tarea T%d fue asignada al Bloque %d!\n", aux_JT->ntarea, pivote->nbloque);
                imprime_MT_paso(pivote, auxiliar);
                printf("Presione Enter para pasar a la siguiente tarea...");
                getch();
                break; 
            } else {
                if (auxiliar->estado == 1) {
                    printf("\n[!] El Bloque %d esta OCUPADO. Pasando al siguiente...\n", auxiliar->nbloque);
                } else {
                    printf("\n[!] El Bloque %d NO TIENE CAPACIDAD. Pasando al siguiente...\n", auxiliar->nbloque);
                }
            }
            
            // Avanzamos el auxiliar
            auxiliar = auxiliar->sig;

            if (auxiliar == NULL && dio_vuelta == 0) {
                printf("\n[INFO] Llegamos al final de la memoria. Volviendo arriba para revisar los bloques saltados...\n");
                auxiliar = PMT->sig;
                dio_vuelta = 1;
                getch();
            }

            if (dio_vuelta == 1 && auxiliar == inicio_busqueda) {
                break;
            }
        }
        
        if (aux_JT->estado == 0) {
            printf("\n[ESPERA] La Tarea T%d (%d KB) no encontro ningun bloque disponible.\n", aux_JT->ntarea, aux_JT->tamanio);
            printf("Presione Enter para continuar...");
            getch();
        }
        
        aux_JT = aux_JT->sig;
    }

    if (bloques_libres == 0 && aux_JT != NULL) {
        printf("\n=======================================================================\n");
        printf("[INFO] Se ha ocupado el ultimo bloque de memoria disponible.\n");
        printf("El resto de las tareas pasaran automaticamente a estado de Espera (-).\n");
        printf("=======================================================================\n");
        printf("Presione Enter para ver los resultados finales...\n");
        getch();
    }
}

/* ==================================================================
   IMPRESIONES
   ================================================================== */
void imprime_MT_inicial(void) {
    AuxMT = PMT;
    printf("\n=======================================================================\n");
    printf("              TABLA DE MEMORIA (MT) - ESTADO INICIAL                   \n");
    printf("=======================================================================\n");
    printf("Bloque\tTamano\tLoc.In\tLoc.Fin\tAsignacion\tEstado\tF.I.\n");
    printf("-----------------------------------------------------------------------\n");
    while (AuxMT != NULL) {
        printf("%d\t%d KB\t%d\t%d\t", AuxMT->nbloque, AuxMT->tamaniob, AuxMT->localidad, AuxMT->locfin);
        if(AuxMT->nbloque == 0) {
            printf("S.O.\t\t1\t0 KB\n");
        } else {
            printf("0\t\t0\t0 KB\n");
        }
        AuxMT = AuxMT->sig;
    }
}

void imprime_JT_inicial(void) {
    struct nodo_JT *aux_JT = PJT;
    printf("\n==============================\n");
    printf("--- TABLA DE TAREAS INICIAL --\n");
    printf("==============================\n");
    printf("Tarea\tTamano\tEstado\n");
    printf("------------------------------\n");
    while (aux_JT != NULL) {
        printf("T%d\t%d KB\t0\n", aux_JT->ntarea, aux_JT->tamanio);
        aux_JT = aux_JT->sig;
    }
}

void imprime_MT_paso(struct nodo_MT *pivote, struct nodo_MT *auxiliar) {
    struct nodo_MT *temp = PMT;
    printf("\n-----------------------------------------------------------------------\n");
    printf(" PUNTEROS: [*] Pivote (Asignado)  |  [->] Auxiliar (Explorando) \n");
    printf("-----------------------------------------------------------------------\n");
    printf("     Bloque\tTamano\tLoc.In\tLoc.Fin\tAsignacion\tEstado\tF.I.\n");
    printf("-----------------------------------------------------------------------\n");
    
    while (temp != NULL) {
        if (temp == pivote && temp == auxiliar) {
            printf(" *-> ");
        } else if (temp == pivote) {
            printf("  * ");
        } else if (temp == auxiliar) {
            printf(" ->  ");
        } else {
            printf("     ");
        }

        printf("%d\t%d KB\t%d\t%d\t", temp->nbloque, temp->tamaniob, temp->localidad, temp->locfin);
        
        if(temp->nbloque == 0) {
            printf("S.O.\t\t1\t0 KB\n");
        } else {
            if(temp->estado == 1) {
                printf("T%d\t\t1\t%d KB\n", temp->acceso, temp->fi);
            } else {
                printf("0\t\t0\t0 KB\n");
            }
        }
        temp = temp->sig;
    }
    
    printf("-----------------------------------------------------------------------\n");
    printf(" Direcciones de Memoria de los Punteros:\n");
    
    // CORRECCION AQUI: Se imprime el numero de bloque junto a la direccion
    if (pivote != NULL) {
        printf(" Pivote (*)   : %p \tBloque: %d\n", (void*)pivote, pivote->nbloque);
    } else {
        printf(" Pivote (*)   : NULL \t\t\tBloque: N/A\n");
    }
    
    if (auxiliar != NULL) {
        printf(" Auxiliar (->): %p \tBloque: %d\n", (void*)auxiliar, auxiliar->nbloque);
    } else {
        printf(" Auxiliar (->): NULL \t\t\tBloque: N/A\n");
    }
    printf("=======================================================================\n");
}

void imprime_MT_resultado(void) {
    int suma_fi = 0; 
    AuxMT = PMT;
    printf("\n=======================================================================\n");
    printf("Bloque\tTamano\tLoc.In\tLoc.Fin\tAsignacion\tEstado\tF.I.\n");
    printf("-----------------------------------------------------------------------\n");
    while (AuxMT != NULL) {
        printf("%d\t%d KB\t%d\t%d\t", AuxMT->nbloque, AuxMT->tamaniob, AuxMT->localidad, AuxMT->locfin);
        if(AuxMT->nbloque == 0) {
            printf("S.O.\t\t1\t0 KB\n");
        } else {
            if(AuxMT->estado == 1) {
                printf("T%d\t\t1\t%d KB\n", AuxMT->acceso, AuxMT->fi);
                suma_fi += AuxMT->fi; 
            } else {
                printf("0\t\t0\t0 KB\n");
            }
        }
        AuxMT = AuxMT->sig;
    }
    printf("-----------------------------------------------------------------------\n");
    printf("\t\t\tTOTAL DE FRAGMENTACION INTERNA: \t%d KB\n", suma_fi); 
    printf("=======================================================================\n");
}

void imprime_JT_resultado(void) {
    struct nodo_JT *aux_JT = PJT;
    printf("\n==============================\n");
    printf("Tarea\tTamano\tEstado\n");
    printf("------------------------------\n");
    while (aux_JT != NULL) {
        printf("T%d\t%d KB\t", aux_JT->ntarea, aux_JT->tamanio);
        if (aux_JT->estado == 1) {
            printf("Eliminado (x)\n");
        } else {
            printf("Espera (-)\n");
        }
        aux_JT = aux_JT->sig;
    }
}

/* ==================================================================
   FUNCION PRINCIPAL 
   ================================================================== */
int main() {
    PMT = NULL; QMT = NULL;
    PJT = NULL; QJT = NULL;
    srand((unsigned)time(NULL));

    crea_MT(); 
    crea_JT(); 

    printf("\n\n>>>>>>>>>>>>>>>>>> FASE 0: ESTADO INICIAL <<<<<<<<<<<<<<<<<<");
    imprime_MT_inicial();
    imprime_JT_inicial();
    printf("\nPresione una tecla para continuar a la asignacion...\n");
    getch();

    primer_ajuste();

    printf("\n\n>>>>>>>>>>>>>>>>>> RESULTADO FINAL <<<<<<<<<<<<<<<<<<");
    imprime_MT_resultado();
    imprime_JT_resultado();

    printf("\nProceso finalizado. Presione cualquier tecla para salir...");
    getch(); 
    
    return 0;
}
