/**
 * Hai-Binh Nguyen #20020189
 * Server file.
 */

import java.io.*;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;

public class Server {
    private ServerSocket server = null;
    private Socket incoming = null;
    private boolean waitingForInfo = false;
    private UserInfo info;
    private DataInputStream in;
    private DataOutputStream out;

    public Server(int port) throws IOException {
        server = new ServerSocket(port, 0, InetAddress.getByAddress(new byte[] { 0, 0, 0, 0 }));

        System.out.printf("Server started @%d\n", port);

        info = new UserInfo();
        listen();
    }

    private void send(String content) throws IOException {
        if (incoming == null) return;

        out.writeUTF(content);
        out.flush();
    }

    private int handleUserInfoUpdate(String payload) {
        var tokens = payload.split("\\{\\s*\\\"|\\\"\\:\\s*\\\"?|\\\"\\,\\s*\\\"|\\s*}");
        var valueNext = false;
        var field = "";

        for (var _token : tokens) {
            var token = _token.trim();

            if (token.isEmpty()) continue;

            if (!valueNext) {
                field = token.toLowerCase();
                valueNext = true;
            } else {
                if (field.equals("user name")) {
                    if (token.matches("[\\W\\d]"))
                        return 1;

                    info.userName = token;
                } else if (field.equals("user age")) {
                    if (token.matches("\\D"))
                        return 2;

                    info.userAge = Integer.parseInt(token);
                }

                valueNext = false;
            }
        }

        return 0;
    }

    private void listen() throws IOException {
        while (true) {
            if (incoming == null) {
                incoming = server.accept();
                in = new DataInputStream(new BufferedInputStream(incoming.getInputStream()));
                out = new DataOutputStream(new BufferedOutputStream(incoming.getOutputStream()));
                System.out.println("\n\n=========== New client connected ===========");
            }

            var line = "";

            try {
                while (!(line = in.readUTF()).equals("QUIT")) {
                    System.out.println("\n\nNew message arrived");
                    System.out.printf("From client: %s\n", line);

                    if (line.startsWith("HELLO")) {
                        System.out.println("Client sent HELLO message");
                        send("200 Hello client");
                    } else if (line.startsWith("USER INFO")) {
                        System.out.println("Client sent USER INFO message, start waiting for INFO");
                        waitingForInfo = true;
                        send("210 OK");
                    } else if (waitingForInfo == true) {
                        var result = handleUserInfoUpdate(line);

                        if (result == 0)
                            send("211 User Info OK");
                        else
                            send("400 User Info Error");

                        System.out.printf("Info set to: '%s' as user name, %d as user age\n", info.userName, info.userAge);

                        waitingForInfo = false;
                    } else
                        send("409 Unknown message");
                }

                System.out.println("\n\nDisconnecting...");
                send("500 bye");
            } catch (SocketException ex) {
                incoming = null;
            }
        }
    }

    public static void main(String[] args) throws Exception {
        new Server(2046);
    }
}
