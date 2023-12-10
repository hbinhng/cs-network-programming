/**
 * Hai-Binh Nguyen #20020189
 * User info (model) file.
 */

public class UserInfo {
    public String userName;
    public int userAge;

    public String toJSON() {
        return String.format("{\"user name\":\"%s\",\"user age\":%d}", userName, userAge);
    }
}
