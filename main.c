#include <stdio.h>
#include "process.h"

int main() {
    ProcessQueue queue;
    initializeQueue(&queue);

    addProcess(&queue, 1, "Process1");
    addProcess(&queue, 2, "Process2");
    displayQueue(&queue);

    suspendProcess(&queue, 1);
    displayQueue(&queue);

    terminateProcess(&queue, 2);
    displayQueue(&queue);

    return 0;
}
