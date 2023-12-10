/**
 * Author: Hai Binh Nguyen #20020189
 * Server.java: This file implements server main thread.
 */

package Server;

import java.io.IOException;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Server {
  private final int capacity;

  private final List<User> users;
  private final Map<Integer, Thread> connections;
  private final InetAddress bindAddress;
  private final ServerSocket internal;

  public Server(String host, int port, int capacity) throws IOException {
    this.users = new ArrayList<>(capacity);
    this.connections = new HashMap<>(capacity);

    this.capacity = capacity;

    bindAddress = InetAddress.getByName(host);

    internal = new ServerSocket(port, 4, bindAddress);
    internal.setReuseAddress(true);

    if (internal.isBound() && !internal.isClosed()) {
      System.out.printf("Server is listening on %s:%d\n", host, port);
    }
  }

  public void lifeCycle() throws IOException, InterruptedException {
    int connectionCount = 0;

    System.out.printf("Server life cycle method started, client capacity: %d\n", capacity);

    while (true) {
      if (users.size() >= capacity) {
        Thread.sleep(10);

        continue;
      }

      System.out.println("Waiting for new client ...");

      var socket = internal.accept();

      System.out.println("New client connected");

      var user = new User(this, socket, ++connectionCount);

      var worker = new Worker(user);

      var workerThread = new Thread(worker);
      workerThread.setName(String.format("Worker#%d", user.getConnectionId()));
      workerThread.start();

      connections.put(user.getConnectionId(), workerThread);
      users.add(user);
      System.out.printf("Connection pool: %d/%d\n", users.size(), capacity);
    }
  }

  public List<User> getUserList() {
    return users.subList(0, users.size());
  }

  public User findUser(int userId) {
    for (var user : users)
      if (user.getUserId() == userId)
        return user;

    return null;
  }

  public void userExit(int userId) {
    User targetUser = null;

    for (var user : users) {
      if (user.getUserId() == userId) {
        targetUser = user;
        break;
      }
    }

    if (targetUser == null)
      return;

    users.remove(targetUser);
    connections.remove(targetUser.getConnectionId());
  }
}
