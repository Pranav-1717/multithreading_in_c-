# Asynchronous Programming: std::promise and std::future

- `std::promise`: The "sender" or "producer" of a value. It sets a value (or an exception) for future retrieval.
- `std::future`: The "receiver" or "consumer" of the value. It waits for and retrieves the value set by std::promise.

### working Analogy :
Think of it like ordering a custom item: you get a receipt (std::future), and the shop promises to call when it's ready (std::promise). You continue your tasks, and the shop works on your item. Once complete, they "set" the item to your receipt, and you "get" it.

```c++
#include <iostream>
#include <vector>
#include <thread>
#include <future>
#include <numeric>  // for iota()

using namespace std;

// Function that calculates the sum of all numbers in a vector
// The result will be sent back using a promise
void findSum(promise<long long>&& sumPromise, const vector<int>& numbers) {
    long long sum = 0;

    // Go through each number and add it to the total
    for (int num : numbers) {
        sum += num;
    }

    // Set the result (sum) into the promise
    sumPromise.set_value(sum);
}

int main() {
    // Create a vector of 1,000,000 integers
    vector<int> numbers(1000000);

    // Fill the vector with numbers 1, 2, 3, ..., 1,000,000
    iota(numbers.begin(), numbers.end(), 1);

    // Create a promise object to send data from the worker thread
    promise<long long> sumPromise;

    // Get a future object to receive the result later
    future<long long> sumFuture = sumPromise.get_future();

    // Start a new thread to calculate the sum
    // 'move' transfers ownership of the promise to the new thread
    // 'cref' passes the vector by constant reference to avoid copying it
    thread worker(findSum, move(sumPromise), cref(numbers));

    // Main thread can do something else while the worker is running
    cout << "Main thread is doing other work..." << endl;
    this_thread::sleep_for(chrono::milliseconds(100)); // simulate some work

    // Wait for the result from the worker thread
    // get() will block until the promise sets a value
    long long result = sumFuture.get();

    // Print the result
    cout << "Sum calculated by worker thread: " << result << endl;

    // Wait for the worker thread to finish before exiting
    worker.join();

    return 0;
}
```

1. `std::promise<long long> sumPromise;`: Creates a promise for a long long sum.
2. `std::future<long long> sumFuture = sumPromise.get_future();`: Gets the future object from the promise.
3. `std::thread workerThread(findSum, std::move(sumPromise), std::ref(numbers));`: Launches findSum in a new thread, moving sumPromise to ensure single ownership and fulfillment.
4. `sumPromise.set_value(sum);`: In findSum, this makes the calculated sum available via sumFuture.
5. `long long result = sumFuture.get();`: In main, this call blocks until sumPromise.set_value() is called, then retrieves the result.
6. `workerThread.join();`: Ensures the worker thread completes before main exits.

### Use Cases
- Long-running computations: Offload heavy tasks to avoid blocking.
- Parallel tasks: Execute multiple independent computations simultaneously.
- Producer-consumer patterns: One thread produces a result, another consumes it.