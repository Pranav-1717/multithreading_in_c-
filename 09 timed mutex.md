# std::timed_mutex

## 1. The Issue with std::mutex::lock()
If Thread 1 (T1) acquires the lock using m.lock(), Thread 2 (T2) must wait indefinitely at its own m.lock() call until T1 calls m.unlock().

>If T1 takes a long time (e.g., 1 minute) to finish its work, T2 will wait for that entire duration. This is considered a time waste if T2 has other, non-critical tasks it could be performing instead of blocking indefinitely.


## 2. The Issue with std::mutex::try_lock()
To avoid indefinite waiting, C++ offers std::mutex::try_lock(). If T1 holds the lock, T2's try_lock() call will immediately return false and T2 will proceed to do something else right away.

The limitation here is the immediacy. T2 doesn't wait at all. Sometimes, you want T2 to wait for a short, specific amount of time before giving up and moving on.

## The Solution: std::timed_mutex
It will block until a timeout period has elapsed OR the lock is acquired.
- If the lock is acquired within the given time, it returns true.
- If the timeout period elapses before the lock is acquired, it returns false.

This allows Thread 2 to say: "I am ready to wait for this lock for 100 milliseconds, but if I don't get it by then, I will move on to other work".

***`1. try_lock_for()`***
This function waits for a specified time duration to acquire the lock.

We have a global variable my_amount and two threads, T1 and T2, trying to access a critical section protected by a std::timed_mutex M.
```c++
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>

using namespace std;

std::timed_mutex M;
int my_amount = 0;

void run_thread(int id) {
    // Tries to acquire the lock for a duration of 1 second
    if (M.try_lock_for(std::chrono::seconds(1))) {
        // Critical Section (Lock Acquired)
        my_amount++;
        std::this_thread::sleep_for(std::chrono::seconds(2)); // T1 holds lock for 2s
        cout << "Thread " << id << " entered" <<endl;
        M.unlock();
    } else {
        // Lock not acquired within 1 second
        cout << "Thread " << id << " could not enter" << endl;
    }
}

int main() {
    std::thread t1(run_thread, 1);
    std::thread t2(run_thread, 2);
    t1.join();
    t2.join();
    cout << "Final amount: " << my_amount << endl;
    return 0;
}
```
1. T1 acquires the lock first. Since the mutex was initially unlocked, T1 immediately gets the lock without waiting for the full 1 second specified in try_lock_for.
2. T1 enters the critical section, increments my_amount to 1, and then sleeps for 2 seconds while holding the lock.
3. T2 attempts to lock. T2 calls M.try_lock_for(std::chrono::seconds(1)). T2 is ready to wait for 1 second.
4. Since T1 holds the lock for 2 seconds, and T2 is only willing to wait for 1 second, the timeout occurs for T2.
5. T2's try_lock_for returns false, and T2 executes the else block, printing "Thread 2 could not enter".
6. The final output for my_amount is 1, because only T1 successfully entered and incremented the variable.

If we change T1's sleep time to 1 second and T2's wait time to 2 seconds:
```c++
// T1 holds lock for 1s
std::this_thread::sleep_for(std::chrono::seconds(1)); 

// T2 waits for 2s
if (M.try_lock_for(std::chrono::seconds(2))) { 
    // ...
}
```
1. T1 acquires the lock and holds it for 1 second.
2. T2 attempts to lock and is ready to wait for 2 seconds.
3. T1 finishes its work and calls M.unlock() after 1 second.
4. Since T2 was waiting and its 2-second timeout had not yet elapsed, T2 successfully acquires the lock after T1 releases it.
5. T2 enters the critical section, increments my_amount again.
6. The final output for my_amount is 2, and both threads print that they entered.


***`2. try_lock_until()`***
This function waits until a specified time point (a specific moment in time) to acquire the lock.

>Similar to try_lock_for(), but instead of providing a duration (e.g., 1 second), you provide a specific time point in the future (e.g., "wait until 10:00:00 AM").

Instead of specifying a duration (e.g., 1 second), you calculate the future time point from the current time using std::chrono::steady_clock::now() and add the desired duration to it

```c++
// Wait until the current time plus 1 second
auto timeout_time = std::chrono::steady_clock::now() + std::chrono::seconds(1);

if (M.try_lock_until(timeout_time)) {
    // ...
}
```


