/**
 * @author Hai-Binh Nguyen #20020189
 * Bài 4: Copy file ảnh
 */

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.util.Scanner;

public class Bai4 {
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

        destFileStream.write(srcFileStream.readAllBytes());

        srcFileStream.close();
        destFileStream.close();
    }
}
