#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>

std::queue<int> buffer;              // Shared buffer (queue)
const unsigned int MAX_BUFFER = 5;   // Max size of buffer
std::mutex mtx;                      // Mutex to protect shared buffer
std::condition_variable cv;          // Condition variable for signaling

bool finished = false;               // To stop consumer when done

// ------------------ PRODUCER ------------------
void producer() {
    for (int i = 1; i <= 10; ++i) {
        std::unique_lock<std::mutex> lock(mtx); // Lock mutex before accessing buffer

        // Wait while buffer is full
        cv.wait(lock, [] { return buffer.size() < MAX_BUFFER; });

        buffer.push(i);  // Produce item
        std::cout << "Produced: " << i << std::endl;

        lock.unlock();   // Unlock mutex before notifying
        cv.notify_all(); // Notify consumers that new data is available

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    // Signal consumer to stop
    {
        std::lock_guard<std::mutex> lock(mtx);
        finished = true;
    }
    cv.notify_all();
}

// ------------------ CONSUMER ------------------
void consumer() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);

        // Wait while buffer is empty and not finished
        cv.wait(lock, [] { return !buffer.empty() || finished; });

        if (buffer.empty() && finished) break;  // Exit condition

        int item = buffer.front();
        buffer.pop();
        std::cout << "Consumed: " << item << std::endl;

        lock.unlock();
        cv.notify_all(); // Notify producer that space is free

        std::this_thread::sleep_for(std::chrono::milliseconds(800));
    }
}

int main() {
    std::thread prod(producer);
    std::thread cons(consumer);

    prod.join();
    cons.join();

    std::cout << "All items produced and consumed successfully!" << std::endl;
    return 0;
}
