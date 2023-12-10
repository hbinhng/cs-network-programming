/**
 * Author: Hai-Binh Nguyen #20020189
 * Server.java: This file implement server side application's main thread which manages worker threads.
 */

package Server;

import java.io.*;
import java.util.LinkedList;
import java.util.List;
import java.net.InetAddress;
import java.net.ServerSocket;

public class Server {
  private static final int POOL_SIZE = 8;

  private final List<ServerWorker> workers;

  private final InetAddress bindAddress;

  private final ServerSocket internal;

  public Server(String host, int port) throws IOException {
    bindAddress = InetAddress.getByName(host);

    internal = new ServerSocket(port, 4, bindAddress);
    internal.setReuseAddress(true);

    if (internal.isBound() && !internal.isClosed()) {
      System.out.printf("Server is listening on %s:%d\n", host, port);
    }

    this.workers = new LinkedList<>();
  }

  public void lifeCycle() throws IOException, InterruptedException {
    while (true) {
      for (var worker : workers) {
        if (worker.isStopped()) {
          System.out.println("Terminating thread");
          workers.remove(worker);
        }
      }

      if (workers.size() < POOL_SIZE) {
        var newSocket = internal.accept();

        System.out.println("New client connected");

        var worker = new ServerWorker(newSocket);

        this.workers.add(worker);

        var workerThread = new Thread(worker);
        workerThread.start();
      }

      Thread.sleep(10);
    }
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
