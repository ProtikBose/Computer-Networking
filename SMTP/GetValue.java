/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package smtp1;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.Base64;

/**
 *
 * @author PRANTO
 */
public class GetValue {

    static final String pathFragment = "\\src\\smtp1\\";

    public static String runTime(BufferedReader br) throws InterruptedException, IOException {
        Thread t = new Thread();
        String welcome = new String();
        welcome = "none";
        // System.out.println("in runtime");
        boolean hasValue = false;
        for (int counter = 0; counter < 15; counter++) {
            if (br.ready()) {
                welcome = br.readLine();
            }
            t.sleep(1000);
            //welcome = br.readLine();
            //System.out.println("welcome");
            if (!welcome.equals("none")) {
                hasValue = true;
                break;
            }

        }

        return welcome;

    }

    public static boolean checkError(String welcome) {
        String[] save = welcome.split(" ");
        if (save[0].charAt(0) == '4' || save[0].charAt(0) == '5') {
            return true;
        }
        return false;
    }

    public static void sendFile(String fileName, Socket socket, String fileType) {
        try {
            File file = new File("D:\\Academic life\\3-2\\My Study\\Networking Sessional\\Offline 1 code\\SMTP1\\src\\smtp1\\" + fileName);
            if (file.exists()) {
                System.out.println("Alive");
            }
            FileInputStream fis = new FileInputStream(file);
            OutputStream os = socket.getOutputStream();
            //BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            PrintWriter pr = new PrintWriter(socket.getOutputStream(), true);
            byte[] contents;
            long fileLength = file.length();

            //content length
            pr.println("MIME-Version : 1.0");
            pr.println("Content-Type: multipart/mixed;" + "boundary=" + "c2ltcGxlIGJvdW5kYXJ5");	//These two lines are used
            pr.println();
            
            //email text
            pr.println("--c2ltcGxlIGJvdW5kYXJ5");
            pr.println("Content-type: text/plain; charset=us-ascii ");     //Message
            pr.println();
            pr.println("Hello Pranto");
            pr.println();
            
            //pdf 
            pr.println("--c2ltcGxlIGJvdW5kYXJ5");
            pr.println("Content-type: application/pdf");
            pr.println("MIME-Version : 1.0");
            pr.println("Content-Transfer-Encoding: base64");
            pr.println();
            File file1 = new File("D:\\Academic life\\3-2\\My Study\\Networking Sessional\\Offline 1 code\\SMTP1\\src\\smtp1\\" + "Assignment.pdf");
            if (file.exists()) {
                System.out.println("Alive");
            }
            FileInputStream fis1 = new FileInputStream(file);
            contents = new byte[(int) fileLength];
            fis1.read(contents, 0, (int) fileLength);
            byte[] encodedBytes1 = Base64.getEncoder().encode(contents);

            os.write(encodedBytes1);
            pr.println();
            
            //song
            pr.println("--c2ltcGxlIGJvdW5kYXJ5");
            pr.println("Content-type: audio/mp3");
            pr.println("MIME-Version : 1.0");
            pr.println("Content-Transfer-Encoding: base64");
            pr.println();
            File file2 = new File("D:\\Academic life\\3-2\\My Study\\Networking Sessional\\Offline 1 code\\SMTP1\\src\\smtp1\\" + "song.mp3");
            if (file.exists()) {
                System.out.println("Alive");
            }
            FileInputStream fis2 = new FileInputStream(file);
            contents = new byte[(int) fileLength];
            fis1.read(contents, 0, (int) fileLength);
            byte[] encodedBytes2 = Base64.getEncoder().encode(contents);

            os.write(encodedBytes2);
            pr.println();
            
            //image
            pr.println("--c2ltcGxlIGJvdW5kYXJ5");
            pr.println("Content-Type: image/jpeg");	//These two lines are used
            pr.println("MIME-Version : 1.0");
            pr.println("Content-Transfer-Encoding: base64");
            pr.println();

//                } 
            contents = new byte[(int) fileLength];
            fis.read(contents, 0, (int) fileLength);
            byte[] encodedBytes = Base64.getEncoder().encode(contents);

            os.write(encodedBytes);

            //os.flush();
            pr.println();
            pr.println("--c2ltcGxlIGJvdW5kYXJ5--");
            pr.println(".");
//            //System.out.println("Sending file ... "+(current*100)/fileLength+"% complete!");
//            }   

            fis.close();
            os.close();
            pr.close();
            System.out.println("File sent successfully!");
        } catch (Exception e) {
            System.err.println("Could not transfer file.");
        }
        // pr.println("Downloaded.");
        //pr.flush();

    }
}
