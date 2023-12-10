import Client.Client;
import Server.Server;

public class Main {
  public static void main(String args[]) throws Exception {
    new Thread(() -> {
      try {
        Client.main(args);
      } catch (Exception ex) {
      }
    }).start();

    Server.main(args);
  }
}