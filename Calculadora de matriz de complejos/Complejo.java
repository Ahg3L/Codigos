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

    public static Complejo restar(Complejo z1, Complejo z2) {
        return new Complejo(z1.getReal() - z2.getReal(), z1.getImagine() - z2.getImagine());
    }

    public static Complejo sumar(Complejo z1, Complejo z2) {
        return new Complejo(z1.getReal() + z2.getReal(), z1.getImagine() + z2.getImagine());
    }

    public static Complejo multiplicar(Complejo z1, Complejo z2) {
        return new Complejo((z1.real * z2.real) - (z1.imaginario * z2.imaginario),
                            (z1.real * z2.imaginario) + (z1.imaginario * z2.real));
    }

    public static Complejo dividir(Complejo z1, Complejo z2) {
        double denominador = z2.real * z2.real + z2.imaginario * z2.imaginario;
        return new Complejo((z1.real * z2.real + z1.imaginario * z2.imaginario)/denominador, 
                            (z1.imaginario * z2.real - z1.real * z2.imaginario)/denominador);
    }

    public static void limpiarPantalla() {
        System.out.print("\033[H\033[2J");
        System.out.flush();
    }

    public static void getMatrizX(Complejo[][] A, Complejo[] R) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                System.out.print("(" + A[i][j].getReal() + "+" + A[i][j].getImagine() + "i) ");
            }
            System.out.println("= " + R[i].getReal() + "+" + R[i].getImagine() + "i");
        }
        System.out.println();
    }

    public static void setMatriz(Complejo[][] M, Complejo[] Re) {
        try (Scanner entrada = new Scanner(System.in)) {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    limpiarPantalla();
                    getMatrizX(M, Re);
                    System.out.println("Introduce el valor para la posición [" + i + "][" + j + "]");
                    System.out.println("Introduce la parte real");
                    double valorReal = entrada.nextDouble();
                    M[i][j]=new Complejo(valorReal,0);
                    System.out.println("Introduce la parte imaginaria");
                    double valorImagina = entrada.nextDouble();
                    M[i][j].setimaginario(valorImagina);
                }
                limpiarPantalla();
                getMatrizX(M, Re);
                System.out.println("¿Tu resultado es Polar o imaginaria ?");
                System.out.println("1.-Numero Polar.");
                System.out.println("2.-Numero Imaginario.");
                double valor = entrada.nextDouble();

                System.out.println("Introduce el resultado para la fila " + i);
                System.out.println("Introduce el primer numero");
                double valorReal = entrada.nextDouble();
                
                System.out.println("Introduce el segundo numero");
                double valorImaginario = entrada.nextDouble();

                if (valor == 1) {
                    // n1 = módulo, n2 = ángulo en radianes
                    double parteReal = valorReal * Math.cos(valorImaginario);
                    double parteImag = valorReal * Math.sin(valorImaginario);
                    Re[i] = new Complejo(parteReal, parteImag);
                } else {
                    Re[i] = new Complejo(valorReal, valorImaginario);
                }
            }
        }
    }

    public static void resolver(Complejo[][] MatrizA, Complejo[] MatrizR) {
        
        Complejo [][] MatrizAux = new Complejo[3][3];
        Complejo [] RespuAux = new Complejo[3];
        Complejo x;
        Complejo y;
        Complejo z;

        for(int j=1;j<3;j++){
            for(int i =0;i<3;i++){
                x = multiplicar(MatrizA[0][0], MatrizA[j][i]);
                y = multiplicar(MatrizA[j][0], MatrizA[0][i]);
                z =restar(x, y);
                MatrizAux[j][i] = new Complejo(z.real, z.imaginario);
            }
                x = multiplicar(MatrizA[0][0], MatrizR[j]);
                y = multiplicar(MatrizA[j][0], MatrizR[0]);
                z =restar(x, y);
                RespuAux [j] = new Complejo(z.real, z.imaginario);
        }

        MatrizA[1][0] = MatrizAux[1][0];
        MatrizA[1][1] = MatrizAux[1][1];
        MatrizA[1][2] = MatrizAux[1][2];
        MatrizR[1] = RespuAux  [1];
        getMatrizX(MatrizA, MatrizR);
        MatrizA[2][0] = MatrizAux[2][0];
        MatrizA[2][1] = MatrizAux[2][1];
        MatrizA[2][2] = MatrizAux[2][2];
        MatrizR[2]= RespuAux[2];
        getMatrizX(MatrizA, MatrizR);

        for(int i=1;i<3;i++){
            x = multiplicar(MatrizA[1][1], MatrizA[2][i]);
            y = multiplicar(MatrizA[2][1], MatrizA[1][i]);
            z =restar(x, y);
            MatrizAux[2][i] = new Complejo(z.real, z.imaginario);
        }

        x = multiplicar(MatrizA[1][1], MatrizR[2]);
        y = multiplicar(MatrizA[2][1], MatrizR[1]);
        z =restar(x, y);
        RespuAux[2] = new Complejo(z.real, z.imaginario);

        MatrizA[2][0] = MatrizAux[2][0];
        MatrizA[2][1] = MatrizAux[2][1];
        MatrizA[2][2] = MatrizAux[2][2];
        MatrizR[2]= RespuAux[2];
        
        Complejo I3 = dividir(MatrizR[2], MatrizA[2][2]);
        Complejo I2 = dividir(restar(MatrizR[1], multiplicar(I3, MatrizA[1][2])), MatrizA[1][1]);
        Complejo I1 = dividir(restar(restar(MatrizR[0], multiplicar(I2, MatrizA[0][1])),
        multiplicar(I3, MatrizA[0][2])), MatrizA[0][0]);

        getMatrizX(MatrizA, MatrizR);

        System.out.println("I3 = " + I3.getReal() + " + " + I3.getImagine() + "i");
        System.out.println("I2 = " + I2.getReal() + " + " + I2.getImagine() + "i");
        System.out.println("I1 = " + I1.getReal() + " + " + I1.getImagine() + "i");

    }
    
    public static void imprimirResultadosPolares(Complejo[] resultados) {
        System.out.println("Resultados en forma polar:");
        for (int i = 0; i < resultados.length; i++) {
            Complejo c = resultados[i];
            double modulo = Math.sqrt(c.getReal() * c.getReal() + c.getImagine() * c.getImagine());
            double anguloRadianes = Math.atan2(c.getImagine(), c.getReal());
            double anguloGrados = Math.toDegrees(anguloRadianes);
            
            System.out.printf("I%d = %.4f < %.2f°%n", i + 1, modulo, anguloGrados);
        }
    }
    

    public static void main(String[] args) {
        Complejo[][] Matriz = new Complejo[3][3];
        Complejo [] MatrizResul = new Complejo[3];

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                Matriz[i][j] = new Complejo(0, 0); 
            }
            MatrizResul[i] = new Complejo(0, 0);
        }
    
        setMatriz(Matriz,MatrizResul);
        limpiarPantalla();

        getMatrizX(Matriz, MatrizResul);
        resolver(Matriz, MatrizResul);

        imprimirResultadosPolares(MatrizResul);
    }
}