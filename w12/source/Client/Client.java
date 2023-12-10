/**
 * Author: Hai Binh Nguyen #20020189
 * Client.java: This file implements the client object.
 */

package Client;

import java.io.IOException;
import java.net.Socket;
import java.net.UnknownHostException;

public class Client {
  private final Socket internal;

  private Thread serverResponseHandler;
  private Thread userInputHandler;

  public Client(String host, int port) throws UnknownHostException, IOException {
    internal = new Socket(host, port);

    serverResponseHandler = new Thread(new ServerResponseHandler(internal));
    userInputHandler = new Thread(new UserInputHandler(internal));

    serverResponseHandler.setName("server response");
    userInputHandler.setName("user input");

    System.out.println("Connected to server");
  }

  public void start() throws InterruptedException {
    serverResponseHandler.start();
    userInputHandler.start();

    serverResponseHandler.join();
    userInputHandler.join();
  }
}
