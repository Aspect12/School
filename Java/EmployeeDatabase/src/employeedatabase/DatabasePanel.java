/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/GUIForms/JFrame.java to edit this template
 */
package employeedatabase;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import javax.swing.JOptionPane;
import javax.swing.table.DefaultTableModel;

/**
 *
 * @author xenoc
 */
public class DatabasePanel extends javax.swing.JFrame {
    boolean isAdmin;

    /**
     * Creates new form DatabasePanel
     */
    public DatabasePanel() {
        initComponents();
    }

    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jScrollPane1 = new javax.swing.JScrollPane();
        employeeTable = new javax.swing.JTable();
        registerButton = new javax.swing.JButton();
        editButton = new javax.swing.JButton();
        deleteButton = new javax.swing.JButton();
        returnButton3 = new javax.swing.JButton();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setPreferredSize(new java.awt.Dimension(1453, 405));
        getContentPane().setLayout(new org.netbeans.lib.awtextra.AbsoluteLayout());

        employeeTable.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {

            },
            new String [] {
                "Name", "Surname", "Father's Name", "Mother's Name", "TIN", "SSN", "Address", "Email 1", "Email 2", "Employee Type", "Vacancies", "Work Hours", "Salary Type", "Kid Count", "Marital Status", "Education Level"
            }
        ) {
            Class[] types = new Class [] {
                java.lang.String.class, java.lang.String.class, java.lang.String.class, java.lang.String.class, java.lang.Integer.class, java.lang.Integer.class, java.lang.String.class, java.lang.String.class, java.lang.String.class, java.lang.String.class, java.lang.Integer.class, java.lang.Integer.class, java.lang.String.class, java.lang.Integer.class, java.lang.String.class, java.lang.String.class
            };
            boolean[] canEdit = new boolean [] {
                false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false
            };

            public Class getColumnClass(int columnIndex) {
                return types [columnIndex];
            }

            public boolean isCellEditable(int rowIndex, int columnIndex) {
                return canEdit [columnIndex];
            }
        });
        jScrollPane1.setViewportView(employeeTable);
        Connection conn = null;

        try {
            conn = DriverManager.getConnection(DBCredentials.CONN_STRING, DBCredentials.USERNAME, DBCredentials.PASSWORD);
            Statement stmt = conn.createStatement();

            String st = "SELECT * FROM employees";
            ResultSet rs = stmt.executeQuery(st);

            DefaultTableModel model = (DefaultTableModel) employeeTable.getModel();

            while (rs.next()) {
                model.addRow(new Object[]{rs.getString("name"), rs.getString("surname"), rs.getString("father_name"), rs.getString("mother_name"), rs.getInt("TIN"), rs.getInt("SSN"), rs.getString("address"), rs.getString("email_1"), rs.getString("email_2"), rs.getInt("employee_type"), rs.getInt("vacancies"), rs.getInt("work_hours"), rs.getInt("salary_type"), rs.getInt("kid_count"), rs.getInt("marital_status"), rs.getString("education_level")});
            }

            conn.close();
        }
        catch (SQLException e)
        {
            JOptionPane.showMessageDialog(null, e);
        }

        getContentPane().add(jScrollPane1, new org.netbeans.lib.awtextra.AbsoluteConstraints(0, 0, 1440, 200));

        registerButton.setText("Register Employee");
        registerButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                registerButtonActionPerformed(evt);
            }
        });
        getContentPane().add(registerButton, new org.netbeans.lib.awtextra.AbsoluteConstraints(654, 220, 145, -1));

        editButton.setText("Edit Employee");
        editButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                editButtonActionPerformed(evt);
            }
        });
        getContentPane().add(editButton, new org.netbeans.lib.awtextra.AbsoluteConstraints(654, 255, 145, -1));

        deleteButton.setText("Delete Employee");
        deleteButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                deleteButtonActionPerformed(evt);
            }
        });
        getContentPane().add(deleteButton, new org.netbeans.lib.awtextra.AbsoluteConstraints(654, 290, 145, -1));

        returnButton3.setText("Return");
        returnButton3.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                returnButton3ActionPerformed(evt);
            }
        });
        getContentPane().add(returnButton3, new org.netbeans.lib.awtextra.AbsoluteConstraints(654, 325, 145, -1));

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void registerButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_registerButtonActionPerformed
        RegisterEmployeeForm employeeForm = new RegisterEmployeeForm();
        employeeForm.setLocationRelativeTo(null);
        employeeForm.setVisible(true);
        employeeForm.isAdmin = isAdmin;

        this.dispose();
    }//GEN-LAST:event_registerButtonActionPerformed

    private void editButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_editButtonActionPerformed
        
    }//GEN-LAST:event_editButtonActionPerformed

    private void deleteButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_deleteButtonActionPerformed
        DefaultTableModel model = (DefaultTableModel) employeeTable.getModel();
        int selectedRow = employeeTable.getSelectedRow();
        
        if (selectedRow < 0) return;
        
        String name = (String) model.getValueAt(selectedRow, 0);
        String surname = (String) model.getValueAt(selectedRow, 1);
        
        Connection conn = null;
        
        try {            
            conn = DriverManager.getConnection(DBCredentials.CONN_STRING, DBCredentials.USERNAME, DBCredentials.PASSWORD);
            Statement stmt = conn.createStatement();
            
            String st = "DELETE FROM `employees` WHERE `employees`.`name` = '" + name + "' AND `employees`.`surname` = '" + surname + "'";

            stmt.executeUpdate(st);
                        
            conn.close();
                        
            JOptionPane.showMessageDialog(null, "Employee deleted!", "Employee Deleted", JOptionPane.INFORMATION_MESSAGE);
        
            model.removeRow(selectedRow);
        }
        catch (SQLException e)
        {
            JOptionPane.showMessageDialog(null, e);
        }        
    }//GEN-LAST:event_deleteButtonActionPerformed

    private void returnButton3ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_returnButton3ActionPerformed
        MainPanel mainPanel = new MainPanel();
        mainPanel.setLocationRelativeTo(null);
        mainPanel.setVisible(true);
        mainPanel.isAdmin = isAdmin;
        mainPanel.manageusersButton.setVisible(isAdmin);

        this.dispose();
    }//GEN-LAST:event_returnButton3ActionPerformed

    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {
        /* Set the Nimbus look and feel */
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
            java.util.logging.Logger.getLogger(DatabasePanel.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(DatabasePanel.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(DatabasePanel.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(DatabasePanel.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>

        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new DatabasePanel().setVisible(true);
            }
        });
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton deleteButton;
    private javax.swing.JButton editButton;
    private javax.swing.JTable employeeTable;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JButton registerButton;
    private javax.swing.JButton returnButton3;
    // End of variables declaration//GEN-END:variables
}
