#ifndef NGUYENHAIBINH_5_COMMON_H
#define NGUYENHAIBINH_5_COMMON_H

#include <ctype.h>

extern const int PORT;

char *ltrim(char *s);
char *rtrim(char *s);
char *trim(char *s);

void sendMessage(int fd, char *content);

#endif // NGUYENHAIBINH_5_COMMON_H