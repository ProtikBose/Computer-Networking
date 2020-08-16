/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package httpresponse;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.Date;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Asus
 */
class Client implements Runnable{

    private Socket connectionSocket;
    private int id;
    static final String pathFragment = "\\src\\httpresponse\\";
    
    Client(int id, Socket sckt){
        this.id = id;
        this.connectionSocket = sckt;
    }
    
    @Override
    public void run() {
        //To change body of generated methods, choose Tools | Templates.
        String fileName;
        String response;
        String input;
        String method;
        String mimeType = "text/plain";
        String requestBody;
        int valueLength;
        String postValue="";

        
        //System.out.println("Here1");
        try{
            //System.out.println("Here2");
            //while(true){
            BufferedReader in = new BufferedReader(new InputStreamReader(connectionSocket.getInputStream()));
            PrintWriter pr = new PrintWriter(connectionSocket.getOutputStream());
            OutputStream os = connectionSocket.getOutputStream();
            
            FileWriter logFile = new FileWriter(System.getProperty("user.dir")+pathFragment+"log.txt",true);
            Date date = new Date();
            logFile.write(date.toString()+"\n\n");
            
            input = in.readLine();
            logFile.write(input+"\n");
            
            if(input.contains("POST")){
                
                char[] temp = new char[10000];
                in.read(temp);
                requestBody = new String(temp);
                System.out.println(requestBody);
                
                //System.out.println(requestBody.charAt(requestBody.indexOf("Content-Length: ")+new String("Content-Length: ").length()));
                int i = requestBody.indexOf("Content-Length: ")+new String("Content-Length: ").length();
  
                
                StringBuilder sb = new StringBuilder();
                while(requestBody.charAt(i)>='0' && requestBody.charAt(i)<='9'){
                    sb.append(requestBody.charAt(i));
                    i++;
                }
                
                //System.out.println(sb.toString());
                valueLength = Integer.parseInt(sb.toString()) - new String("user=").length();
                
                i = requestBody.indexOf("user=") + new String("user=").length();
                int endIndex = i+valueLength-1;
                //System.out.println("b: "+requestBody.charAt(i)+"e: "+requestBody.charAt(endIndex));
                
                sb = new StringBuilder();
                while(i<=endIndex){
                    sb.append(requestBody.charAt(i));
                    i++;
                }
               
                postValue = sb.toString();
                System.out.println(postValue);
            }
            
            System.out.println("request : "+input);
            //System.out.println("Here3");
            
            //extracting method
            String[] methodExtract = input.split("/");
            method = methodExtract[0];
            
            fileName = Helper.fileNameExtract(input);

            if(fileName.equals(""))
                fileName = "httpPost.html";
            
            
            //System.out.println("Here4");
            File temp = new File(System.getProperty("user.dir")+pathFragment+fileName);
            //when 404 error occurs
            if(!temp.exists()){
                logFile.write("404\n\n");
                response = "HTTP/1.1 404 NOT FOUND";
                pr.println(response);
                pr.flush();
                System.out.println(in.readLine());
                System.out.println("404 NOT FOUND");
                //send response headers
                response="Content-Type: text/html";
                pr.println(response);
                pr.flush();
                System.out.println(in.readLine());
                
                response = "\r\n";
                pr.println(response);
                pr.flush();
                System.out.println(in.readLine());
                
                
                response = "<html>"+
                        "<head></head>"+
                        "<body>"+
                        "<h1>404 NOT FOUND<h1>"+
                        "</body>"+
                        "</html>";

                pr.println(response);
                pr.flush();
                System.out.println(in.readLine());

                
            }
            
            //when 200 OK 
            else{
                
                logFile.write("200\n\n");
                //System.out.println(fileName);
                //edit the html file with the type of method requested
                if(fileName.equals("httpPost.html") || fileName.equals("form_submitted.html")){
                    String editedFile = Helper.convertHTML(System.getProperty("user.dir")+pathFragment+fileName, method, postValue);
                    //System.out.println("file\n"+editedFile);

                    FileWriter fw = new FileWriter(System.getProperty("user.dir")+pathFragment+fileName);
                    fw.write(editedFile);
                    fw.close();
                }
                //first response line
                response="HTTP/1.1 200 OK";
                pr.println(response);
                pr.flush();
               // System.out.println("1."+in.readLine());
               // System.out.println("fileName: "+fileName);
                
                //extract content type  
                if(fileName.contains("html"))
                    mimeType = "text/html";
                else if(fileName.contains("txt"))
                    mimeType = "text/plain";
                else if(fileName.contains("jpg")||fileName.contains("jpeg"))
                    mimeType = "image/jpeg";
                else if(fileName.contains("gif"))
                    mimeType = "image/gif";
                else if(fileName.contains("png"))
                    mimeType = "image/png";
                else if(fileName.contains("bmp"))
                    mimeType = "image/bmp";
                else if(fileName.contains("pdf"))
                    mimeType = "application/pdf";
                else if(fileName.contains("mp3"))
                    mimeType = "audio/mp3";
                
                //System.out.println("mimeType: "+mimeType);
                //send response headers
             
                pr.println("Content-type: "+mimeType);             
                Helper.sendFile(fileName, method,connectionSocket,in);
            }
    
            logFile.close();
            in.close();
            pr.close();
            connectionSocket.close();
            
           //}
            
        }catch(Exception e){
           System.out.println("Error loading page.."); 
        }
    
    }
}
