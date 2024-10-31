package calcu;

import java.util.Scanner;

public class Complejo {
    private double real;
    private double imaginario;

    public Complejo(double real, double imaginario) {
        this.real = real;
        this.imaginario = imaginario;
    }

    public void setReal(double real) {
        this.real = real;
    }

    public void setimaginario(double imaginario) {
        this.imaginario = imaginario;
    }

    public double getReal() {
        return real;
    }

    public double getImagine() {
        return imaginario;
    }

    public static void setMatriz(Complejo[][] Matriz, Complejo[] MatrizResul) {
        for (int i = 0; i < 2; i++) {
            try (Scanner entrada = new Scanner(System.in)) {
                for (int j = 0; j < 2; j++) {
                    System.out.println("Introduce el valor para la posición [" + i + "][" + j + "]");
                    System.out.println("Introduce la parte real");
                    double valorReal = entrada.nextDouble();
                    Matriz[i][j] = new Complejo(valorReal, 0);
                    System.out.println("Introduce la parte imaginaria");
                    double valorImagina = entrada.nextDouble();
                    Matriz[i][j].setimaginario(valorImagina);
                }
                System.out.println("Introduce el primer resultado");
                System.out.println("Escrive la parte real");
                double valorReal = entrada.nextDouble();
                System.out.println("Escrive la parte imaginaria");
                double valorImaginario = entrada.nextDouble();
                MatrizResul[i] = new Complejo(valorReal, valorImaginario);
            }
        }
    }

    public static void getMatriz(Complejo[][] Matriz, Complejo[] MatrizResul) {
        System.out.println("La matriz es:");
        System.out.println("(" + Matriz[0][0].getReal() + "+" + Matriz[0][0].getImagine() + "i) + ("
                + Matriz[0][1].getReal() + "+" + Matriz[0][1].getImagine() + "i) = "
                + MatrizResul[0].getReal() + "+" + MatrizResul[0].getImagine() + "i");
        System.out.println("(" + Matriz[1][0].getReal() + "+" + Matriz[1][0].getImagine() + "i) + ("
                + Matriz[1][1].getReal() + "+" + Matriz[1][1].getImagine() + "i) = "
                + MatrizResul[1].getReal() + "+" + MatrizResul[1].getImagine() + "i");
    }
    
    public static void limpiarPantalla() {
        System.out.print("\033[H\033[2J");
        System.out.flush();
    }

    public static Complejo dividir(Complejo z1, Complejo z2) {
        double a = z1.getReal();
        double b = z1.getImagine();
        double c = z2.getReal();
        double d = z2.getImagine();
        double numeradorReal = (a * c + b * d);
        double numeradorImaginario = (b * c - a * d);
        double denominador = c * c + d * d;
        if (denominador == 0) {
            throw new ArithmeticException("División por cero en números complejos");
        }
        double parteReal = numeradorReal / denominador;
        double parteImaginaria = numeradorImaginario / denominador;
        return new Complejo(parteReal, parteImaginaria);
    }

    public static Complejo multiplicar(Complejo z1, Complejo z2) {
        double a = z1.getReal();
        double b = z1.getImagine();
        double c = z2.getReal();
        double d = z2.getImagine();
        double parteReal = (a * c) - (b * d);
        double parteImaginaria = (a * d) + (b * c);
        return new Complejo(parteReal, parteImaginaria);
    }

    public static Complejo restar(Complejo z1, Complejo z2) {
        double parteReal = z1.getReal() - z2.getReal();
        double parteImaginaria = z1.getImagine() - z2.getImagine();
        return new Complejo(parteReal, parteImaginaria);
    }    

   public static void resolver(Complejo[][] Matriz, Complejo[] MatrizResul) {
    Complejo[] arreglo = new Complejo[6];

    arreglo[0] = restar((multiplicar(Matriz [0][0], Matriz[1][0])),(multiplicar(Matriz[1][0], Matriz[0][0])));
    arreglo[1] = restar((multiplicar(Matriz [0][0], Matriz[1][1])),(multiplicar(Matriz[1][0], Matriz[0][1])));
    arreglo[2] = restar((multiplicar(Matriz[1][0], MatrizResul[0])),(multiplicar(Matriz [0][0], MatrizResul[1])));
    
    Matriz[1][0] = arreglo[0];
    Matriz[1][1] = arreglo[1];
    MatrizResul[1] =arreglo[2];
    getMatriz(Matriz, MatrizResul);
    Complejo resultado = dividir(MatrizResul[1], Matriz[1][1]);
    System.out.println("L2 = " +resultado.getReal() + " | " + resultado.getImagine());
}   

    public static void main(String[] args) {
        Complejo[][] Matriz = new Complejo[2][3];
        Complejo[] MatrizResul = new Complejo[2];

        Matriz[0][0] = new Complejo(0, -1);
        Matriz[0][1] = new Complejo(2,  1);
        Matriz[1][0] = new Complejo(-1, 0);
        Matriz[1][1] = new Complejo(2, 0);
        MatrizResul[0] = new Complejo(4,0);
        MatrizResul[1] = new Complejo(8, 0);
        limpiarPantalla();
        getMatriz(Matriz, MatrizResul);
        resolver(Matriz, MatrizResul);
    }

}
