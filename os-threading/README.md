# Multi-threaded Semaphore Example

This C program demonstrates the use of semaphores and pthreads to manage multiple threads. The program defines a simple scenario where a limited number of threads (representing tasks) can be executed concurrently, and a semaphore is used to control access to the shared resource.

## Overview

The simulation involves two types of threads: workers and tasks. Workers are responsible for executing tasks, and the number of tasks that can be executed concurrently is limited.

## Compilation and Execution

To compile the program, use the following command:

```bash
gcc -pthread semaphore_example.c -o semaphore_example
 ```

To run the program:

```bash
./semaphore_example
```

## Components

### Workers
- Each worker is represented by a thread.
- Workers execute tasks and release the semaphore to allow other workers to proceed.

### Tasks
- Each task is represented by a thread.
- Tasks are executed by workers in a limited concurrency scenario.

### Semaphore
- A semaphore is used to control access to the shared resource (concurrent execution of tasks).
- The semaphore limits the number of tasks that can be executed concurrently.

### Simulation Flow
- Workers are created and start executing tasks.
- The semaphore controls access to the shared resource, ensuring a limited number of tasks can be executed concurrently.
- Tasks are completed, releasing the semaphore for other tasks to proceed.
- The simulation runs indefinitely.

### Important Notes
- The number of workers and tasks is configurable.
- The program uses pthreads for multithreading and semaphores for synchronization.
- The sleep(1) calls simulate the passage of time during task execution.
