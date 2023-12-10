/**
 * Author: Hai Binh Nguyen #20020189
 * Program.java: This file boostraps the application.
 */

package Client;

import java.io.FilterInputStream;
import java.io.IOException;
import java.util.Scanner;

public class Program {
  public static void main(String[] args) throws Exception {
    var sc = new Scanner(new FilterInputStream(System.in) {
      @Override
      public void close() throws IOException {
        // don't close System.in!
      }
    });

    System.out.print("Server host? ");

    var host = sc.next();

    sc.close();

    var client = new Client(host, 4620);
    client.start();
  }
}
