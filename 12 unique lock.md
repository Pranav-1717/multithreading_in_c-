# std::unique_lock

- **Flexible Locking Strategies:** Supports `defer_lock`, `try_to_lock`, and `adopt_lock`.
- **Automatic Unlocking:** Unlocks the mutex automatically when the `unique_lock` goes out of scope.
- **Movable but Non-Copyable:** Ownership can be transferred between `unique_lock` objects.
- **Condition Variable Compatibility:** Works seamlessly with `std::condition_variable`.

```c++
std::unique_lock<std::mutex> lock(m, lock_type);
//lock type -: 1)defer_lock
//             2)adapt_lock
//             3)try_to_lock
std::unique_lock<std::mutex> lock(m);
//If you do not passed any type then default behaviour is seen (lock immedialtely).
```

## 1: Deferred Locking (std::defer_lock)

```c++
#include <iostream>
#include <thread>
#include <mutex>

std::mutex m1;
int buffer = 0;

void task(int thread_num) {
    std::cout << "Thread " << thread_num << " doing some work before locking.\n";

    std::unique_lock<std::mutex> lock(m1, std::defer_lock); // Mutex not locked yet
    // Do other work before acquiring the lock
    lock.lock(); // Now acquire the lock

    for (int i = 0; i < 5; ++i) {
        buffer++;
        std::cout << "Thread " << thread_num << " incremented buffer to " << buffer << "\n";
    }
    // Automatic unlock when 'lock' goes out of scope
}

int main() {
    std::thread t1(task, 1);
    std::thread t2(task, 2);

    t1.join();
    t2.join();

    std::cout << "Final buffer value: " << buffer << "\n";
    return 0;
}
```
The mutex is not locked at construction. The thread can perform other operations and explicitly lock later using lock.lock().

## 2: Adopting a Pre-Locked Mutex (std::adopt_lock)
Sometimes, you manually lock a mutex before creating a unique_lock. Using adopt_lock tells unique_lock:
>“Hey, I already locked this mutex. Please just manage unlocking it for me.”

```c++
#include <iostream>
#include <mutex>

std::mutex m;

int main() {
    m.lock(); // Lock manually
    std::unique_lock<std::mutex> lock(m, std::adopt_lock); // Take ownership
    std::cout << "Mutex is managed by unique_lock now\n";
    // No need to call m.unlock(); it will be unlocked automatically
}
```
Think of adopt_lock as “I locked it myself, now you take care of unlocking.”

## 3: Try-to-Lock (std::try_to_lock)
Sometimes, you don’t want your thread to wait forever for a mutex. try_to_lock tries to acquire the mutex once. If it’s busy, it just moves on.

```c++
#include <iostream>
#include <thread>
#include <mutex>

std::mutex m;

void task() {
    std::unique_lock<std::mutex> lock(m, std::try_to_lock);
    if (lock.owns_lock()) {
        std::cout << "Got the lock!\n";
    } else {
        std::cout << "Lock busy, skipping work.\n";
    }
}

int main() {
    std::thread t1(task);
    std::thread t2(task);

    t1.join();
    t2.join();
}
```
Attempts to acquire the lock without blocking. The thread proceeds even if the mutex is not available.