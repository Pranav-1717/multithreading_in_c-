# C++ Mutex: lock() vs. try_lock()

When working with multithreading in C++, protecting shared resources (like a global counter) is essential to prevent race conditions. The standard way to do this is using a mutex (mutual exclusion).

But C++ gives us two main ways to acquire a lock: the blocking `lock()` and the non-blocking `try_lock()` .

## 1. The Blocking Approach: mutex.lock()
The `lock()` is traditional blocking way to protect critical section.

Key Behaviour:
- **Wait and Acquire** -: If mutex is held by another thread then it wait till lock is released.
- **Guaranteed Execution** -: This guarantees that the critical section code will eventually execute.

```c++
void increaseCounter_Lock() {
    for (int i = 0; i < 100000; ++i) {
        // The thread waits here until the lock is acquired.
        m.lock(); 
        counter++;
        m.unlock();
    }
}
// Result: Always 200,000
```
If two threads each try to increment a counter 100,000 times using `lock()`, the final result will always be 200,000 because no increment is ever skipped, the threads simply wait their turn.

## 2. The Non-Blocking Approach: mutex.try_lock()

**Try and Return:** -: 
-  `true` -> If lock is acquired.
-  `false` -> If lock is not acquired.

**No waiting** -: If the lock is busy, the thread does not get blocked, it returns false and continues executing the rest of its code.

```c++
void increaseCounter_TryLock() {
    for (int i = 0; i < 100000; ++i) {
        // We MUST use an if condition to check the return value
        if (m.try_lock()) { 
            counter++;
            m.unlock();
        }
        // If try_lock() fails (returns false), the counter++ is skipped.
    }
}
// Result: Random value < 200,000
```

When two threads use `try_lock()`, the final counter value is often less than 200,000 and is usually a random number (e.g., 176,000 or 185,000). This happens because when a thread fails to get the lock, it skips that iteration of the counter increment entirely.
<br>
<br>



| **Feature**         | **`mutex.lock()`**              | **`mutex.try_lock()`**                     |
| ------------------- | ------------------------------- | ------------------------------------------ |
| **Blocking**        | Yes — waits until mutex is free | No — returns immediately                   |
| **Return Type**     | `void`                          | `bool` (`true` if locked, `false` if busy) |
| **When Mutex Busy** | Thread waits (blocks)           | Returns `false`                            |
| **Guarantee**       | Always locks (when possible)    | May skip if lock not acquired              |
| **Use Case**        | Must access shared data         | Optional / non-critical work               |



