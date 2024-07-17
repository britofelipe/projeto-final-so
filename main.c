#include <stdio.h>
#include "process.h"

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

    return 0;
}
