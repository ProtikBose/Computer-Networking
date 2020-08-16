/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package httpresponse;

import static httpresponse.Client.pathFragment;
import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.Socket;



/**
 *
 * @author Asus
 */
public class Helper {
    
    public static String convertHTML(String fileName,String method,String postValue){
        
        StringBuilder contentBuilder = new StringBuilder();
        System.out.println("filName: " +fileName);
        System.out.println("method: " +method);
        System.out.println("postValue: " +postValue);
        try {
            
                
                
                BufferedReader in = new BufferedReader(new FileReader(fileName));
                String str;
                while ((str = in.readLine()) != null) {
                    //System.out.println("str: " +str);
                    contentBuilder.append(str+"\n");
            }
            in.close();
        } catch (IOException e) {
          
            System.out.println("File not found");
 
        }
        String content = contentBuilder.toString();
        
        //edit html
        //Document doc = Jsoup.parse(content);
        
        contentBuilder = new StringBuilder();
        int index = content.indexOf("HTTP REQUEST TYPE-> ");
        String replaced1,replaced2;
        
        while(content.charAt(index) != '<'){
            contentBuilder.append(content.charAt(index));
            index++;
        }
        replaced1 = contentBuilder.toString();
        
        index = content.indexOf("Post-> ");
        contentBuilder = new StringBuilder();
        while(content.charAt(index) != '<'){
            contentBuilder.append(content.charAt(index));
            index++;
        }
        replaced2 = contentBuilder.toString();
        
        System.out.println(replaced1+" | "+replaced2);
        content = content.replace( replaced1,"HTTP REQUEST TYPE-> "+method.toUpperCase() );
        content = content.replace( replaced2,"Post-> "+postValue);
        
        return content;
    }
    
    public static String fileNameExtract(String input){
        
//        System.out.println(input);
        //getting the file name 
        String []a=input.split("/");           
        // System.out.println(a[1]);             
        String []b = a[1].split(" ");
        
//        System.out.println(b);
//        
//        System.out.println(a[0]+","+a[1]);
//        
//        System.out.println(b[0]);
        
        return b[0];
        
    }
    
    public static void sendFile(String fileName, String Method,Socket socket, BufferedReader in){
        try
	{
            File file = new File(System.getProperty("user.dir")+Client.pathFragment+fileName);
            FileInputStream fis = new FileInputStream(file);
	    OutputStream os = socket.getOutputStream();
            //BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            PrintWriter pr = new PrintWriter(socket.getOutputStream());
	    byte[] contents;
	    long fileLength = file.length();

            //content length
            pr.println("Content-Length: "+String.valueOf(fileLength));	//These two lines are used
	    pr.println();
            pr.flush();                               //to send the file size in bytes.
           // System.out.println("2."+in.readLine());
           
							
            long current = 0;

            while(current!=fileLength){ 
                int size = 10000;
                if(fileLength - current >= size)
                    current += size;    
                else{ 
                    size = (int)(fileLength - current); 
                    current = fileLength;
                } 
                contents = new byte[(int)fileLength]; 
                fis.read(contents, 0, (int)fileLength); 
                os.write(contents);
            //System.out.println("Sending file ... "+(current*100)/fileLength+"% complete!");
            }   
            os.flush(); 
            fis.close();
            os.close();
            pr.close();
            System.out.println("File sent successfully!");
            }
            catch(Exception e)
            {
                System.err.println("Could not transfer file.");
            }
           // pr.println("Downloaded.");
            //pr.flush();

    }
    
}
