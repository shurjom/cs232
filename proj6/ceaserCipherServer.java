/*
 * caesarCipherServer implements the server side of 
 * taking a string and caeser cipher encrypting it.
 * client side calls to implement/get data from server
 *
 * Written by: Shurjo Maitra (sm47)
 * Project 6 - CS 232
 * Date: May 14, 2017
 */

import java.util.Scanner;
import java.net.Socket;
import java.net.ServerSocket;
import java.io.PrintWriter;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.IOException;
import java.util.Date;
import java.text.DateFormat;
import java.text.SimpleDateFormat;

public class caesarCipherServer {
	public static void main(String[] args) throws Exception {
		// checks the input argument from client for length = 1
		if (args.length != 1) {
            System.err.println("Usage Error: Java Server <port number>");
            System.exit(1);
        }
        // port number is obtained from user
		int portNo = Integer.parseInt(args[0]);
		System.out.println("Server started in port:  " + portNo);
		// creates inifinte loop variable that waits for new connections to server
		Boolean listening = true;
		// attempts to connect to given port number server
		try (ServerSocket serverSocket = new ServerSocket(portNo)) { 

			// waits for new connections to server and creates new threads
            while (listening) {
	            new caesarCipherMultiThread(serverSocket.accept()).start();
	        }
	    // outputs error if the given port number is unusable
	    } catch (IOException e) {
            System.err.println("Port Error: Could not listen on port: " + portNo);
            System.exit(-1);
        }
    }
}



// Class which implements new threads to server, when client creates it
class caesarCipherMultiThread extends Thread {
	// private instance variable for socket connection from clieint
	private Socket clientSocket = null;
	private String ipAddr; //ipaddress

	//function implmententing the threads of the client 
	public caesarCipherMultiThread(Socket socket) {
		//assign connection from main class to the newly created thread
		this.clientSocket = socket;
		// display the date, time and IP of the client connected 
		DateFormat dateFormat = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss");
		Date date = new Date();
		ipAddr = this.clientSocket.getRemoteSocketAddress().toString();
		System.out.println("New connection to " + ipAddr + " at " + dateFormat.format(date));
	}

	// infinite loop to obtain user input and encrypt it
	public void run() {

		try (   PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);
				BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
		) {
			// to determine if the user has inputted an integer
			Boolean integer = true;
			Integer rotationInt = 0;
			String inputline;
			// while input can be read from the client by server
			while ((inputline = in.readLine()) != null) {
				// when user connects, they need to input a rotation value
				if (integer) {
					rotationInt = Integer.parseInt(inputline);
					out.append(rotation.toString());
					out.append("\n");
					out.flush();
					integer = false;
					continue;
				}
				// calls the cipher function to carry out encryption and append to output
				out.append(caesarCipher(rotationInt, inputline));
				out.append("\n");
				out.flush();
			}
			// when client is quit, close connectoin to client, close thread
			System.out.println("Closing connection to client: " + ipAddr);
			out.close();
			in.close();
			clientSocket.close();
		} 
		//catches and throws error
		catch (IOException e) {
			e.printStackTrace();
		}
	}
	//implments function that encrypts text based on a rotationInt value
	private static String caesarCipher(Integer rotation, String string) {
		//declare strings used to start of the text string
		String tempStr = "";
		Integer length = string.length();
		// the loop goes back to the beginning of the alphabet if rotation makes it reach end
		for (Integer x = 0; x < length; x++) {
			char c = (char)(string.charAt(x) + rotationInt);
			if (c > 'z') {
				tempStr += (char)(string.charAt(x) - (26-rotationInt));
			} else {
				tempStr += (char)(string.charAt(x) + rotationInt);
			}
		}
		return tempStr;
	}
}