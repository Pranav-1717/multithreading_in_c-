// Synchronization: Avoiding Data Races
//   When multiple threads try to access and modify the same piece of data at the same time, 
//   race conditions can occur. A race condition happens when the program’s behavior depends on the unpredictable timing of threads. 
//   This can lead to incorrect results, crashes, or other unexpected behavior.

// Using std::mutex to Protect Shared Data
// A mutex (short for “mutual exclusion”) is a tool that ensures only one thread can access a piece of code or data at a time. 
// This prevents race conditions.

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

using namespace std;

mutex mtx; // Mutex to protect shared output

// Function executed by each thread
void safePrint(int id) {
    mtx.lock(); // Lock the mutex before accessing shared data
    cout << "Thread " << id << " is running\n";
    mtx.unlock(); // Unlock the mutex after finishing
}

int main() {
    const int numThreads = 5; // Number of threads
    vector<thread> threads;

    // Launch threads
    for (int i = 1; i <= numThreads; ++i) {
        threads.push_back(thread(safePrint, i));
    }

    // Wait for all threads to finish
    for (auto &t : threads) {
        t.join();
    }

    cout << "All threads have finished.\n";
    return 0;
}


// Output (order may vary):
// Thread 3 is running
// Thread 1 is running
// Thread 5 is running
// Thread 2 is running
// Thread 4 is running
// All threads have finished.