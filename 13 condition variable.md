# Condition Variables

They synchronize threads based on specific conditions.<br>
They allow threads to wait until a certain condition is met and then be notified to resume execution.

### ***Two primary purposes:***
1. ***Notifying Other Threads*** : 
    One thread can notify other threads that a condition they are waiting for has been met.
    - `notify_one()`: Wakes up one of the waiting threads.
    - `notify_all()`: Wakes up all threads currently waiting on the condition variable.

2. ***Waiting for Conditions*** : 
    - Threads can wait on a condition variable until a specific condition becomes true.
    - When waiting, a thread releases the associated mutex and suspends execution until notified and the condition is met.

```c++
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

int balance = 0; // Shared resource
std::mutex m;
std::condition_variable cv;

void withdrawMoney(int money) {
    std::unique_lock<std::mutex> ul(m); // Acquire lock
    cv.wait(ul, [] { return balance != 0; }); // Wait if balance is zero, releases lock
    // After notification, lock is reacquired, and condition is rechecked.

    if (balance >= money) {
        balance -= money;
        std::cout << "Amount Deducted: " << money << std::endl;
    } else {
        std::cout << "Amount can't be deducted. Current balance is less than " << money << std::endl;
    }
    std::cout << "Current Balance: " << balance << std::endl;
}

void addMoney(int money) {
    std::lock_guard<std::mutex> lg(m); // Acquire lock
    balance += money;
    std::cout << "Amount Added: " << money << std::endl;
    std::cout << "Current Balance: " << balance << std::endl;
    cv.notify_one(); // Notify one waiting thread
}

int main() {
    std::thread t1(withdrawMoney, 600); // Tries to withdraw 600
    std::thread t2(addMoney, 500);    // Adds 500

    t1.join();
    t2.join();

    return 0;
}
```

## How the Code Works
1. ***Initial State :*** balance is 0. t1 (withdraw) and t2 (add) threads are created.

2. ***`withdrawMoney` (Thread 1) Execution :***
    - t1 acquires a lock using `std::unique_lock<std::mutex> ul(m)`.
    - It then calls `cv.wait(ul, [] { return balance != 0; })`.
    - The predicate balance != 0 is false (since balance is 0).
    - `cv.wait()` releases the mutex m and puts t1 to sleep, waiting for a notification.

3. ***`addMoney` (Thread 2) Execution :***
    - Since m is released, t2 can now acquire the lock using `std::lock_guard<std::mutex> lg(m)`.
    - It adds 500 to balance, making balance = 500.
    - `cv.notify_one()` is called, which wakes up t1.

4. ***`withdrawMoney` (Thread 1) Resumes :***
    - t1 wakes up and reacquires the mutex m .
    - It rechecks the predicate balance != 0 . This time, balance is 500, so the predicate is true.
    - t1 proceeds to check if balance >= money (500 >= 600), which is false. It prints that the amount cannot be deducted and the current balance is 500. 