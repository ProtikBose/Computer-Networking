/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package smtp1;

/**
 *
 * @author Asus
 */
//package smtpskeleton;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Scanner;

public class SMTPSkeleton {

    public static void main(String[] args) throws UnknownHostException, IOException, InterruptedException {

        Thread t = new Thread();
        String instruction;
        Scanner sc = new Scanner(System.in);
        String ins;
        String msg;
        int receivers;
        boolean hasValue;
        String state = "Closed";
        System.out.println("The state is closed state now");

       // String mailServer = "webmail.buet.ac.bd";
        String mailServer = "localhost";
        //String mailServer="smtp.gmail.com";
        InetAddress mailHost = InetAddress.getByName(mailServer);
        InetAddress localHost = InetAddress.getLocalHost();
        Socket smtpSocket = new Socket(mailHost, 25);
        BufferedReader in = new BufferedReader(new InputStreamReader(smtpSocket.getInputStream()));
        PrintWriter pr = new PrintWriter(smtpSocket.getOutputStream(), true);
        
        String welcome = new String();

       // String initialID = in.readLine();
        while (!state.equalsIgnoreCase("Begin")) {


            //timeout check
            welcome = GetValue.runTime(in);

            if (!welcome.equals("none")) {
                if (!GetValue.checkError(welcome)) {
                    System.out.println(welcome);
                    state = "Begin";
                    System.out.println("The system is in " + state + " state");
                } else {
                    System.out.println("The system is in " + state + " state");
                }

            } else {

                System.out.println("The system is in " + state + " state");
                continue;
            }

        }
        
        state = "Begin";

       // System.out.println(initialID);
        //System.out.println("The state is in Begin State");

        

        while (!state.equalsIgnoreCase("Wait")) {

            ins = sc.nextLine();
            pr.println(ins + " " + localHost.getHostName());

            //timeout check
            welcome = GetValue.runTime(in);

            if (!welcome.equals("none")) {
                if (!GetValue.checkError(welcome)) {
                    System.out.println(welcome);
                    state = "Wait";
                    System.out.println("The system is in " + state + " state");
                } else {
                    System.out.println("The system is in " + state + " state");
                }

            } else {

                System.out.println("The system is in " + state + " state");
                continue;
            }

        }
        /* welcome = GetValue.runTime(in);
        if (!welcome.equals("none")) {
            System.out.println(welcome);
            if (welcome.contains("354")) {
                state = "Mail";
                System.out.println("The system is in " + state + " state");
            }

        } else {
            pr.println("RSET");
            state = "Wait";
            System.out.println("The system is in " + state + " state");
        } */

        //System.out.println(welcome);
        //System.out.println("The state is in " + state + " state now");
        while (true) {

            System.out.println("Enter a command : ");

            if (state.equalsIgnoreCase("Recipient")) {
                //System.out.println("How many recipients?: ");
                //receivers = Integer.parseInt(sc.nextLine());

                // System.out.println("To: ");
                ins = sc.nextLine();

                pr.println(ins);
                //timeout check
                welcome = GetValue.runTime(in);
                if (!welcome.equals("none")) {
                    System.out.println(welcome);
                    if (welcome.contains("354")) {
                        state = "Mail";
                        System.out.println("The system is in " + state + " state");
                    }

                } else {
                    pr.println("RSET");
                    state = "Wait";
                    System.out.println("The system is in " + state + " state");
                }
                System.out.println("The system is in " + state + " state");

            } else if (state.equalsIgnoreCase("Mail")) {
                msg = "";
                System.out.println("Type 1 for attachment, 2 for mail");
                Scanner input = new Scanner(System.in);
                if (sc.nextInt() == 2) {
                    while (true) {

                        String temp = sc.nextLine();

                        if (temp.equals(".")) {
                            // msg += temp;
                            break;
                        } else {
                            msg += (temp + "\n");
                        }

                    }

                    msg += ".";
                    ins = msg;
                    state = "Ready";
                    pr.println(ins);
                    //System.out.println(in.read);
                    welcome = GetValue.runTime(in);
                    if (!welcome.equals("none")) {
                        System.out.println(welcome);

                    } else {
                        pr.println("RSET");
                        state = "Wait";
                        System.out.println("The system is in " + state + " state");
                    }
                } else {
                    GetValue.sendFile("pic.jpg", smtpSocket, "image/jpeg");
                    state = "Ready";
                    //System.out.println(in.read);
                    
                }
                System.out.println("The system is in " + state + " state");
            } else {
                ins = sc.nextLine();

                if (ins.equalsIgnoreCase("Quit")) {
                    pr.println("QUIT");
                    //timeout check
                    welcome = GetValue.runTime(in);
                    if (welcome.equals("none")) {
                        continue;
                    } else {
                        System.out.println(welcome);
                        break;
                    }
                } else {
                    if (state.equalsIgnoreCase("Ready")) {
                        state = "Wait";
                        pr.println("RSET");
                        System.out.println("The system is in "+state+" now");
                    }
                    pr.println(ins);
                    welcome = GetValue.runTime(in);
                    System.out.println(welcome);
                    if (!welcome.equals("none")) {
                        if (!GetValue.checkError(welcome)) {
                            System.out.println(welcome);

                            if (state.equalsIgnoreCase("Wait")) {
                                state = "Envelope";
                            } else if (state.equalsIgnoreCase("Envelope")) {
                                state = "Recipient";
                            } else if (state.equalsIgnoreCase("Recipient")) {
                                state = "Mail";

                            }

                            System.out.println("The system is in " + state + " state");
                        } else {
                            System.out.println("The system is in " + state + " state");
                        }
                    } else {
                        pr.println("RSET");
                        state = "Wait";
                        System.out.println("The system is in " + state + " state");
                    }
                }
            }
        }
    }

}
