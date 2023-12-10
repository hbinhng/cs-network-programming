#ifndef W11_SERVER_WORKER_H
#define W11_SERVER_WORKER_H

#include <pthread.h>

#include "../User/User.h"

struct WorkerArg {
  pthread_t *ThreadId;
  User *User;
};

typedef struct WorkerArg WorkerArg;

void *Worker_ThreadRoutine(void *args);
WorkerArg *WorkerArg_Init(pthread_t *threadId, User *user);
void WorkerArg_Dispose(WorkerArg *arg);

#endif // W11_SERVER_WORKER_H