/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package com.mycompany.ejemplolap2;

import java.awt.Canvas;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;

/**
 *
 * @author Ahg3L
 */
public class grafica_principal extends Canvas{
    //este programa sirve para graficar la función:
    // y = 6x^2 +17x-20;
    String HeadText = "Mi grafica"; 

    public grafica_principal(String in_headtext){
        HeadText = in_headtext;
        this.setSize(new Dimension(850, 600));
        this.setBackground(new Color(20,20,20));
    }

    grafica_principal() {
        this.setSize(new Dimension(800, 500));
        this.setBackground(new Color(20,20,20));
    }

@Override

    public void paint(Graphics g) {

        super.paint(g); // Generated from nbfs://nbhost/SystemFileSystem/Templates/Classes/Code/OverriddenMethodBody

        double[] x = new  double[200];
        double[] y = new double[200]; 
        int[] xx = new int[200];
        int[] yy = new int[200]; 
        //float incr = 850/x.length;
        double incr = 45.0/200.0;
        double razonx  = 800.0/45.0;
        //float icount = 425;
        double icount =-22.5;
        for(int ii = 0; ii<200; ii++, icount +=incr){

              x[ii] = icount;
              y[ii]= (6*x[ii]*x[ii]) + (17*x[ii])- 20;
              System.out.println(x[ii]+", "+y[ii]);
              xx[ii] = tr_x(x[ii]*razonx);
              yy[ii] = tr_y(y[ii]);



        }

        g.setColor(Color.LIGHT_GRAY);
        g.drawLine(0, 250, 800, 250);
        g.drawLine(400, 0, 400, 500);

        g.setColor(Color.PINK);

        for(int ii= 0;ii< xx.length-1; ii++){
            g.setColor(Color.PINK);
            g.drawLine((int)xx[ii] ,(int ) yy[ii] ,(int ) xx[ii+1],(int ) yy[ii+1]);
            int rr = (int)(Math.random()*255);
            int gg = (int)(Math.random()*255);
            int bb = (int)(Math.random()*255);
            Color my_color = new Color(rr, gg, bb);
            g.drawLine(((int)xx[ii])-10 ,(int ) yy[ii],((int)xx[ii])+10, (int ) yy[ii]);
            g.drawLine(((int)xx[ii]) ,(int ) yy[ii]+10,((int)xx[ii]), (int ) yy[ii]-10);

//g.setColor(Color);

        }

    }

    private int tr_x(double x){
        double xx= x+400;
        return Math.round((float)xx);
    }

    private int tr_y(double y){
        double yy = 250 - y;
        return Math.round((float)yy);
    }
}

