/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/GUIForms/JFrame.java to edit this template
 */
package com.mycompany.fractus;

/**
 *
 * @author Lab.Computacion
 */

public class ventanita extends javax.swing.JFrame {

    private lienzo miLienzo;
    
    public ventanita() {
        initComponents();
        miLienzo = new lienzo();
        add(miLienzo);
    }
    
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        longitud = new javax.swing.JTextField();
        iteraciones = new javax.swing.JTextField();
        boton = new javax.swing.JButton();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setMaximumSize(new java.awt.Dimension(1000, 700));
        setMinimumSize(new java.awt.Dimension(1000, 700));
        setPreferredSize(new java.awt.Dimension(1000, 700));

        longitud.setMaximumSize(new java.awt.Dimension(20, 20));
        longitud.setName(""); // NOI18N
        longitud.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                longitudActionPerformed(evt);
            }
        });

        iteraciones.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                iteracionesActionPerformed(evt);
            }
        });

        boton.setText("Dibujar");
        boton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                botonActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGap(200, 200, 200)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addComponent(longitud, javax.swing.GroupLayout.DEFAULT_SIZE, 78, Short.MAX_VALUE)
                    .addComponent(iteraciones))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 425, Short.MAX_VALUE)
                .addComponent(boton)
                .addGap(16, 16, 16))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addContainerGap()
                        .addComponent(longitud, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(iteraciones, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(20, 20, 20)
                        .addComponent(boton)))
                .addContainerGap(406, Short.MAX_VALUE))
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void botonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_botonActionPerformed
        try { 
        int l = Integer.parseInt(longitud.getText());
        int it = Integer.parseInt(iteraciones.getText());
        
        miLienzo.setParametros(l, it);   // ← ESTA ES LA LÍNEA IMPORTANTE
        
        } catch (NumberFormatException e) {       
            javax.swing.JOptionPane.showMessageDialog(this, 
            "Por favor ingresa números válidos en ambos campos.");
        }
    }//GEN-LAST:event_botonActionPerformed

    private void iteracionesActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_iteracionesActionPerformed
        try { int it = Integer.parseInt(iteraciones.getText()); javax.swing.JOptionPane.showMessageDialog(this, "Has ingresado las iteraciones: " + it);
        } catch (NumberFormatException e) { javax.swing.JOptionPane.showMessageDialog(this, "Por favor ingresa un número válido en iteraciones."); }
    }//GEN-LAST:event_iteracionesActionPerformed

    private void longitudActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_longitudActionPerformed
        try { int l = Integer.parseInt(longitud.getText()); javax.swing.JOptionPane.showMessageDialog(this, "Has ingresado la longitud: " + l);
        } catch (NumberFormatException e) { javax.swing.JOptionPane.showMessageDialog(this, "Por favor ingresa un número válido en longitud."); }
    }//GEN-LAST:event_longitudActionPerformed

    public static void main(String args[]) {
        //<editor-fold defaultstate="collapsed" desc=" Look and feel setting code (optional) ">
        /* If Nimbus (introduced in Java SE 6) is not available, stay with the default look and feel.
         * For details see http://download.oracle.com/javase/tutorial/uiswing/lookandfeel/plaf.html 
         */
        try {
            for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) {
                    javax.swing.UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        } catch (ClassNotFoundException ex) {
            java.util.logging.Logger.getLogger(ventanita.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(ventanita.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(ventanita.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(ventanita.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>
        
        java.awt.EventQueue.invokeLater(() -> {
            new ventanita().setVisible(true);
        });
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton boton;
    private javax.swing.JTextField iteraciones;
    private javax.swing.JTextField longitud;
    // End of variables declaration//GEN-END:variables

}