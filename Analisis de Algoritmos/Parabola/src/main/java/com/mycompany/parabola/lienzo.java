/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package com.mycompany.parabola;
import java.awt.Canvas;
import java.awt.Graphics;

/**
 *
 * @author Ahg3L
 */

public class lienzo extends Canvas{
  
    double theta ;
    double velocidad;
    double escalaX;
    double escalaY;
    
    public lienzo() {
        setSize(1000, 600);
    }
    
    public void setParametros(double velocidad, double theta){ 
        this.velocidad = velocidad;
        this.theta = theta; 
        repaint();
    }
      
    @Override 
    
    public void paint(Graphics g) { 
    super.paint(g);

        double thetaRad = Math.toRadians(theta);

        double vx = velocidad * Math.cos(thetaRad);
        double vy = velocidad * Math.sin(thetaRad);

        double t_total = (2 * vy) / 9.81;

        double alcance = (Math.pow(velocidad, 2) * Math.sin(2 * thetaRad)) / 9.81;
        double altura = (Math.pow(vy, 2)) / (2 * 9.81);

        // calcular escalas
        calcularEscalas(alcance, altura);

        g.drawString("Velocidad inicial (m/s): " , 20, 30);
        g.drawString("Theta (°): " , 20, 50);
        g.drawString("Alcance: " + String.format("%.2f", alcance) + " m", 20, 70);
        g.drawString("Altura maxima: " + String.format("%.2f", altura) + " m", 20, 90);

        dibujarEjes(g, alcance, altura);
        dibujarParabola(g, vx, vy, t_total);
    }

    public void calcularEscalas(double alcance, double altura) {

        int anchoDisponible = 800;
        int altoDisponible = 400;

        if(altura < alcance * 0.25){
            altura = alcance * 0.25;
        }

        escalaX = anchoDisponible / alcance;
        escalaY = altoDisponible / altura;
    }

    public void dibujarEjes(Graphics g, double alcance, double altura) {

        int origenX = 60;
        int origenY = 500;

        int ejeX = origenX + (int) (alcance * escalaX);
        int ejeY = origenY - (int) (altura * escalaY);

        g.drawLine(origenX, origenY, ejeX, origenY);
        g.drawLine(origenX, origenY, origenX, ejeY);

        g.drawString("X", ejeX + 10, origenY + 10);
        g.drawString("Y", origenX - 10, ejeY - 10);

        dibujarEscalaX(g, alcance, origenX, origenY);
        dibujarEscalaY(g, altura, origenX, origenY);
    }

    public void dibujarEscalaX(Graphics g, double alcance, int origenX, int origenY) {

        int divisiones = 10;
        double paso = alcance / divisiones;

        for (int i = 1; i <= divisiones; i++) {

            int x = origenX + (int) (i * paso * escalaX);

            g.drawLine(x, origenY - 5, x, origenY + 5);

            g.drawString(String.format("%.1f", i * paso), x - 10, origenY + 20);
        }
    }

    public void dibujarEscalaY(Graphics g, double altura, int origenX, int origenY) {

        int divisiones = 10;
        double paso = altura / divisiones;

        for (int i = 1; i <= divisiones; i++) {

            int y = origenY - (int) (i * paso * escalaY);

            g.drawLine(origenX - 5, y, origenX + 5, y);

            g.drawString(String.format("%.1f", i * paso), origenX - 45, y + 5);
        }
    }

    public void dibujarParabola(Graphics g, double vx, double vy, double t_total) {

        int origenX = 60;
        int origenY = 500;

        int xAnt = origenX;
        int yAnt = origenY;

        for (double t = 0; t <= t_total; t += 0.05) {

            double x = vx * t;
            double y = vy * t - (0.5 * 9.81 * t * t);

            int xCanvas = origenX + (int) (x * escalaX);
            int yCanvas = origenY - (int) (y * escalaY);

            g.drawLine(xAnt, yAnt, xCanvas, yCanvas);

            xAnt = xCanvas;
            yAnt = yCanvas;
        }
    }
}