#ifndef NGUYENHAIBINH_6_CLIENT_H
#define NGUYENHAIBINH_6_CLIENT_H

#include <string>

using namespace std;

void downloadFile(int clientFd, string fileName, string localFilePath);
int initializeClient(string serverHost, int port);

#endif // NGUYENHAIBINH_6_CLIENT_H