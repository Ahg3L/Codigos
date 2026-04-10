/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package esime.grafica01;

import java.awt.Canvas;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;

/**
 *
 * @author Ahg3L
 */

public class lienzo extends Canvas{
    public lienzo() {
        this.setSize(new Dimension(890,620));
        Color ncolor = new Color(230, 230, 230);
        this.setBackground(ncolor);
        
        double resultado = integral(0.1, 3, 1000);
        System.out.println("Integral numerica = " + resultado);
    }
     
public double f(double x) {
    return 2.5 * Math.log(Math.pow(x, 3)) 
           - 0.33 * Math.pow(x, 5) 
           + 2;
}

public double integral(double a, double b, int n) {
    double h = (b - a) / n;
    double suma = 0;

    for (int i = 1; i < n; i++) {
        double x = a + i * h;
        suma += f(x);
    }

    return h * ((f(a) + f(b)) / 2 + suma);
}

@Override    
    public void paint(Graphics g){
        int ancho = getWidth();
        int alto = getHeight();
        int origenX = 50;
        int origenY = alto / 2;

        double escalaX = 80;  // qué tan “abierta” la gráfica
        double escalaY = 20;  // qué tan “alta”

        // fondo
        g.setColor(getBackground());
        g.fillRect(0, 0,ancho,alto);

        // ejes
        g.setColor(Color.BLACK);

        // eje x (horizontal)
        g.drawLine(50, alto/2, ancho-50, alto/2);

        // eje y (vertical)
        g.drawLine(50,50,50, alto-50);
        
        
        
        g.setColor(Color.BLUE);

    double xMin = 0.1;
    double xMax = 3;
    double paso = 0.01;

    int xPrev = 0;
    int yPrev = 0;
    boolean primero = true;

    for (double x = xMin; x <= xMax; x += paso) {
        double y = f(x);

        int xp = (int) (origenX + x * escalaX);
        int yp = (int) (origenY - y * escalaY); 

        if (!primero) {
            g.drawLine(xPrev, yPrev, xp, yp);
        }

        xPrev = xp;
        yPrev = yp;
        primero = false;
    }
    }
}
