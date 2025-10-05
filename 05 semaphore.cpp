//A semaphore is a synchronization primitive that controls access to a shared resource among multiple threads.
//hink of it as a counter + waiting mechanism.

// It keeps track of how many "permits" are available.
// Threads acquire a permit before entering a critical section.
// Threads release a permit when they leave.

// Imagine a parking lot with 3 spaces.
//   When a car enters → one space (permit) is taken.
//   When a car leaves → one space is released.
//   When it’s full → new cars wait.


//Types of Semaphores:
// 1. Binary Semaphore -> Only 0 or 1 permit (like a mutex). -> Mutual exclusion (lock/unlock)
// 2. Counting Semaphore -> Holds a count > 1 -> Limit access to N resources.

#include <iostream>
#include <thread>
#include <semaphore>
#include <vector>

using namespace std;

// Create a counting semaphore object with a maximum count of 2.
// Syntax: std::counting_semaphore<MAX_COUNT> semaphore_object(initial_value);
counting_semaphore<2> sem(2);
/*
Meaning:
- Template parameter <2> → sets the maximum number of permits this semaphore can hold (compile-time limit).
- Constructor argument (2) → sets the *initial count*, i.e., how many threads can acquire() right now without waiting.

So initially:
    sem count = 2  (two threads can enter simultaneously)
*/

void worker(int id) {
    sem.acquire(); 
    /*
    ➤ Function: acquire()
       - Tries to decrease the semaphore count by 1.
       - If count > 0, it decrements immediately and continues.
       - If count == 0, the calling thread **blocks (waits)** until another thread calls release().

    Example timeline:
        - Initial sem = 2
        - Thread 0 calls acquire() → sem = 1 (enters)
        - Thread 1 calls acquire() → sem = 0 (enters)
        - Thread 2 calls acquire() → sem = 0, must wait!
    */
    cout << "Thread " << id << " entered critical section.\n";

    this_thread::sleep_for(chrono::seconds(1)); // Simulate work

    cout << "Thread " << id << " leaving critical section.\n";
    
    sem.release(); 
     /*
    ➤ Function: release()
       - Increments the semaphore count by 1.
       - If any threads are blocked waiting in acquire(), one of them will be woken up.
       - Allows the next waiting thread to proceed.

    Example:
        Before: sem = 0
        After: sem = 1 (another thread can now enter)
    */

}

int main() {
    vector<thread> threads;
    for (int i = 0; i < 5; ++i)
        threads.emplace_back(worker, i);

    for (auto &t : threads)
        t.join();

    cout << "All threads finished.\n";
}

// Output (order may vary):
// Thread 0 entered critical section.
// Thread 1 entered critical section.
// Thread 0 leaving critical section.
// Thread 2 entered critical section.
// Thread 1 leaving critical section.
// Thread 3 entered critical section.
// Thread 2 leaving critical section.
// Thread 4 entered critical section.
// Thread 3 leaving critical section.
// Thread 4 leaving critical section.
// All threads finished.

