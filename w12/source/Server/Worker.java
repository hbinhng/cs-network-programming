/**
 * Author: Hai Binh Nguyen #20020189
 * Worker.java: This file implements worker thread.
 */

package Server;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;

public class Worker implements Runnable {
  private final User user;

  private BufferedReader clientIncoming;
  private DataOutputStream clientOutgoing;

  public Worker(User user) throws IOException {
    this.user = user;

    var socket = user.getSocket();

    clientIncoming = new BufferedReader(
        new InputStreamReader(socket.getInputStream()));
    clientOutgoing = new DataOutputStream(socket.getOutputStream());
  }

  private String readContent() {
    String content = null;

    try {
      content = clientIncoming.readLine();
    } catch (IOException ex) {
      return null;
    }

    return content;
  }

  private void handleLogin(String[] tokens) throws IOException {
    if (tokens.length < 2) {
      clientOutgoing.write("Empty ID\n".getBytes());
      return;
    }

    int id;

    try {
      id = Integer.parseInt(tokens[1]);

      if (id < 0) {
        clientOutgoing.write("ID must be positive\n".getBytes());
        return;
      }
    } catch (NumberFormatException ex) {
      clientOutgoing.write("ID must be a number\n".getBytes());
      return;
    }

    var existingUser = user.getServer().findUser(id);

    if (existingUser != null) {
      clientOutgoing.write(
          String.format("ID #%d already exists\n", id).getBytes());
      return;
    }

    System.out.printf("User changed from ID #%d to #%d\n", user.getUserId(), id);

    user.setUserId(id);

    clientOutgoing.write(
        String.format("Logged in with ID #%d\n", id)
            .getBytes());
  }

  private void handleList() throws IOException {
    for (var user : user.getServer().getUserList()) {
      var iUid = user.getUserId();

      if (iUid <= 0)
        continue;

      clientOutgoing.write(
          String.format("%d%s\n", iUid, iUid == this.user.getUserId() ? " <- you" : "")
              .getBytes());
    }

    clientOutgoing.write("#\n".getBytes());
  }

  private void handleSend(String[] tokens, String origin) throws IOException {
    if (user.getUserId() < 0) {
      clientOutgoing.write("You have to log in first\n".getBytes());
      return;
    }

    int targetUid;

    try {
      if (tokens.length < 2)
        throw new NumberFormatException();

      targetUid = Integer.parseInt(tokens[1]);
    } catch (NumberFormatException ignored) {
      clientOutgoing.write("Recipient does not exist\n".getBytes());
      return;
    }

    var targetUser = user.getServer().findUser(targetUid);

    if (targetUser == null) {
      clientOutgoing.write("Recipient does not exist\n".getBytes());
      return;
    }

    var targetStream = new DataOutputStream(targetUser.getSocket().getOutputStream());

    var content = String.format("[From %d] %s\n",
        user.getUserId(),
        origin.substring(origin.indexOf(tokens[1]) + tokens[1].length()));
    targetStream.write(content.getBytes());

    clientOutgoing.write("Message was sent successfully\n".getBytes());
  }

  private boolean handle(String content) throws IOException {
    var tokens = content.split("\\s+");

    switch (tokens[0]) {
      case "quit":
        return true;

      case "login":
        handleLogin(tokens);
        break;

      case "list":
        handleList();
        break;

      case "send":
        handleSend(tokens, content);
        break;

      default:
        clientOutgoing.write("Unknown command\n".getBytes());
        break;
    }

    return false;
  }

  @Override
  public void run() {
    System.out.println("Worker thread initialized\n");

    while (true) {
      var content = readContent();

      if (content == null) {
        System.out.println("Client unexpectedly closed connection");
        break;
      }

      var quited = false;

      try {
        quited = handle(content);
      } catch (Exception ignored) {
      }

      if (quited) {
        System.out.println("Client exited\n");
        try {
          clientOutgoing.write("OK bye\n".getBytes());
        } catch (IOException ignored) {
        }
        user.getServer().userExit(user.getUserId());
        break;
      }

      try {
        clientOutgoing.flush();
      } catch (Exception ignored) {
      }
    }

    try {
      clientIncoming.close();
      clientOutgoing.close();
      user.getSocket().close();
    } catch (IOException ignored) {
    }
  }
}
