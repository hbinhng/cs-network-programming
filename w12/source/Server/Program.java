/**
 * Author: Hai Binh Nguyen #20020189
 * Program.java: This file bootstraps the application.
 */

package Server;

public class Program {
  public static void main(String[] args) throws Exception {
    var server = new Server("127.0.0.1", 4620, 10);
    server.lifeCycle();
  }
}
