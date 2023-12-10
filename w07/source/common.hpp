#ifndef NGUYENHAIBINH_6_COMMON_H
#define NGUYENHAIBINH_6_COMMON_H

#include <string>
#include <algorithm>
#include <sys/socket.h>

using namespace std;

struct Message
{
  string content;
  sockaddr sourceAddress;
};

string ltrim(const string &s);
string rtrim(const string &s);
string trim(const string &s);

static inline Message receiveMessage(int srcFd);
static inline void sendMessage(int dstFd, const Message &message);

#endif // NGUYENHAIBINH_6_COMMON_H