/**
 * Author: Hai Binh Nguyen #20020189
 * UserInputHandler.java: This file implements handler
 * for user input thread.
 */

package Client;

import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;

public class UserInputHandler implements Runnable {
  private final DataOutputStream egress;

  public UserInputHandler(Socket socket) throws IOException {
    egress = new DataOutputStream(socket.getOutputStream());
  }

  private void sendBuffer(byte[] buffer, int readBytes) {
    try {
      egress.write(buffer, 0, readBytes);
      egress.flush();
    } catch (IOException ignored) {
    }
  }

  private boolean isQuit(byte[] buffer, int readBytes) {
    if (readBytes < 4)
      return false;

    return buffer[0] == 'q' &&
        buffer[1] == 'u' &&
        buffer[2] == 'i' &&
        buffer[3] == 't';
  }

  @Override
  public void run() {
    var buffer = new byte[1024];

    while (true) {
      try {
        var readBytes = System.in.read(buffer);

        this.sendBuffer(buffer, readBytes);

        if (isQuit(buffer, readBytes))
          break;
      } catch (IOException ignored) {
      }

      try {
        Thread.sleep(1);
      } catch (InterruptedException ignored) {
      }
    }
  }
}
