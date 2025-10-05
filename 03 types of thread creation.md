# Types of Creating Threads in C++

When we talk about threads in C++, think of them as little helpers in your house. Each helper can do some work for you at the same time. Instead of you doing everything alone, you assign tasks to helpers, and they get things done in parallel.<br>
But the real fun is: you can hire your helpers (threads) in different ways. Let’s explore the 5 most common ones.

## 1. Threads Using Free (Regular) Functions
A free function is just a normal function that is not part of a class. You can create a thread that runs this function in parallel with your main program.

Think of a free function as a helper you hire to do a specific task  like fetching water. You just tell them “Go do this job,” and they do it while you continue doing other things.

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
## 2. Threads Using Functions with Parameters
Sometimes you want your thread function to take input values like a name or a number.
This helps the thread do personalized tasks instead of one fixed job.
C++ allows you to pass arguments to the function directly when creating the thread.

Imagine you have a helper who greets people, but you want them to greet Alice three times and Bob five times.
So you pass them the name and number of times to greet.
Your helper follows your instructions exactly and does their job while you wait.

```c++
#include <iostream>
#include <thread>
using namespace std;

void greetNTimes(string name, int times) {
    for (int i = 0; i < times; ++i)
        cout << "Hello, " << name << "! (" << i+1 << ")\n";
}

int main() {
    thread t1(greetNTimes, "Alice", 3); // Start a thread with parameters
    t1.join(); // Wait for it to finish
    cout << "Greetings done.\n";
}
```

## 3. Threads Using Lambda Functions
Sometimes you just need to perform a tiny task — so tiny that creating a separate function feels like too much.
A lambda function lets you write a small, unnamed function directly where you use it.
You can start a thread with this lambda, making the code shorter and clearer.

```c++
#include <iostream>
#include <thread>
using namespace std;

int main() {
    thread t1([]() {
        cout << "Hello from a lambda thread!\n";
    });
    t1.join();
    cout << "Main thread finished.\n";
}
```
## 4. Threads Using Functors (Function Objects)
A functor is an object that behaves like a function.
It’s a class with a special `operator()` defined inside.
Threads can run these objects just like they run regular functions.
Functors are great when your thread needs to keep some internal data or state.

```c++
#include <iostream>
#include <thread>
using namespace std;

class Greeter {
public:
    void operator()() const {
        cout << "Hello from a functor thread!\n";
    }
};

int main() {
    Greeter g;
    thread t1(g);
    t1.join();
    cout << "Functor thread finished.\n";
}
```

## 5. Threads Using Class Member Functions
Threads can also run functions that belong to a class.
But since these functions belong to an object, you need to tell the thread which object it should use.
You do this by passing the function’s address and the object’s address.
This way, the thread knows whose function to call.
It’s perfect when you want each object to do its own work in parallel.

```c++
#include <iostream>
#include <thread>
using namespace std;

class Worker {
public:
    void doWork() {
        cout << "Working inside a class member function!\n";
    }
};

int main() {
    Worker w;
    thread t1(&Worker::doWork, &w); // Run w.doWork() in a new thread
    t1.join();
    cout << "Member function thread done.\n";
}
```

## 6. Threads Using Static Member Functions
A static member function doesn’t belong to any specific object it belongs to the class itself.
That means you can call it without creating an object.
Threads can run static functions easily, just like free functions.
They are great when the task doesn’t depend on any object’s data.
It’s a clean and simple way to keep code organized inside a class.

```c++
#include <iostream>
#include <thread>
using namespace std;

class Machine {
public:
    static void start() {
        cout << "Static function running in a thread!\n";
    }
};

int main() {
    thread t1(&Machine::start);
    t1.join();
    cout << "Static thread done.\n";
}
```

## 7. Threads Using Detached Threads
Usually, we wait for a thread to finish using `join()`.
But sometimes, we want a thread to run freely even if the main program moves on.
That’s when we use `detach()`.
A detached thread works independently and continues its job even if the main thread doesn’t wait.
But be careful if the program ends too soon, your detached thread might stop halfway!

```c++
#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

void longTask() {
    cout << "Starting long task...\n";
    this_thread::sleep_for(chrono::seconds(2));
    cout << "Long task finished!\n";
}

int main() {
    thread t1(longTask);
    t1.detach(); // Let it run independently
    cout << "Main thread continues immediately.\n";
    this_thread::sleep_for(chrono::seconds(3)); // Wait to see output
}
```

## 8. Threads Using std::async and std::future
`std::async` starts a function in a background thread and returns a special object called a future.
A future acts like a promise  it will hold the result once the thread is done.
You can do other work while it runs, then use `future.get()` to collect the result later.
It’s a safe and convenient way to manage threads that return values.
No need to manually create or join threads!

```c++
#include <iostream>
#include <future>
using namespace std;

int addNumbers(int a, int b) {
    return a + b;
}

int main() {
    future<int> result = async(addNumbers, 5, 7); // Run in background
    cout << "Doing something else...\n";
    cout << "Result: " << result.get() << endl;   // Wait and get result
}
```
