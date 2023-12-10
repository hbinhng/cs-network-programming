#ifndef NGUYENHAIBINH_5_CLIENT_H
#define NGUYENHAIBINH_5_CLIENT_H

void downloadFile(int clientFd, char *fileName, char *localFilePath);
int initializeClient(char *serverHost);

#endif // NGUYENHAIBINH_5_CLIENT_H