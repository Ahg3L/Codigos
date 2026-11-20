#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>

using namespace std;

// ==========================================
// --- ARREGLO DE ESTRUCTURAS PARA LA PMT ---
// ==========================================
struct ElementoPMT
{
    int pagina;
    int marco;
};

struct PMT
{
    string id_tarea;
    vector<ElementoPMT> filas; // Arreglo de estructuras para el mapeo
};

struct Tarea
{
    string id;
    int tamano;
    string estado; // "..", "Elim.", "Espera", "Term."
    PMT pmt;       // Estructura PMT embebida en cada tarea
};

struct Marco
{
    int id;
    int loc_ini;
    int loc_fin;
    int estado;    // 0 = Libre, 1 = Ocupado
    string acceso; // Indica Tarea y P�gina (Ej: J1-P0)
};

// ==========================================
// --- FUNCIONES DE IMPRESI�N ---
// ==========================================
void imprimir_tareas(const vector<Tarea> &tareas)
{
    cout << "\n========================================\n";
    cout << "          TABLA DE TAREAS (JT)          \n";
    cout << "========================================\n";
    cout << left << setw(10) << "Tarea" << setw(10) << "Tamano" << "Estado\n";
    cout << "----------------------------------------\n";
    for (const auto &t : tareas)
    {
        cout << left << setw(10) << t.id
             << setw(10) << (to_string(t.tamano) + " K")
             << t.estado << "\n";
    }
}

void imprimir_memoria(const vector<Marco> &memoria)
{
    cout << "\n=========================================================\n";
    cout << "          MAPA DE MEMORIA / MARCOS (MHT)                 \n";
    cout << "=========================================================\n";
    cout << left << setw(10) << "Marco"
         << setw(12) << "Loc Ini"
         << setw(12) << "Loc Fin"
         << setw(8) << "Edo"
         << "Acceso (Tarea-Pag)\n";
    cout << "---------------------------------------------------------\n";
    for (const auto &m : memoria)
    {
        cout << left << setw(10) << ("M" + to_string(m.id))
             << setw(12) << (to_string(m.loc_ini) + " K")
             << setw(12) << (to_string(m.loc_fin) + " K")
             << setw(8) << m.estado
             << m.acceso << "\n";
    }
    cout << "=========================================================\n";
}

void imprimir_todas_las_pmt(const vector<Tarea> &tareas)
{
    cout << "\n========================================\n";
    cout << "      TABLAS DE MAPEO DE PAGINAS (PMT)  \n";
    cout << "========================================\n";

    bool alguna_activa = false;
    for (const auto &t : tareas)
    {
        // Solo imprimimos la PMT de las tareas que est�n cargadas en memoria
        if (t.estado == "Elim." && !t.pmt.filas.empty())
        {
            alguna_activa = true;
            cout << " PMT de la Tarea: " << t.pmt.id_tarea << "\n";
            cout << "---------------------\n";
            cout << left << setw(10) << "Pagina" << "Marco\n";
            cout << "---------------------\n";
            for (const auto &fila : t.pmt.filas)
            {
                cout << left << setw(10) << fila.pagina
                     << ("M" + to_string(fila.marco)) << "\n";
            }
            cout << "---------------------\n\n";
        }
    }
    if (!alguna_activa)
    {
        cout << "[ No hay tareas activas en memoria con PMT ]\n========================================\n";
    }
}

// ==========================================
// --- L�GICA PRINCIPAL ---
// ==========================================
int main()
{
    srand(time(0)); // Semilla para los n�meros aleatorios

    // Configuraci�n de rangos requerida por la maestra
    const int MIN_TAM = 5;
    const int MAX_TAM = 45;

    int memoria_total, tamano_pagina, num_tareas;

    cout << "=== SIMULADOR DE PAGINACION ===\n\n";
    cout << "Ingrese el tamano total de la memoria (K): ";
    cin >> memoria_total;
    cout << "Ingrese el tamano de Pagina / Marco (K): ";
    cin >> tamano_pagina;
    cout << "Ingrese la cantidad de tareas a generar: ";
    cin >> num_tareas;

    // 1. Mostrar rango en pantalla tal como se solicita
    cout << "\n--------------------------------------------------\n";
    cout << "[ CONFIG ] Rango de tamano para tareas: [" << MIN_TAM << " K - " << MAX_TAM << " K]\n";
    cout << "--------------------------------------------------\n";

    // 2. Generaci�n aleatoria de la Tabla de Tareas (JT)
    vector<Tarea> lista_tareas;
    for (int i = 1; i <= num_tareas; i++)
    {
        Tarea t;
        t.id = "J" + to_string(i);
        t.tamano = rand() % (MAX_TAM - MIN_TAM + 1) + MIN_TAM; // Tama�o aleatorio en rango
        t.estado = "..";
        t.pmt.id_tarea = t.id; // Inicializamos su estructura PMT interna
        lista_tareas.push_back(t);
    }

    // 3. Inicializaci�n de marcos de memoria (MHT)
    int total_marcos = memoria_total / tamano_pagina;
    vector<Marco> lista_memoria;
    for (int i = 0; i < total_marcos; i++)
    {
        Marco m;
        m.id = i;
        m.loc_ini = i * tamano_pagina;
        m.loc_fin = ((i + 1) * tamano_pagina) - 1;
        m.estado = 0;
        m.acceso = "Libre";
        lista_memoria.push_back(m);
    }

    int ciclo = 1;
    bool simulacion_activa = true;

    // --- CICLO INFINITO DE SIMULACI�N ---
    while (simulacion_activa)
    {
        cout << "\n\n*********************************************************\n";
        cout << "                       CICLO " << ciclo << "\n";
        cout << "*********************************************************\n";

        // Intentar asignar tareas pendientes
        for (auto &t : lista_tareas)
        {
            if (t.estado == ".." || t.estado == "Espera")
            {
                // C�lculo de p�ginas necesarias (Techo de la divisi�n)
                int paginas_requeridas = (t.tamano + tamano_pagina - 1) / tamano_pagina;

                // Contar cu�ntos marcos libres hay en total (No contiguos)
                int marcos_libres = 0;
                for (const auto &m : lista_memoria)
                {
                    if (m.estado == 0)
                        marcos_libres++;
                }

                // Si hay suficientes marcos, se realiza la asignaci�n NO CONTIGUA
                if (marcos_libres >= paginas_requeridas)
                {
                    int pag_actual = 0;

                    for (auto &m : lista_memoria)
                    {
                        if (m.estado == 0 && pag_actual < paginas_requeridas)
                        {
                            // Ocupamos el marco
                            m.estado = 1;
                            m.acceso = t.id + "-P" + to_string(pag_actual);

                            // Guardamos en el arreglo de estructuras PMT de la tarea
                            ElementoPMT fila;
                            fila.pagina = pag_actual;
                            fila.marco = m.id;
                            t.pmt.filas.push_back(fila);

                            pag_actual++;
                        }
                    }
                    t.estado = "Elim."; // Asignada con �xito
                    cout << "-> " << t.id << " asignada de forma NO contigua (" << paginas_requeridas << " pags).\n";
                }
                else
                {
                    t.estado = "Espera";
                }
            }
        }

        // Desplegar el estado actual de todas las estructuras
        imprimir_tareas(lista_tareas);
        imprimir_memoria(lista_memoria);
        imprimir_todas_las_pmt(lista_tareas);

        // Validar si ya finalizaron todas las tareas
        bool todas_terminadas = true;
        for (const auto &t : lista_tareas)
        {
            if (t.estado != "Term.")
                todas_terminadas = false;
        }

        if (todas_terminadas)
        {
            cout << "\n[!] Todas las tareas han sido procesadas y finalizadas. Fin del programa.\n";
            break;
        }

        // Control de eventos por teclado
        cout << "\nIngrese el ID de una tarea para liberarla (ej. J1) o 'n' para continuar: ";
        string entrada;
        cin >> entrada;

        if (entrada != "n")
        {
            bool encontrada = false;
            for (auto &t : lista_tareas)
            {
                if (t.id == entrada && t.estado == "Elim.")
                {
                    t.estado = "Term.";

                    // Liberar los marcos asociados inspeccionando su PMT
                    for (const auto &fila : t.pmt.filas)
                    {
                        lista_memoria[fila.marco].estado = 0;
                        lista_memoria[fila.marco].acceso = "Libre";
                    }
                    // Limpiamos su PMT ya que sali� de memoria f�sica
                    t.pmt.filas.clear();

                    cout << "-> Tarea " << entrada << " terminada. Marcos liberados.\n";
                    encontrada = true;
                    break;
                }
            }
            if (!encontrada)
                cout << "-> Tarea invalida o no se encuentra activa en memoria.\n";
        }

        ciclo++;
    }

    return 0;
}
