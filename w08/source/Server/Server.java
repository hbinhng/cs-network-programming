/**
 * Author: Hai-Binh Nguyen #20020189
 * Server.java: This file implement server side application.
 */

package Server;

import java.net.Socket;
import java.net.SocketException;
import java.io.*;
import java.nio.ByteBuffer;
import java.net.InetAddress;
import java.net.ServerSocket;

public class Server {
  private static enum SessionType {
    NORMAL, DOWNLOAD, QUIT, PASSEDTHRU
  }

  private final InetAddress bindAddress;

  private final ServerSocket internal;

  private Socket client = null;
  private BufferedReader clientIncoming = null;
  private DataOutputStream clientOutgoing = null;

  public Server(String host, int port) throws IOException {
    bindAddress = InetAddress.getByName(host);

    internal = new ServerSocket(port, 4, bindAddress);

    if (internal.isBound() && !internal.isClosed()) {
      System.out.printf("Server is listening on %s:%d\n", host, port);
    }
  }

  private void handleDownload(String fileName) throws IOException {
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
    } catch (FileNotFoundException ex) {
      clientOutgoing.write("404 FILE not found".getBytes());
    }

  }

  public void lifeCycle() throws IOException {
    var session = SessionType.NORMAL;

    while (true) {
      if (internal.isClosed())
        break;

      if (client == null) {
        client = internal.accept();

        System.out.println("New client connected");

        clientIncoming = new BufferedReader(
            new InputStreamReader(client.getInputStream()));
        clientOutgoing = new DataOutputStream(client.getOutputStream());

        continue;
      }

      try {
        var content = clientIncoming.readLine();

        if (content == null)
          throw new SocketException("Unexpectedly closed");

        if (content.equals(""))
          continue;

        System.out.printf("Received message: %s\n", content);

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

        if (session == SessionType.QUIT) {
          clientIncoming.close();
          clientOutgoing.close();
          client.close();
          client = null;
          clientIncoming = null;
          clientOutgoing = null;
        }

        if (session == SessionType.PASSEDTHRU)
          session = SessionType.NORMAL;
      } catch (SocketException ex) {
        if (clientIncoming != null)
          clientIncoming.close();
        if (clientOutgoing != null)
          clientOutgoing.close();
        if (client != null)
          client.close();

        clientIncoming = null;
        clientOutgoing = null;
        client = null;
        session = SessionType.NORMAL;

        System.out.println("Client unexpectedly close the connection");
      }
    }

    internal.close();
  }

  public static void main(String args[]) {
    try {
      var server = new Server("127.0.0.1", 4620);

      server.lifeCycle();
    } catch (Exception ex) {
      System.err.println(ex.getMessage());
    }
  }
}
