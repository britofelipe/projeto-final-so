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

// Funções de simulação de chamadas de sistema
int sys_fork(ProcessQueue *queue, int parent_pid, int new_pid, const char *name);
void sys_exec(ProcessQueue *queue, int pid, const char *new_name);
void sys_wait(ProcessQueue *queue, int pid);

#endif // PROCESS_H
