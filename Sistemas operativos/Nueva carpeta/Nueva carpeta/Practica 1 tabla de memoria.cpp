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
void imprime_MT(void);
void imprime_JT(void);

/* ==================================================================
   VARIABLES GLOBALES (APUNTADORES)
   ================================================================== */
struct nodo_MT *PMT = NULL, *QMT = NULL, *NuevoMT = NULL, *AuxMT = NULL;
struct nodo_JT *PJT = NULL, *QJT = NULL, *NuevoJT = NULL;

/* ==================================================================
   IMPLEMENTACION DE FUNCIONES
   ================================================================== */

void crea_MT(void) {
    int r; 
    int capacidad;
    int mem_disponible = MEMORIA_MAX - SO; // 180 KB
    int loc_actual = 0; 
    int n_bloque = 0; // Empezamos en el bloque 0 (S.O.)

    // 1. SIEMPRE CREAMOS EL BLOQUE 0 PARA EL SISTEMA OPERATIVO
    PMT = (struct nodo_MT *)malloc(sizeof(struct nodo_MT));
    PMT->nbloque = n_bloque;
    PMT->tamaniob = SO;          // 20 KB
    PMT->localidad = loc_actual; // Empieza en 0
    PMT->locfin = loc_actual + SO - 1; // Termina en 19
    PMT->acceso = -1; // Usamos -1 como bandera para "S.O."
    PMT->estado = 1;  // 1 = Ocupado
    PMT->fi = 0;
    PMT->sig = NULL;
    QMT = PMT;

    loc_actual += SO; // Actualizamos la localidad a 20
    n_bloque++;       // Siguiente bloque sera el 1

    printf("\n--- CONFIGURACION DE LA MEMORIA ---\n");
    printf("Vas a ingresar una particion? (1=Si, 0=No): ");
    scanf("%d", &r);

    // 2. CICLO DE CREACION DE BLOQUES MANUALES
    while (mem_disponible != 0 && r == 1) {
        
        // Te avisamos cuanta memoria resta (como pediste)
        printf("\nMemoria restante: %d KB\n", mem_disponible);
        printf("De cuanto quieres tu bloque?: ");
        scanf("%d", &capacidad);

        if (capacidad <= mem_disponible) {
            
            // Creamos el nuevo nodo
            NuevoMT = (struct nodo_MT *)malloc(sizeof(struct nodo_MT));
            NuevoMT->nbloque = n_bloque;
            NuevoMT->tamaniob = capacidad;
            NuevoMT->localidad = loc_actual;
            NuevoMT->locfin = loc_actual + capacidad - 1;
            NuevoMT->acceso = 0; // 0 = Sin tarea asignada
            NuevoMT->estado = 0; // 0 = Libre
            NuevoMT->fi = 0;
            NuevoMT->sig = NULL;
            
            QMT->sig = NuevoMT;
            QMT = NuevoMT;

            // Actualizamos contadores
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
            printf("Error: La capacidad supera los %d KB restantes. Intenta de nuevo.\n", mem_disponible);
        }
    }

    // 3. SI EL USUARIO DICE "0" Y AUN SOBRA MEMORIA
    // (Aplica si dice "0" inmediatamente al inicio, o si sobra memoria y decide salir)
    if (mem_disponible > 0) {
        NuevoMT = (struct nodo_MT *)malloc(sizeof(struct nodo_MT));
        NuevoMT->nbloque = n_bloque;
        NuevoMT->tamaniob = mem_disponible; // Agarra toda la memoria restante
        NuevoMT->localidad = loc_actual;
        NuevoMT->locfin = loc_actual + mem_disponible - 1;
        NuevoMT->acceso = 0; // 0 = Sin tarea
        NuevoMT->estado = 0; // 0 = Libre
        NuevoMT->fi = 0;
        NuevoMT->sig = NULL;
        
        QMT->sig = NuevoMT;
        QMT = NuevoMT;
        
        printf("\n[S.O.] Se ha creado un bloque final con los %d KB restantes.\n", mem_disponible);
    }
}

// Genera la tabla de tareas con un 'for' a 20 (Para la Practica 1, solo se crean)
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

void imprime_MT(void) {
    AuxMT = PMT;
    printf("\n=======================================================================\n");
    printf("              TABLA DE ADMINISTRACION DE MEMORIA (MT)                  \n");
    printf("=======================================================================\n");
    printf("Bloque\tTamano\tLoc.In\tLoc.Fin\tAsignacion\tEstado\tF.I.\n");
    printf("-----------------------------------------------------------------------\n");
    while (AuxMT != NULL) {
        printf("%d\t%d KB\t%d\t%d\t", AuxMT->nbloque, AuxMT->tamaniob, AuxMT->localidad, AuxMT->locfin);
        
        if(AuxMT->nbloque == 0) {
            // Reglas especificas para el Bloque 0 (S.O.)
            printf("S.O.\t\t1\t0 KB\n");
        } else {
            // Reglas para los demas bloques que aun no se asignan (estado 0)
            printf("0\t\t0\t0 KB\n");
        }
        AuxMT = AuxMT->sig;
    }
}

void imprime_JT(void) {
    struct nodo_JT *aux_JT = PJT;
    printf("\n==============================\n");
    printf("--- TABLA DE TAREAS (JT) ---\n");
    printf("==============================\n");
    printf("Tarea\tTamano\tEstado\n");
    printf("------------------------------\n");
    while (aux_JT != NULL) {
        // En esta practica todas las tareas nacen en estado 0 (Aun no asignadas)
        printf("T%d\t%d KB\t0\n", aux_JT->ntarea, aux_JT->tamanio);
        aux_JT = aux_JT->sig;
    }
}

/* ==================================================================
   FUNCION PRINCIPAL 
   ================================================================== */
int main() {
    // Inicializar punteros globales
    PMT = NULL; QMT = NULL;
    PJT = NULL; QJT = NULL;
    
    srand((unsigned)time(NULL));

    // 1. Crear las tablas (No hay asignacion aun, es la Practica 1)
    crea_MT(); 
    crea_JT(); 

    // 2. Imprimir tablas finales con los datos inicializados
    imprime_MT();
    imprime_JT();

    printf("\nPresione cualquier tecla para salir...");
    getch(); 
}