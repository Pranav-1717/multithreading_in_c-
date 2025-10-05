# Mutex vs Semaphore

| Feature              | **Mutex**                                                 | **Semaphore**                                                          |
| -------------------- | --------------------------------------------------------- | ---------------------------------------------------------------------- |
| **Definition**       | Mutual exclusion lock allowing only one thread at a time. | Synchronization tool controlling multiple access with a counter.       |
| **Resource Access**  | Only 1 thread can access.                                 | Multiple threads (as per count) can access simultaneously.             |
| **Ownership**        | The same thread that locks must unlock.                   | Any thread can signal (release).                                       |
| **Type**             | Binary only (locked/unlocked).                            | Binary or Counting.                                                    |
| **Usage**            | Protect critical sections from simultaneous access.       | Limit concurrent access to resources (like connection pools, buffers). |
| **Implementation**   | Lock and unlock mechanism.                                | Wait (acquire) and signal (release) mechanism.                         |
| **Example Use Case** | Writing to a shared file.                                 | Managing a fixed number of database connections.                       |
| **C++ Header**       | `<mutex>`                                                 | `<semaphore>` (C++20 and later)                                        |
| **Risk**             | Deadlock if not unlocked properly.                        | Over-release or incorrect counting can cause logic errors.             |
| **Analogy**          | One key for one room.                                     | Parking lot with multiple spaces.                                      |
<br><br>


# When to Use What
<br>

| Situation                                           | Use           |
| --------------------------------------------------- | ------------- |
| Only one thread should access data at a time        | **Mutex**     |
| Limited number of threads can access simultaneously | **Semaphore** |
| Need strict ownership (same thread unlocks)         | **Mutex**     |
| Threads can signal each other or release on behalf  | **Semaphore** |
