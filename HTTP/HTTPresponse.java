/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package httpresponse;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

/**
 *
 * @author Asus
 */
public class HTTPresponse {

    /**
     * @param args the command line arguments
     */
    static final int PORT = 6789;
    public static void main(String[] args) throws IOException {
        
        ServerSocket serverConnect = new ServerSocket(PORT);
        int id = 0;
        
        System.out.println("Server started.\nListening for connections on port : " + PORT + " ...\n");
        while(true)
        {
            Socket s=serverConnect.accept();
            
            Client newClient = new Client(++id,s);
            Thread t = new Thread(newClient);
            t.start();
            System.out.println("Client " + id + " connected to server");
            
        }
        
    }
    
    
    
}

