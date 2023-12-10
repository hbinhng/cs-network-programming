/**
 * @author Hai-Binh Nguyen #20020189
 * Bài 3: Copy chỉ các ký tự là chữ
 */

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.nio.charset.StandardCharsets;
import java.util.Scanner;

public class Bai3 {
    private static boolean isChar(char character) {
        return (character >= 'a' && character <= 'z') ||
                (character >= 'A' && character <= 'Z');
    }

    public static void main(String[] args) throws Exception {
        var scanner = new Scanner(System.in);

        System.out.print("Source file name: ");
        var srcFileName = scanner.next();
        var srcFile = new File(srcFileName);

        if (!srcFile.exists()) {
            System.out.print("Source file does not exist");
            return;
        }

        System.out.print("Destination file name: ");
        var destFileName = scanner.next();
        var destFile = new File(destFileName);

        scanner.close();
        if (!destFile.exists()) destFile.createNewFile();

        var srcFileStream = new FileInputStream(srcFile);
        var destFileStream = new FileOutputStream(destFile);

        var source = new String(
                srcFileStream.readAllBytes(),
                StandardCharsets.UTF_8
        ).toCharArray();

        srcFileStream.close();

        var contentBuilder = new StringBuilder();

        for (var character : source)
            if (isChar(character)) contentBuilder.append(character);

        var content = contentBuilder.toString();
        destFileStream.write(content.getBytes(StandardCharsets.UTF_8));

        destFileStream.close();
    }
}
