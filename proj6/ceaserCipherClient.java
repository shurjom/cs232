/*
 * caesarCipherClient is the client for calling the CaesarCipher server after prompting for the rotationInt number
 *	and any number of lines of text
 *
 * Written by: Shurjo Maitra (sm47)
 * CS 232 - Project6
 * Date: May 14, 2017
 */

import java.util.Scanner;
import java.net.Socket;
import java.io.PrintWriter;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.IOException;
import java.io.OutputStream;

public class caesarCipherClient {

	public static void main(String[] args) throws IOException {

		// check for 2 arguments from user
		if (args.length != 2) {
            System.err.println("Required number of arguments not found! (2)");
            System.exit(1);
        }

		// catch port no. and server name from returnText arg list
		Integer portNo;
		String serverName = args[0];

		// determine if portNO was an integer (valid)
		try {
			portNo = Integer.parseInt(args[1]);
		} catch (NumberFormatException ex) {
			System.out.println("Port number not valid! (int)");
			return;
		}

		// introduce user to program and prompt for an integer offset value
		System.out.println("Shurjo's casesarCipherClient!\n");
		System.out.print("Enter the rotation offset integer (1-25: ");

		// read in rotationInt offset integer from user
		Scanner getIn = new Scanner(System.in);
		Integer rotationInt = getIn.nextInt();

		// make required server sockets and connections to cipher
		try (
			Socket serverSocket = new Socket(serverName, portNo);
			OutputStream os = serverSocket.getOutputStream();
			PrintWriter out = new PrintWriter(os, true);
			BufferedReader input = new BufferedReader(new InputStreamReader(serverSocket.getInStream()));
		) {
			// write the rotation offset integer to the server
			out.append(rotationInt.toString());
			out.append("\n");
			out.flush();

			// read the integer output from the server 
			String outputInt = input.readLine();
			System.out.println("Server return rotation offset integer of: " + outputInt + "\n");

			//
			String returnText;
			while (true) {
				//Ask user continually to enter text to use with cipher
				System.out.print("Please enter the text to use with cipher: ");
				String text = getIn.nextLine();

				//quit the program when the user enters the string "quit"
				if (text.equals("quit")) {
					System.out.println("\nText successfully sent to cipher server!\n");
					break;
				}
				//send text to cipher server to do the encryption
				out.append(text);
				out.append("\n");
				out.flush();

				returnText = input.readLine();
				//read the text back from the cipher server
				System.out.println("Returned text from cipher is: " + returnText + "\n");
			}
		} catch (IOException e) {
			//if there is an error print out an error message, then exit
			System.err.println("No connection to server " + serverName);
			System.exit(-1);
		}

	}
}
