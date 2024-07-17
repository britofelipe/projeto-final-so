#ifndef PROCESS_H
#define PROCESS_H

#define MAX_PROCESSES 10

typedef enum {
    RUNNING,
    SUSPENDED,
    TERMINATED
} ProcessState;

typedef struct {
    int pid;
    ProcessState state;
    char name[50];
} Process;

typedef struct {
    Process processes[MAX_PROCESSES];
    int front;
    int rear;
} ProcessQueue;

void initializeQueue(ProcessQueue *queue);
int isQueueFull(ProcessQueue *queue);
int isQueueEmpty(ProcessQueue *queue);
void enqueue(ProcessQueue *queue, Process process);
Process dequeue(ProcessQueue *queue);
void displayQueue(const ProcessQueue *queue);
void addProcess(ProcessQueue *queue, int pid, const char *name);
void suspendProcess(ProcessQueue *queue, int pid);
void terminateProcess(ProcessQueue *queue, int pid);

#endif // PROCESS_H
