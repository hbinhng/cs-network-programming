/**
 * @author Hai-Binh Nguyen #20020189
 * Bài 2: Lưu lại các ký tự nhập đến khi gặp ký tự dừng
 */

import java.io.File;
import java.io.FileOutputStream;
import java.nio.charset.StandardCharsets;
import java.util.Scanner;

public class Bai2 {
    private static final char STOP_CHAR = '$';

    public static void main(String[] args) throws Exception {
        var scanner = new Scanner(System.in);

        System.out.print("File name: ");
        var fileName = scanner.next();

        var file = new File(fileName);

        if (!file.exists()) file.createNewFile();

        var fileStream = new FileOutputStream(file);

        var contentBuilder = new StringBuilder();

        while (scanner.hasNext()) {
            var word = scanner.next();
            var breakFlag = false;

            for (var i = 0; i < word.length(); i++) {
                if (word.charAt(i) == STOP_CHAR) {
                    breakFlag = true;
                    word = word.substring(0, i);
                }

                if (breakFlag) break;
            }

            contentBuilder.append(word);

            if (breakFlag) break;

            contentBuilder.append(' ');
        }

        scanner.close();

        var content = contentBuilder.toString();
        fileStream.write(content.getBytes(StandardCharsets.UTF_8));

        fileStream.close();

        var fileLength = file.length();
        System.out.printf("File size: %d\n", fileLength);
    }
}
