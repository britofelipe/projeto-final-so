#include <stdio.h>
#include "process.h"

void *thread_function(void *arg) {
    printf("Thread is running...\n");
    return NULL;
}

int main() {
    ProcessQueue queue;
    initializeQueue(&queue);

    // Adicionar processos iniciais
    addProcess(&queue, 1, "Process1");
    addProcess(&queue, 2, "Process2");
    displayQueue(&queue);

    // Suspender e exibir processos
    suspendProcess(&queue, 1);
    displayQueue(&queue);

    // Terminar e exibir processos
    terminateProcess(&queue, 2);
    displayQueue(&queue);

    // Simulação de chamadas de sistema
    int child_pid = sys_fork(&queue, 1, 3, "ChildProcess1");
    displayQueue(&queue);

    if (child_pid != -1) {
        sys_exec(&queue, child_pid, "NewChildProcess1");
        displayQueue(&queue);
    }

    sys_wait(&queue, 3);
    terminateProcess(&queue, 3);
    displayQueue(&queue);

    // Gerenciamento de threads
    ThreadPool pool;
    initializeThreadPool(&pool);

    int tid1 = createThread(&pool, "Thread1", thread_function);
    int tid2 = createThread(&pool, "Thread2", thread_function);
    displayThreads(&pool);

    suspendThread(&pool, tid1);
    displayThreads(&pool);

    terminateThread(&pool, tid2);
    displayThreads(&pool);

    return 0;
}
