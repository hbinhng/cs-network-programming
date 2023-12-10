/**
 * @author Hai-Binh Nguyen #20020189
 * Bài 1: Đếm số lượng chữ cái, số và ký tự đặc biệt
 */

import java.util.Scanner;

public class Bai1 {
    public static void main(String[] args) {
        var scanner = new Scanner(System.in);
        var str = scanner.next();

        var characterCount = 0;
        var digitCount = 0;
        var symbolCount = 0;

        for (var i : str.toCharArray()) {
            if ((i >= 'a' && i <= 'z') || (i >= 'A' && i <= 'Z')) {
                characterCount++;
                continue;
            }

            if (i >= '0' && i <= '9') {
                digitCount++;
                continue;
            }

            symbolCount++;
        }

        System.out.printf("Characters: %d\nDigits: %d\nSymbols: %d\n", characterCount, digitCount, symbolCount);
    }
}
