# Types of Creating Threads in C++

When we talk about threads in C++, think of them as little helpers in your house. Each helper can do some work for you at the same time. Instead of you doing everything alone, you assign tasks to helpers, and they get things done in parallel.<br>
But the real fun is: you can hire your helpers (threads) in different ways. Let’s explore the 5 most common ones.

## 1. Threads Using Free (Regular) Functions
A free function is just a normal function that is not part of a class. You can create a thread that runs this function in parallel with your main program.

Think of a free function as a helper you hire to do a specific task — like fetching water. You just tell them “Go do this job,” and they do it while you continue doing other things.

```c++
#include <iostream>
#include <thread>
using namespace std;

void sayHello() {
    cout << "Hello from a thread!\n";
}

int main() {
    thread t1(sayHello); // Start a new thread to run sayHello
    t1.join(); // Wait for it to finish
    cout << "Thread finished.\n";
}
```
