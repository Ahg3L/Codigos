#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

// --- ESTRUCTURAS ---
struct Tarea {
    string id;
    int tamano;
    string estado; // "Elim." (Asignada), "Espera", o ".." (Sin procesar)
};

struct Bloque {
    string id;
    int capacidad;
    int loc_ini;
    int loc_fin;
    int estado;     // 0 según tu pizarrón
    string acceso;  // Qué tarea u SO está adentro
    string fe;      // Fragmentación Interna
};

// --- FUNCIONES ---

void imprimir_tareas(const vector<Tarea>& tareas) {
    cout << "\n========================================\n";
    cout << "          TABLA DE TAREAS (JT)          \n";
    cout << "========================================\n";
    cout << left << setw(10) << "Tarea" << setw(10) << "Tamano" << "Estado\n";
    cout << "----------------------------------------\n";
    for (const auto& t : tareas) {
        cout << left << setw(10) << t.id 
             << setw(10) << (to_string(t.tamano) + " K") 
             << t.estado << "\n";
    }
}

void imprimir_memoria(const vector<Bloque>& memoria) {
    cout << "\n=========================================================\n";
    cout << "               TABLA DE MEMORIA                          \n";
    cout << "=========================================================\n";
    cout << left << setw(8) << "Bloque" 
         << setw(12) << "Capacidad" 
         << setw(10) << "Loc Ini" 
         << setw(10) << "Loc Fin" 
         << setw(6) << "Edo" 
         << setw(10) << "Acceso" 
         << "F.E.\n";
    cout << "---------------------------------------------------------\n";
    for (const auto& b : memoria) {
        cout << left << setw(8) << b.id 
             << setw(12) << (to_string(b.capacidad) + " K") 
             << setw(10) << (to_string(b.loc_ini) + " K") 
             << setw(10) << (to_string(b.loc_fin) + " K") 
             << setw(6) << b.estado 
             << setw(10) << b.acceso 
             << b.fe << "\n";
    }
    cout << "=========================================================\n";
}

void asignar_memoria(vector<Tarea>& tareas, vector<Bloque>& memoria, int memoria_total) {
    // Calculamos cuánto espacio queda (Le restamos el SO que ya debe estar en la memoria)
    int memoria_disponible = memoria_total - memoria[0].capacidad; 
    int loc_actual = memoria[0].loc_fin + 1; // Empezamos a asignar después del SO
    int num_bloque = 1;

    cout << "\nIniciando asignacion (Memoria disponible: " << memoria_disponible << " K)...\n";

    for (size_t i = 0; i < tareas.size(); i++) {
        // Si ya no hay memoria en absoluto, todas las demás tareas se quedan en espera
        if (memoria_disponible == 0) {
            tareas[i].estado = "Espera";
            continue;
        }

        // Verificamos si la tarea cabe en la memoria que nos sobra
        if (tareas[i].tamano <= memoria_disponible) {
            // 1. Creamos el bloque a la medida exacta
            Bloque nuevo_bloque;
            nuevo_bloque.id = "B" + to_string(num_bloque);
            nuevo_bloque.capacidad = tareas[i].tamano;
            nuevo_bloque.loc_ini = loc_actual;
            nuevo_bloque.loc_fin = loc_actual + tareas[i].tamano - 1;
            nuevo_bloque.estado = 0;
            nuevo_bloque.acceso = tareas[i].id;
            nuevo_bloque.fe = "0 K"; // Como es dinámico, siempre es 0

            // 2. Lo agregamos a la tabla de memoria
            memoria.push_back(nuevo_bloque);

            // 3. Actualizamos las variables de control
            memoria_disponible -= tareas[i].tamano;
            loc_actual += tareas[i].tamano;
            num_bloque++;

            // 4. Marcamos la tarea como asignada (Elim. según el pizarrón)
            tareas[i].estado = "Elim.";
            
            cout << "-> Asignada " << tareas[i].id << " | Memoria restante: " << memoria_disponible << " K\n";
        } else {
            // No cabe, se queda esperando
            tareas[i].estado = "Espera";
            cout << "-> " << tareas[i].id << " NO CABE. Pasa a Espera.\n";
        }
    }
    
    if (memoria_disponible == 0) {
        cout << "\n[!] ALERTA: La memoria se ha llenado por completo (0 K disponibles).\n";
    }
}

// --- FUNCION PRINCIPAL ---
int main() {
    int MEMORIA_MAXIMA = 200;

    // 1. Definimos las tareas como en el pizarrón
    vector<Tarea> lista_tareas = {
        {"J1",  25, ".."},
        {"J2",  15, ".."},
        {"J3",  40, ".."},
        {"J4",  35, ".."},
        {"J5",  10, ".."},
        {"J6",  45, ".."},
        {"J7",  20, ".."},
        {"J8",  15, ".."},
        {"J9",  10, ".."},
        {"J10", 5,  ".."}
    };

    // 2. Inicializamos la memoria SOLO con el Sistema Operativo
    vector<Bloque> lista_memoria;
    Bloque so = {"B0", 20, 0, 19, 0, "S.O.", "0 K"};
    lista_memoria.push_back(so);

    // 3. Ejecutamos la asignación dinámica
    asignar_memoria(lista_tareas, lista_memoria, MEMORIA_MAXIMA);

    // 4. Imprimimos los resultados con las funciones separadas
    imprimir_tareas(lista_tareas);
    imprimir_memoria(lista_memoria);

    return 0;
}