/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package com.mycompany.graficadora2;

import java.awt.Canvas;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;

/**
 *
 * @author Ahg3L
 */
public class lienzo2 extends Canvas {

    public lienzo2() {
        this.setSize(new Dimension(890, 620));
        Color ncolor = new Color(230, 230, 230);
        this.setBackground(ncolor);
    }
    
    // Función: y = 2x^3 + 5x^2 - 2x - 6
    public double f(double x) {
        return 2 * Math.pow(x, 3) + 5 * Math.pow
        (x, 2) - 2 * x - 6;
    }
    
    @Override
    public void paint(Graphics g) {

        int ancho = getWidth();
        int alto = getHeight();

        int origenX = (int)(ancho*.65);
        int origenY = alto /2;

        double escalaX = 50;
        double escalaY = 12;

        g.setColor(getBackground());
        g.fillRect(0, 0, ancho, alto);
        g.setColor(Color.BLACK);
        g.drawLine(0, origenY, ancho, origenY);
        g.drawLine(origenX, 0, origenX, alto);

        g.setColor(Color.BLUE);

        double xMin = -15;
        double xMax = 8;
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

        for (int i = -20;i <= 30;i++){
            int y = (int) (origenY - i*escalaY);
            if (y >= 0 && y <= alto){
                g.drawLine(origenX - 5, y,origenX + 5, y);
                g.drawString(Integer.toString(i),origenX+ 8,y+ 4); 
                }
            }

        for (int i= -20;i <= 20; i++){
            int x=(int) (origenX + i * escalaX);
            if (x>=0&&x <= ancho) {
                g.drawLine(x,origenY- 5, x,origenY + 5); 
                g.drawString(Integer.toString(i), x - 5,origenY+ 15); 
            }
        }
       
        
        g.setFont(new Font("Arial", Font.PLAIN, 10));

    }    
}
