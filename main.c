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
    addProcess(&queue, 1, "Process1", 5, 1);
    addProcess(&queue, 2, "Process2", 3, 3);
    addProcess(&queue, 3, "Process3", 2, 2);
    displayQueue(&queue);

    // Teste de escalonamento FIFO
    printf("\n--- Scheduling using FIFO ---\n");
    scheduleFIFO(&queue);
    displayQueue(&queue);

    // Re-adicionar processos para outros testes
    addProcess(&queue, 1, "Process1", 5, 1);
    addProcess(&queue, 2, "Process2", 3, 3);
    addProcess(&queue, 3, "Process3", 2, 2);

    // Teste de escalonamento Round-Robin
    printf("\n--- Scheduling using Round-Robin ---\n");
    scheduleRoundRobin(&queue);
    displayQueue(&queue);

    // Re-adicionar processos para outros testes
    addProcess(&queue, 1, "Process1", 5, 1);
    addProcess(&queue, 2, "Process2", 3, 3);
    addProcess(&queue, 3, "Process3", 2, 2);

    // Teste de escalonamento por Prioridade
    printf("\n--- Scheduling using Priority ---\n");
    schedulePriority(&queue);
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
