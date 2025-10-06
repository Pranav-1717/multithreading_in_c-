# std::recursive_mutex
A recursive mutex is a synchronization primitive in C++ threading that allows the owning thread to lock the mutex multiple times without causing a deadlock. This is crucial when dealing with recursive function calls or loops that access a critical section repeatedly within the same thread's execution path.

## The Deadlock Scenario
Consider a scenario where a function calls itself recursively, and the critical section is inside that function:
1. Thread T1 acquires the lock (`m.lock()`) on a standard `std::mutex (M)`.
2. The function calls itself recursively.
3. T1 reaches the `m.lock()` statement again.
4. Since M is already locked by T1, the standard mutex will force T1 to wait for the lock to be released s .
5. T1 is waiting for itself to release the lock, but it cannot proceed to the `unlock()` statement because it is stuck waiting. This creates a deadlock situation.

## The Solution: std::recursive_mutex

A recursive mutex is designed to check the identity of the thread attempting to acquire the lock:
- If the mutex is unlocked, the calling thread acquires the lock.
- If the mutex is locked by a different thread, the calling thread waits.
- If the mutex is already locked by the calling thread (the current owner), the lock is allowed to proceed immediately, and the thread continues execution, preventing the deadlock.

The core difference is that a recursive mutex is intelligent; it keeps a count of how many times the owning thread has locked it
>The number of times you lock a recursive mutex must be matched by the number of times you unlock it.

If a thread locks the recursive mutex 10 times (using `lock()` or `try_lock()`), it must call `unlock()` 10 times. If the unlock count does not match the lock count, the mutex is still considered locked by that thread, and other waiting threads cannot acquire it.

```c++
// Syntax for a recursive mutex
std::recursive_mutex m1;
int buffer = 0;

void recursive_function(int loop_for, int thread_id) {
    // Terminating condition for recursion
    if (loop_for < 0) {
        return;
    }

    // Lock the mutex
    m1.lock(); 
    
    // Critical Section
    cout << "Locked by Thread " << thread_id << " | Buffer: " << buffer++ <<endl;

    // Recursive Call
    recursive_function(loop_for - 1, thread_id); 

    // Unlock the mutex (Executed only when recursion unwinds)
    m1.unlock(); 
    cout << "Unlocked by Thread " << thread_id << endl;
}
```

1. Thread T1 calls recursive_function (e.g., 11 times).
2. T1 calls m1.lock() 11 times. Since it is the owner, the lock count increments each time, and T1 proceeds without waiting.
3. When the function hits the return condition, the recursion stops and the stack begins to unwind.
4. As the stack unwinds, the m1.unlock() statement is executed for each level of the recursion, decrementing the lock count 11 times.
5. Once the lock count reaches zero, the mutex is fully released, allowing a waiting thread (like T2) to acquire it.