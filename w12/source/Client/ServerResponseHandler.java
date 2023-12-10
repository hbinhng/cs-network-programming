/**
 * Author: Hai Binh Nguyen #20020189
 * ServerResponseHandler.java: This file implements handler
 * for server response thread.
 */

package Client;

import java.io.BufferedInputStream;
import java.io.IOException;
import java.net.Socket;

public class ServerResponseHandler implements Runnable {
  private final BufferedInputStream ingress;
  private final Socket socket;

  public ServerResponseHandler(Socket socket) throws IOException {
    this.socket = socket;
    ingress = new BufferedInputStream(socket.getInputStream());
  }

  private void printBuffer(byte[] buffer, int readBytes) {
    var charArray = new char[readBytes];

    for (var i = 0; i < readBytes; i++)
      charArray[i] = (char) buffer[i];

    var content = String.copyValueOf(charArray);

    System.out.print(content);
  }

  @Override
  public void run() {
    var buffer = new byte[1024];

    while (true) {
      try {
        if (!socket.isConnected())
          break;

        var readBytes = ingress.read(buffer);

        if (readBytes <= 0)
          break;

        this.printBuffer(buffer, readBytes);
      } catch (IOException ignored) {
        System.out.println("Here");
      }

      try {
        Thread.sleep(1);
      } catch (InterruptedException ignored) {
      }
    }

    try {
      ingress.close();
    } catch (IOException ignored) {
    }
  }
}
