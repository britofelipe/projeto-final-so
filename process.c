#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "process.h"

void initializeQueue(ProcessQueue *queue) {
    queue->front = -1;
    queue->rear = -1;
}

int isQueueFull(ProcessQueue *queue) {
    return (queue->rear + 1) % MAX_PROCESSES == queue->front;
}

int isQueueEmpty(ProcessQueue *queue) {
    return queue->front == -1;
}

void enqueue(ProcessQueue *queue, Process process) {
    if (isQueueFull(queue)) {
        printf("Queue is full. Cannot add process.\n");
        return;
    }
    if (isQueueEmpty(queue)) {
        queue->front = 0;
    }
    queue->rear = (queue->rear + 1) % MAX_PROCESSES;
    queue->processes[queue->rear] = process;
}

Process dequeue(ProcessQueue *queue) {
    if (isQueueEmpty(queue)) {
        printf("Queue is empty. Cannot remove process.\n");
        exit(1);
    }
    Process process = queue->processes[queue->front];
    if (queue->front == queue->rear) {
        queue->front = -1;
        queue->rear = -1;
    } else {
        queue->front = (queue->front + 1) % MAX_PROCESSES;
    }
    return process;
}

void displayQueue(const ProcessQueue *queue) {
    if (isQueueEmpty(queue)) {
        printf("Queue is empty.\n");
        return;
    }
    printf("Processes in the queue:\n");
    int i = queue->front;
    while (1) {
        printf("PID: %d, Name: %s, State: %s\n", queue->processes[i].pid, queue->processes[i].name,
               queue->processes[i].state == RUNNING ? "RUNNING" :
               queue->processes[i].state == SUSPENDED ? "SUSPENDED" : "TERMINATED");
        if (i == queue->rear) break;
        i = (i + 1) % MAX_PROCESSES;
    }
}

void addProcess(ProcessQueue *queue, int pid, const char *name) {
    Process process;
    process.pid = pid;
    process.state = RUNNING;
    strncpy(process.name, name, sizeof(process.name) - 1);
    process.name[sizeof(process.name) - 1] = '\0';
    enqueue(queue, process);
    printf("Process %s with PID %d added and is now RUNNING.\n", name, pid);
}

void suspendProcess(ProcessQueue *queue, int pid) {
    if (isQueueEmpty(queue)) {
        printf("Queue is empty. No process to suspend.\n");
        return;
    }
    int i = queue->front;
    while (1) {
        if (queue->processes[i].pid == pid) {
            queue->processes[i].state = SUSPENDED;
            printf("Process with PID %d is now SUSPENDED.\n", pid);
            return;
        }
        if (i == queue->rear) break;
        i = (i + 1) % MAX_PROCESSES;
    }
    printf("Process with PID %d not found.\n", pid);
}

void terminateProcess(ProcessQueue *queue, int pid) {
    if (isQueueEmpty(queue)) {
        printf("Queue is empty. No process to terminate.\n");
        return;
    }
    int i = queue->front;
    while (1) {
        if (queue->processes[i].pid == pid) {
            queue->processes[i].state = TERMINATED;
            printf("Process with PID %d is now TERMINATED.\n", pid);
            return;
        }
        if (i == queue->rear) break;
        i = (i + 1) % MAX_PROCESSES;
    }
    printf("Process with PID %d not found.\n", pid);
}

// Simulação de chamadas de sistema

int sys_fork(ProcessQueue *queue, int parent_pid, int new_pid, const char *name) {
    int i = queue->front;
    while (1) {
        if (queue->processes[i].pid == parent_pid) {
            addProcess(queue, new_pid, name);
            return new_pid;
        }
        if (i == queue->rear) break;
        i = (i + 1) % MAX_PROCESSES;
    }
    printf("Parent process with PID %d not found.\n", parent_pid);
    return -1;
}

void sys_exec(ProcessQueue *queue, int pid, const char *new_name) {
    int i = queue->front;
    while (1) {
        if (queue->processes[i].pid == pid) {
            strncpy(queue->processes[i].name, new_name, sizeof(queue->processes[i].name) - 1);
            queue->processes[i].name[sizeof(queue->processes[i].name) - 1] = '\0';
            printf("Process with PID %d is now executing %s.\n", pid, new_name);
            return;
        }
        if (i == queue->rear) break;
        i = (i + 1) % MAX_PROCESSES;
    }
    printf("Process with PID %d not found.\n", pid);
}

void sys_wait(ProcessQueue *queue, int pid) {
    int i = queue->front;
    while (1) {
        if (queue->processes[i].pid == pid) {
            printf("Waiting for process with PID %d to terminate.\n", pid);
            while (queue->processes[i].state != TERMINATED) {
                // Aqui poderia ser uma espera ativa ou uma simulação de espera real
            }
            printf("Process with PID %d has terminated.\n", pid);
            return;
        }
        if (i == queue->rear) break;
        i = (i + 1) % MAX_PROCESSES;
    }
    printf("Process with PID %d not found.\n", pid);
}

// Gerenciamento de threads

void initializeThreadPool(ThreadPool *pool) {
    pool->count = 0;
    pthread_mutex_init(&pool->lock, NULL);
}

int createThread(ThreadPool *pool, const char *name, void *(*start_routine)(void *)) {
    if (pool->count >= MAX_THREADS) {
        printf("Thread pool is full. Cannot create more threads.\n");
        return -1;
    }

    pthread_mutex_lock(&pool->lock);

    Thread thread;
    thread.tid = pool->count;
    thread.state = RUNNING;
    strncpy(thread.name, name, sizeof(thread.name) - 1);
    thread.name[sizeof(thread.name) - 1] = '\0';
    pthread_create(&thread.thread, NULL, start_routine, NULL);

    pool->threads[pool->count++] = thread;

    pthread_mutex_unlock(&pool->lock);

    printf("Thread %s with TID %d created and is now RUNNING.\n", name, thread.tid);
    return thread.tid;
}

void suspendThread(ThreadPool *pool, int tid) {
    pthread_mutex_lock(&pool->lock);

    for (int i = 0; i < pool->count; i++) {
        if (pool->threads[i].tid == tid) {
            pool->threads[i].state = SUSPENDED;
            printf("Thread with TID %d is now SUSPENDED.\n", tid);
            pthread_mutex_unlock(&pool->lock);
            return;
        }
    }

    pthread_mutex_unlock(&pool->lock);
    printf("Thread with TID %d not found.\n", tid);
}

void terminateThread(ThreadPool *pool, int tid) {
    pthread_mutex_lock(&pool->lock);

    for (int i = 0; i < pool->count; i++) {
        if (pool->threads[i].tid == tid) {
            pool->threads[i].state = TERMINATED;
            pthread_cancel(pool->threads[i].thread);
            printf("Thread with TID %d is now TERMINATED.\n", tid);
            pthread_mutex_unlock(&pool->lock);
            return;
        }
    }

    pthread_mutex_unlock(&pool->lock);
    printf("Thread with TID %d not found.\n", tid);
}

void displayThreads(const ThreadPool *pool) {
    pthread_mutex_lock((pthread_mutex_t *)&pool->lock);

    if (pool->count == 0) {
        printf("No threads in the pool.\n");
        pthread_mutex_unlock((pthread_mutex_t *)&pool->lock);
        return;
    }

    printf("Threads in the pool:\n");
    for (int i = 0; i < pool->count; i++) {
        printf("TID: %d, Name: %s, State: %s\n", pool->threads[i].tid, pool->threads[i].name,
               pool->threads[i].state == RUNNING ? "RUNNING" :
               pool->threads[i].state == SUSPENDED ? "SUSPENDED" : "TERMINATED");
    }

    pthread_mutex_unlock((pthread_mutex_t *)&pool->lock);
}
