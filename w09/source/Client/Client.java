/**
 * Author: Hai-Binh Nguyen #20020189
 * Client.java: This file implement client side application.
 */

package Client;

import java.io.*;
import java.net.*;
import java.util.Scanner;

public class Client {
  private final String host;
  private final Socket internal;
  private final BufferedInputStream incoming;
  private final DataOutputStream outgoing;

  public Client(String host, int port) throws UnknownHostException, IOException {
    this.host = host;
    internal = new Socket(host, port);

    incoming = new BufferedInputStream(internal.getInputStream());
    outgoing = new DataOutputStream(internal.getOutputStream());
  }

  private String readBuffer(byte[] buffer, int start, int length) {
    char[] raw = new char[length];

    for (int i = 0; i < length; i++)
      raw[i] = (char) buffer[i + start];

    return new String(raw);
  }

  public void downloadFile(String fileName, String localFileName) throws IOException {
    System.out.printf("Trying to download %s@%s to ./%s\n", fileName, host, localFileName);

    outgoing.writeBytes("DOWNLOAD\n");

    var buffer = new byte[1024];

    var readBytes = incoming.read(buffer);

    var response = readBuffer(buffer, 0, readBytes);

    assert response.equals("200 DOWNLOAD OK");

    outgoing.writeBytes(fileName + "\n");

    readBytes = incoming.read(buffer);

    response = readBuffer(buffer, 0, readBytes);

    assert response.equals("201 FILE found;");

    var localFile = new File(localFileName);
    var fileOutputStream = new FileOutputStream(localFile);
    var dataOutputStream = new DataOutputStream(fileOutputStream);
    var eof = false;

    while ((readBytes = incoming.read(buffer)) > 0) {
      var ending = readBuffer(buffer, readBytes - 7, 7);

      if (ending.equals("<<EOF>>")) {
        readBytes -= 7;
        eof = true;
      }

      dataOutputStream.write(buffer, 0, readBytes);

      if (eof)
        break;
    }

    dataOutputStream.close();
    fileOutputStream.flush();
    fileOutputStream.close();
    incoming.close();
    outgoing.close();
    internal.close();
  }

  public static void main(String[] args) throws Exception {
    var scanner = new Scanner(System.in);

    System.out.print("Server host? ");
    var host = scanner.next();
    System.out.print("File name? ");
    var fileName = scanner.next();

    var client = new Client(host, 4620);

    client.downloadFile(fileName, fileName + " - copy");

    scanner.close();
  }
}
