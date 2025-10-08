# std::lock_guard
It provides an automatic mechanism for managing mutexes based on the Scope-Bound Resource Management (SBRM) or Resource Acquisition Is Initialization (RAII) principle.<br>
 It's a lightweight wrapper for owning a mutex on a scoped basis.

 ```c++
 #include <iostream>
#include <thread>
#include <mutex>

int buffer = 0;
std::mutex m1;

void task(int thread_id) {
    for (int i = 0; i < 10; ++i) {
        std::lock_guard<std::mutex> lock(m1); // Acquire lock automatically
        buffer++;
        std::cout << "Thread " << thread_id << " incremented buffer to " << buffer << std::endl;
    }
    // Lock is automatically released when 'lock' goes out of scope.
}

int main() {
    std::thread t0(task, 0);
    std::thread t1(task, 1);

    t0.join();
    t1.join();

    std::cout << "Final buffer value: " << buffer << std::endl;
    return 0;
}
```

## Features
1. Automatic Locking and Unlocking :
    - When an std::lock_guard object is created, it attempts to acquire a lock on the provided mutex immediately. If the mutex is already locked by another thread, the std::lock_guard will block until the lock can be acquired.
    - When the std::lock_guard object goes out of scope its destructor is automatically called. The destructor then releases the mutex  ensuring that the lock is always freed.

2. Non-Copyable : 
    - std::lock_guard objects cannot be copied, preventing unintended transfer of mutex ownership.

3. Lightweight : 
    - It is a very lightweight wrapper, incurring minimal overhead