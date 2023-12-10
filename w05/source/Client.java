/**
 * Hai-Binh Nguyen #20020189
 * Client file.
 */

import java.io.*;
import java.net.InetAddress;
import java.net.Socket;

public class Client {
    private Socket socket;
    private DataInputStream in;
    private DataOutputStream out;

    public Client(InetAddress serverAddress, int port) throws IOException {
        socket = new Socket(serverAddress, port);

        System.out.println("Client connected");

        in = new DataInputStream(new BufferedInputStream(socket.getInputStream()));
        out = new DataOutputStream(new BufferedOutputStream(socket.getOutputStream()));
    }

    private void send(String content) throws IOException {
        out.writeUTF(content);
        out.flush();
    }

    private String read() throws IOException {
        return in.readUTF();
    }

    public String hello() throws IOException {
        send("HELLO server");

        return read();
    }

    public String sendInfo(UserInfo info) throws IOException {
        var builder = new StringBuilder();

        send("USER INFO");

        builder.append(read());

        send(info.toJSON());

        builder.append(read());

        return builder.toString();
    }

    public String quit() throws IOException {
        send("QUIT");

        return read();
    }
}
