// A thread is the smallest unit of execution. A program can contain:
// 	Single-threaded execution: one task at a time.
// 	Multi-threaded execution: multiple tasks in parallel.

#include <iostream>
#include <thread>

using namespace std;

void printMessage() {
    cout << "Hello from thread!\n";
}

int main() {
    thread t1(printMessage); // start a new thread
    t1.join(); // wait for thread to finish

    thread t2(printMessage);
    t2.detach(); // let thread run independently
    cout << "Hello from main!\n";
    return 0;
}


// Without join() or detach(), the program may crash.
//  join(): The main thread waits for the child thread to finish.
// 	detach(): The child thread runs independently in the background.

// 👉Note: Detached threads may still be running when the main program exits,
// which can lead to undefined behavior if they access resources that have been cleaned up.



