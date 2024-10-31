    package calcu.calcu;

import java.util.Scanner;

class Jcalculadora {
    float[][] matriz = new float[2][4];
    float[] matriz2 = new float[4];
    Scanner entrada = new Scanner(System.in);
    public Jcalculadora() {
        boolean esParteReal = true; // Alterna entre parte real e imaginaria

        // Llenar la matriz con valores reales e imaginarios
        for (int i = 0; i < matriz.length; i++) {
            for (int j = 0; j < matriz[i].length; j++) {
                System.out.println("Introduce el valor para la posición [" + i + "][" + j + "]");

                if (esParteReal) {
                    System.err.println("Introduce la parte real");
                    float valorReal = entrada.nextFloat();
                    matriz[i][j] = valorReal;
                } else {
                    System.err.println("Introduce la parte imaginaria");
                    float valorImaginario = entrada.nextFloat();
                    matriz[i][j] = valorImaginario;
                }
                esParteReal = !esParteReal; // Cambiar entre real e imaginario
            }
        }

        esParteReal = true; 

        for (int r = 0; r < matriz2.length; r++) {
            limpiarPantalla(); // Limpiar pantalla
            if (esParteReal) {
                System.err.println("Escribe el resultado de la parte real: ");
            } else {
                System.err.println("Escribe el resultado de la parte imaginaria: ");
            }

            float resultado = entrada.nextFloat();
            matriz2[r] = resultado;
            esParteReal = !esParteReal; // Cambiar entre real e imaginario
        }
    }

    // Método para imprimir la matriz
    public void imprimirMatriz(float [][] matriz) {
        System.out.println("La matriz es:");
        boolean esParteReal = true;

        for (float[] fila : matriz) {
            System.out.print(" | ");
            for (float valor : fila) {
                System.out.print(valor + " ");
                esParteReal = !esParteReal;

                if (esParteReal) {
                    System.out.print(" | ");
                }
            }
            System.out.println();
            System.out.println("----------------------");
        }
    }

    public void imprimirMatriz2(float [] matrizA) {
        System.out.println("La matriz es:");
        for (float num : matrizA) {
            System.out.println(num);
        }
        
    }

    // Método para limpiar la pantalla
    private void limpiarPantalla() {
        System.out.print("\033[H\033[2J");
        System.out.flush();
    }

    public void calcularM() {
        float [][] matriz3 = new float [2][6];
        
        matriz3 [1][0] = ((((matriz [1][0]) * matriz[0][0]) - ((matriz[1][1])*(matriz[0][1]))) - ((matriz[1][0])*(matriz[0][0]) - ((matriz [1][1])* matriz[0][1])));
        matriz3 [1][1] = ((((matriz [1][1]) * matriz[0][0]) + ((matriz[1][0])*(matriz[0][1]))) - ((matriz[1][0])*(matriz[0][1]) + ((matriz [1][1])* matriz[0][0])));
        matriz3 [1][2] = ((((matriz [1][2]) * matriz[0][0]) - ((matriz[1][3])*(matriz[0][1]))) + ((matriz[1][0])*(matriz[0][2]) - ((matriz [1][1])* matriz[0][3])));
        matriz3 [1][3] = -((((matriz [1][3]) * matriz[0][0]) + ((matriz[1][2])*(matriz[0][1]))) - ((matriz[1][0])*(matriz[0][3]) + ((matriz [1][1])* matriz[0][2])));

        matriz3[1][4] = - ((((matriz[0][0]) * matriz2[2]) - ((matriz[0][1]) * matriz2[3])) - ((matriz[1][0] * matriz2[0]) - (matriz[1][1] * matriz2[1])));
        matriz3[1][5] = - ((((matriz[0][0]) * matriz2[3]) + ((matriz[0][1]) * matriz2[2])) - ((matriz[1][0] * matriz2[1]) - (matriz[1][1] * matriz2[0])));
        
        System.out.println(matriz3[1][0] +"+"+ matriz3[1][1]);
        System.out.println(matriz3[1][2] +"+"+ matriz3[1][3]);
        System.out.println(matriz3[1][4] +"+"+ matriz3[1][5]);
        
        float numerador1 = (matriz3 [1][4] * matriz3 [1][2]) + (matriz3[1][3] * matriz3[1][5]);
        float numerador2 = (matriz3 [1][5] * matriz3 [1][2]) - (matriz3[1][3] * matriz3[1][4]);
        float denominad1 = (matriz3 [1][2] * matriz3 [1][2]) + (matriz3[1][3] * matriz3[1][3]);

        System.out.println(numerador1/denominad1 + " + " + numerador2/denominad1);
        
    }

    // Método principal
    public static void main(String[] args) {
        Jcalculadora calculadora = new Jcalculadora();
        calculadora.limpiarPantalla();
        calculadora.imprimirMatriz(calculadora.matriz);
        calculadora.imprimirMatriz2(calculadora.matriz2);

        calculadora.calcularM();
        
    }
}