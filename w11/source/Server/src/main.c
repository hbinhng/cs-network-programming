/**
 * Author: Hai Binh Nguyen #20020189
 * main.c: This file bootstraps the server.
 */

#include "Server/Server.h"

int main() {
  Server *server = Server_Init("127.0.0.1", 4620, 10);
  Server_LifeCycle(server);
}