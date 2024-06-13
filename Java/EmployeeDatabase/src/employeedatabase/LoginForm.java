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

/**
 *
 * @author xenoc
 */
public class LoginForm extends javax.swing.JFrame {

    /**
     * Creates new form LoginForm
     */
    public LoginForm() {
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

        usernameField = new javax.swing.JTextField();
        loginButton = new javax.swing.JButton();
        passwordField = new javax.swing.JPasswordField();
        titlePanel = new javax.swing.JLabel();
        jLabel1 = new javax.swing.JLabel();
        jLabel2 = new javax.swing.JLabel();
        forgotPassword = new javax.swing.JLabel();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setPreferredSize(new java.awt.Dimension(413, 255));
        setResizable(false);
        getContentPane().setLayout(new org.netbeans.lib.awtextra.AbsoluteLayout());

        usernameField.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                usernameFieldActionPerformed(evt);
            }
        });
        getContentPane().add(usernameField, new org.netbeans.lib.awtextra.AbsoluteConstraints(115, 69, 170, -1));

        loginButton.setText("Log In");
        loginButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                loginButtonActionPerformed(evt);
            }
        });
        getContentPane().add(loginButton, new org.netbeans.lib.awtextra.AbsoluteConstraints(133, 167, 135, -1));
        getContentPane().add(passwordField, new org.netbeans.lib.awtextra.AbsoluteConstraints(115, 101, 170, -1));

        titlePanel.setFont(new java.awt.Font("Segoe UI", 1, 24)); // NOI18N
        titlePanel.setText("Login Form");
        getContentPane().add(titlePanel, new org.netbeans.lib.awtextra.AbsoluteConstraints(135, 25, 130, -1));

        jLabel1.setFont(new java.awt.Font("Segoe UI", 1, 14)); // NOI18N
        jLabel1.setText("Username:");
        getContentPane().add(jLabel1, new org.netbeans.lib.awtextra.AbsoluteConstraints(29, 66, -1, -1));

        jLabel2.setFont(new java.awt.Font("Segoe UI", 1, 14)); // NOI18N
        jLabel2.setText("Password:");
        getContentPane().add(jLabel2, new org.netbeans.lib.awtextra.AbsoluteConstraints(29, 98, -1, -1));

        forgotPassword.setForeground(new java.awt.Color(11, 87, 208));
        forgotPassword.setText("<html><u>Forgot Password?</u></html>");
        forgotPassword.setCursor(new java.awt.Cursor(java.awt.Cursor.HAND_CURSOR));
        forgotPassword.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                forgotPasswordMouseClicked(evt);
            }
        });
        getContentPane().add(forgotPassword, new org.netbeans.lib.awtextra.AbsoluteConstraints(180, 133, -1, -1));

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void usernameFieldActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_usernameFieldActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_usernameFieldActionPerformed

    private void loginButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_loginButtonActionPerformed
        String username = usernameField.getText();
        char[] password = passwordField.getPassword();
        
        Connection conn = null;
        
        try {            
            conn = DriverManager.getConnection(DBCredentials.CONN_STRING, DBCredentials.USERNAME, DBCredentials.PASSWORD);
            Statement stmt = conn.createStatement();
            
            String st = "SELECT access_accounts.username username, access_accounts.password password, access_accounts.isAdmin isAdmin FROM access_accounts WHERE ";
            st = st + "username = \"" + username + "\" AND ";
            st = st + "password = \"" + new String(password) + "\"";
                       
            ResultSet rs = stmt.executeQuery(st);
            Boolean success = false;
            Boolean isAdmin = false;

            if (rs.next()) {
                success = true;
                isAdmin = rs.getBoolean(3);
            }
                        
            conn.close();
            
            if (!success) {
                JOptionPane.showMessageDialog(null, "That is not a valid username or password!", "Login Failure", JOptionPane.ERROR_MESSAGE);
            
                return;
            }
            
            MainPanel mainPanel = new MainPanel();
            mainPanel.setLocationRelativeTo(null);
            mainPanel.setVisible(true);
            mainPanel.isAdmin = isAdmin;
            mainPanel.manageusersButton.setVisible(isAdmin);
            
            this.dispose();
        }
        catch (SQLException e)
        {
            JOptionPane.showMessageDialog(null, e);
        }
    }//GEN-LAST:event_loginButtonActionPerformed

    private void forgotPasswordMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_forgotPasswordMouseClicked
        JOptionPane.showMessageDialog(null, "Too bad!", "Forgot Password", JOptionPane.WARNING_MESSAGE);
    }//GEN-LAST:event_forgotPasswordMouseClicked

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
            java.util.logging.Logger.getLogger(LoginForm.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(LoginForm.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(LoginForm.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(LoginForm.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>

        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new LoginForm().setVisible(true);
            }
        });
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JLabel forgotPassword;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JButton loginButton;
    private javax.swing.JPasswordField passwordField;
    private javax.swing.JLabel titlePanel;
    private javax.swing.JTextField usernameField;
    // End of variables declaration//GEN-END:variables
}
