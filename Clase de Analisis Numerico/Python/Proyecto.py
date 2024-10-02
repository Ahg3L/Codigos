import os
import numpy as np

A = [
    [1, 2, 3],
    [4, 5, 6],
    [7, 8, 9]
]

def limpiar_pantalla():
    os.system('cls' if os.name == 'nt' else 'clear')

def factorizacion_lu(matriz):
    n = len(matriz)
    L = np.zeros((n, n))
    U = np.zeros((n, n))

    for k in range(n):
        L[k][k] = 1

        for i in range(k, n):
            suma = sum(L[i][p] * U[p][k] for p in range(k))
            U[i][k] = matriz[i][k] - suma

        for j in range(k + 1, n):
            suma = sum(L[k][p] * U[p][j] for p in range(k))
            L[k][j] = (matriz[k][j] - suma) / U[k][k]

    return L, U

def resolver_sistema_ecuaciones(L, U, b):
    n = len(L)
    y = np.zeros(n)
    x = np.zeros(n)

    # Resolver Ly = b
    for i in range(n):
        y[i] = b[i] - sum(L[i][j] * y[j] for j in range(i))

    # Resolver Ux = y
    for i in range(n - 1, -1, -1):
        x[i] = (y[i] - sum(U[i][j] * x[j] for j in range(i + 1, n))) / U[i][i]

    return x

def mostrar_matriz(matriz, nombre):
    print(f"\nMatriz {nombre}:")
    for fila in matriz:
        for elemento in fila:
            print(elemento, end=' ')
        print()

def CalcularMatx():
    while True:
        limpiar_pantalla()
        print("1.- Ingresar datos de la matriz")
        print("2.- Resolver el sistema de ecuaciones ")
        print("3.- Salir")
        opc = input()

        if opc == "1":
            for lista_interna in A:
                for elemento in lista_interna:
                    print(elemento, end=' ')
                print()
            
            print("Ingresa los datos\n")
            for i in range(3):
                for j in range(3):
                    A[i][j] = float(input())
        elif opc == "2":
            if not all(A[i][i] != 0 for i in range(len(A))):
                print("La matriz no es invertible. No se puede resolver el sistema.")
            else:
                b = [float(input(f"Ingrese el valor para b[{i}]: ")) for i in range(len(A))]
                L, U = factorizacion_lu(A)
                mostrar_matriz(L, "L")
                mostrar_matriz(U, "U")
                x = resolver_sistema_ecuaciones(L, U, b)
                print("\nSolución del sistema de ecuaciones:")
                for i, valor in enumerate(x):
                    print(f"x[{i}] = {valor}")
        elif opc == "3":
            print("Saliendo del programa. ¡Hasta luego!")
            break
        else:
            print("Opción no válida. Inténtalo de nuevo.")

def menu():
    limpiar_pantalla()

    while True:
        print("\nMenú:")
        print("1. Matrices")
        print("2. Sistemas de ecuaciones")

        opcion = input("Selecciona una opción: ")

        if opcion == "1":
            CalcularMatx()
        elif opcion == "2":
            print("Saliendo del programa. ¡Hasta luego!")
            break
        else:
            print("Opción no válida. Inténtalo de nuevo.")

if __name__ == "__main__":
    menu()
