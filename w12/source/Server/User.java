/**
 * Author: Hai Binh Nguyen #20020189
 * User.java: This file implements user object.
 */

package Server;

import java.net.Socket;

public class User {
  private final Server server;
  private final Socket socket;

  private int userId;
  private int connectionId;

  public User(Server server, Socket socket, int connectionId) {
    this.server = server;
    this.socket = socket;
    this.connectionId = connectionId;
    this.userId = -connectionId;
  }

  public int getConnectionId() {
    return connectionId;
  }

  public int getUserId() {
    return userId;
  }

  public void setUserId(int userId) {
    this.userId = userId;
  }

  public Socket getSocket() {
    return socket;
  }

  public Server getServer() {
    return server;
  }
}
