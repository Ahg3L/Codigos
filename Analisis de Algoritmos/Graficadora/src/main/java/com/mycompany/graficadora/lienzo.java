/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package com.mycompany.graficadora;
import java.awt.Canvas;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;

/**
 *
 * @author Ahg3L
 */
public class lienzo extends Canvas {

    public lienzo() {
        this.setSize(new Dimension(890,620));
        Color ncolor = new Color(230, 230, 230);
        this.setBackground(ncolor);
    }
    
    public static double f(double x) {
    return 2.5 * Math.log10(Math.pow(x, 3)) 
           -.33 * Math.pow(x, 5) + 2;
    }
    

@Override    
    public void paint(Graphics g){
        int ancho = getWidth();
        int alto = getHeight();
        int origenX = 100;
        int origenY = alto / 2;

        double escalaX = 40;  // ancho
        double escalaY = 20;  // alto

        g.setColor(getBackground());
        g.fillRect(0, 0,ancho,alto);
        g.setColor(Color.BLACK);
        g.drawLine(0, origenY, ancho, origenY);
        g.drawLine(origenX, 0, origenX, alto);
        g.setColor(Color.BLUE);

        double xMin = 0.01;
        double xMax = 15;
        double paso = .01;
        int xPrev = 0;
        int yPrev = 0;
        boolean primero = true;
        
        for (double x=xMin; x <= xMax; x += paso) {
            double y= f(x);

            int xp = (int)(origenX + x * escalaX);
            int yp = (int)(origenY - y * escalaY); 

            if (!primero) {
                g.drawLine(xPrev, yPrev, xp, yp);
            }
            xPrev = xp;
            yPrev = yp;
            primero = false;
        }
        
        for (int i =-100;i<=100; i++) {
            int x = (int)(origenX + i * escalaX);
            if ( x <= ancho) {
                g.drawLine(x,origenY - 5,x,origenY +5);
            }
        }    
        for (int i= -100; i<=100;i++) {
            int y =(int)(origenY - i * escalaY);
            if ( y <=alto) {
            g.drawLine(origenX -5, y, origenX + 5, y);
            }
        }
        g.setFont(new Font("Arial", Font.PLAIN, 11));     
    }
}        
    
   
