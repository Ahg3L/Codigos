/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package com.mycompany.fractus;

import java.awt.Canvas;
import java.awt.Graphics;

/**
 *
 * @author Lab.Computacion
 */

public class lienzo extends Canvas{
    private int longitud = 100;
    int iteraciones = 1;
    public lienzo() {
        setSize(1000, 700);
    }
    
    public void setParametros(int longitud, int iteraciones) { 
        this.longitud = longitud;
        this.iteraciones = iteraciones; 
        repaint();
    }
    
    
    @Override 
    
    public void paint(Graphics g) {
        super.paint(g);

        g.drawString("Longitud inicial de la arista : " , 20, 20);
        g.drawString("Cantidad de iteraciones : " , 20, 50);

        // centramos el fractal
        int x = (getWidth() / 2) - (longitud / 2);
        int y = (getHeight() / 2) - (longitud / 2);

        dibujarFractal(g, x, y, longitud, iteraciones);
    }

    private void dibujarFractal(Graphics g, int x, int y, int lado, int it) {

        // condiciones de parada
        if (it <= 0 || lado < 3) {
            return;
        }

        g.drawRect(x, y, lado, lado);
        int nuevoLado = lado / 3;

        dibujarFractal(g, x - (nuevoLado / 2), y - (nuevoLado / 2), nuevoLado, it - 1);
        dibujarFractal(g, x + lado - (nuevoLado / 2), y - (nuevoLado / 2), nuevoLado, it - 1);
        dibujarFractal(g, x - (nuevoLado / 2), y + lado - (nuevoLado / 2), nuevoLado, it - 1);
        dibujarFractal(g, x + lado - (nuevoLado / 2), y + lado - (nuevoLado / 2), nuevoLado, it - 1);
    }
}

