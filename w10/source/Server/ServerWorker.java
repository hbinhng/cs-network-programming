/**
 * Author: Hai-Binh Nguyen #20020189
 * ServerWorker.java: This file implement server worker which handles client connections directly.
 */

package Server;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;
import java.net.SocketException;
import java.nio.ByteBuffer;

public class ServerWorker implements Runnable {
  private static enum SessionType {
    NORMAL, DOWNLOAD, QUIT, PASSEDTHRU
  }

  private boolean stopped;

  private final Socket client;
  private BufferedReader clientIncoming;
  private DataOutputStream clientOutgoing;
  private SessionType session;

  public ServerWorker(Socket socket) throws IOException {
    if (socket == null)
      throw new NullPointerException("Socket passed to worker thread is null");

    stopped = false;

    client = socket;

    clientIncoming = new BufferedReader(
        new InputStreamReader(socket.getInputStream()));
    clientOutgoing = new DataOutputStream(socket.getOutputStream());
  }

  private void handleDownload(String fileName) {
    try {
      var file = new File(fileName);

      if (!file.exists()) {
        clientOutgoing.write("404 FILE not found".getBytes());
        return;
      }

      var fileInputStream = new FileInputStream(file);
      var dataInputStream = new DataInputStream(fileInputStream);
      var buffer = ByteBuffer.allocate(1024);
      var bufferInternal = buffer.array();
      var readBytes = 0;

      System.out.printf("File found: %s (%d bytes)\n", fileName, file.length());
      clientOutgoing.write("201 FILE found;".getBytes());

      while ((readBytes = dataInputStream.read(bufferInternal)) > 0) {
        clientOutgoing.write(bufferInternal, 0, readBytes);

        buffer.clear();
      }

      clientOutgoing.write("<<EOF>>".getBytes());

      dataInputStream.close();
      fileInputStream.close();
    } catch (IOException ex) {
      if (ex instanceof FileNotFoundException)
        try {
          clientOutgoing.write("404 FILE not found".getBytes());
        } catch (IOException ignored) {
        }
    }
  }

  private void handle(String content) throws IOException {
    String outgoing = "";

    if (session == SessionType.NORMAL && content.equals("DOWNLOAD")) {
      outgoing = "200 DOWNLOAD OK";
      session = SessionType.DOWNLOAD;
    } else if (session == SessionType.NORMAL && content.equals("QUIT")) {
      outgoing = "500 bye";
      session = SessionType.QUIT;
    } else if (session == SessionType.DOWNLOAD) {
      handleDownload(content);
      session = SessionType.PASSEDTHRU;
    } else
      outgoing = "400 UNKNOWN command";

    if (session != SessionType.PASSEDTHRU)
      clientOutgoing.write(outgoing.getBytes());

    if (session == SessionType.QUIT)
      throw new SocketException("closed");

    if (session == SessionType.PASSEDTHRU)
      session = SessionType.NORMAL;
  }

  @Override
  public void run() {
    this.session = SessionType.NORMAL;

    while (!stopped) {
      if (!client.isConnected()) {
        stopped = true;
        continue;
      }

      try {
        String content = null;

        try {
          content = clientIncoming.readLine();
        } catch (IOException ignored) {
        }

        if (content == null)
          throw new SocketException("unexpectedly closed");

        if (content.equals(""))
          continue;

        System.out.printf("Received message: %s\n", content);

        handle(content);
      } catch (IOException ex) {
        if (ex instanceof SocketException) {
          try {
            if (clientIncoming != null)
              clientIncoming.close();
            if (clientOutgoing != null)
              clientOutgoing.close();
            if (client != null) {
              client.shutdownInput();
              client.shutdownOutput();
              client.close();
            }
          } catch (Exception ignored) {
          }

          clientIncoming = null;
          clientOutgoing = null;
          stopped = true;

          System.out.printf("Client %s the connection\n", ex.getMessage());
        }
      }
    }

    Thread.currentThread().interrupt();
  }

  public boolean isStopped() {
    return stopped;
  }
}
