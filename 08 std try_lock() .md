# std::try_lock()

When working with multiple threads in C++, you often need to protect several shared resources at once. If you try to lock them one by one, you risk a deadlock.
The standard library provides a powerful global function, `std::try_lock`, that solves this problem elegantly by attempting to lock multiple lockable objects simultaneously.

`mutex::try_lock()` -> member function -> used on a single mutex object.<br>
`std::try_lock` -> global function -> takes any number of lockable objects.

```
std::try_lock(m1, m2, m3, ...)
```
- It replaces multiple lines of individual try_lock calls with a single, clean function call.
- Non blocking.
- Prevents partial locking. If fails to lock any resource then release other also.

### "All or Nothing" Rule
Imagine you need to lock two mutexes, m1 and m2, to access two variables, X and Y, in a critical section.
- `std::try_lock` attempts to lock m1. (Success)
- It then attempts to lock m2. (Failure)

Because m2 failed, `std::try_lock` immediately releases the lock it held on m1 before returning to your code. This guarantees that if you enter the critical section, you have all the locks you need.


| **Return Value** | **Meaning**                                                                              | **Action to Take**                                                                                                                                      |
| ---------------: | ---------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------- |
|           **-1** | **Success!** — All lockable objects were successfully locked.                          | Enter the **critical section**, perform your work, and then **explicitly unlock** the mutexes later (e.g., using `unlock()` or `std::lock_guard`).      |
| **0 or greater** | **Failure.** The value is the **0-based index** of the mutex that could not be locked. | Skip the critical section — all previously locked mutexes have been **automatically released**. Take alternative action (retry later, log error, etc.). |

```
For example, if you call std::try_lock(m1, m2, m3):
- If m3 fails, the function returns 2 (since m3 is at index 2).
- If it returns -1, both m1, m2, and m3 are locked.
```

---

## Code Example:
Here is how you would typically use `std::try_lock` in a consumer thread that needs to access two resources (X and Y) protected by two mutexes (m1 and m2) simultaneously.

```c++
#include <mutex>
#include <iostream>

std::mutex m1, m2;
int X = 0;
int Y = 0;

void consume_data() {
    // Tries to lock BOTH m1 and m2 simultaneously
    int lock_result = std::try_lock(m1, m2);

    // Check for success
    if (lock_result == -1) {
        // Success: We have both locks!
        
        if (X != 0 && Y != 0) {
            int consumed_value = X + Y;
            X = 0; // Emptying the "buckets"
            Y = 0;
            std::cout << "Consumed: " << consumed_value << std::endl;
        }

        // CRITICAL: We must explicitly unlock them after success
        m1.unlock();
        m2.unlock();
    } else {
        // Failure: lock_result is 0 (m1 failed) or 1 (m2 failed)
        // Since we failed, all locks were automatically released.
        // We can now do something else without waiting.
        std::cout << "Failed to acquire both locks. Trying again later." << std::endl;
    }
}

// ... main function and thread creation would go here
```

In this example, the consumer thread only proceeds if it successfully locks both m1 and m2 (when lock_result == -1). If it fails, it simply prints a message and moves on, ensuring the thread never blocks and never causes a deadlock by holding one lock while waiting for another.
