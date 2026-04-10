/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package com.mycompany.evalfuncion;
import java.awt.Canvas;
import java.awt.Graphics;

/**
 *
 * @author Ahg3L
 */

public class lienzo extends Canvas {
    double a;
    double b;
    double c;
    public lienzo() {
         setSize(600, 400);
    }
    
    public void setParametros(double a, double b, double c){ 
        this.a = a;
        this.b = b; 
        this.c = c; 
        repaint();
    }
    
    @Override
    
    public void paint(Graphics g){

        super.paint(g);

        double xmin = -10;
        double xmax = 10;

        double ymin = Double.MAX_VALUE;
        double ymax = -Double.MAX_VALUE;

        for(double x=xmin;x<=xmax;x+=0.1){

            double y = a*x*x + b*x + c;

            if(y<ymin) ymin=y;
            if(y>ymax) ymax=y;
        }

        double escalaX = calcularEscalaX(xmin,xmax);
        double escalaY = calcularEscalaY(ymin,ymax);

        g.drawString("Funcion: y = ax² + bx + c",20,20);
        g.drawString("a = "+a,20,40);
        g.drawString("b = "+b,20,60);
        g.drawString("c = "+c,80,40);
        g.drawString("Ingresa Ax^2 : " , 460, 115);
        g.drawString("Ingresa Bx : " , 460, 140);
        g.drawString("Ingresa C : " , 460, 165);
        
        dibujarEjes(g,xmin,xmax,ymin,ymax,escalaX,escalaY);
        dibujarFuncion(g,xmin,xmax,ymin,escalaX,escalaY);
    }


    public double calcularEscalaX(double xmin,double xmax){

        int anchoDisponible = 500;

        return anchoDisponible/(xmax-xmin);
    }

    public double calcularEscalaY(double ymin,double ymax){

        int altoDisponible = 250;

        return altoDisponible/(ymax-ymin);
    }


    public void dibujarEjes(Graphics g,double xmin,double xmax,double ymin,double ymax,double escalaX,double escalaY){

        int origenX = 50;
        int origenY = 320;

        int ejeX = origenX + (int)((xmax-xmin)*escalaX);
        int ejeY = origenY - (int)((ymax-ymin)*escalaY);

        g.drawLine(origenX,origenY,ejeX,origenY);
        g.drawLine(origenX,origenY,origenX,ejeY);

        g.drawString("X",ejeX+10,origenY+10);
        g.drawString("Y",origenX+15,ejeY+5);

        dibujarEscalaX(g,xmin,xmax,escalaX,origenX,origenY);
        dibujarEscalaY(g,ymin,ymax,escalaY,origenX,origenY);
    }


    public void dibujarEscalaX(Graphics g,double xmin,double xmax,double escalaX,int origenX,int origenY){

        int divisiones = 10;
        double paso = (xmax-xmin)/divisiones;

        for(int i=0;i<=divisiones;i++){

            double valor = xmin + i*paso;

            int x = origenX + (int)((valor-xmin)*escalaX);

            g.drawLine(x,origenY-5,x,origenY+5);

            g.drawString(String.format("%.1f",valor),x-10,origenY+20);
        }
    }


    public void dibujarEscalaY(Graphics g,double ymin,double ymax,double escalaY,int origenX,int origenY){

        int divisiones = 10;
        double paso = (ymax-ymin)/divisiones;

        for(int i=0;i<=divisiones;i++){

            double valor = ymin + i*paso;

            int y = origenY - (int)((valor-ymin)*escalaY);

            g.drawLine(origenX-5,y,origenX+5,y);

            g.drawString(String.format("%.1f",valor),origenX-45,y+5);
        }
    }


    public void dibujarFuncion(Graphics g,double xmin,double xmax,double ymin,double escalaX,double escalaY){

        int origenX = 50;
        int origenY = 320;

        int xAnt=0;
        int yAnt=0;

        boolean primera=true;

        for(double x=xmin;x<=xmax;x+=0.05){

            double y = a*x*x + b*x + c;

            int xCanvas = origenX + (int)((x-xmin)*escalaX);
            int yCanvas = origenY - (int)((y-ymin)*escalaY);

            if(!primera){
                g.drawLine(xAnt,yAnt,xCanvas,yCanvas);
            }

            xAnt=xCanvas;
            yAnt=yCanvas;
            primera=false;
        }
    }
}
