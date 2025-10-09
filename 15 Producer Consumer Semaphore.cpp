#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <semaphore>    // For std::counting_semaphore (C++20)
#include <chrono>

// ------------------ Shared Data Structures ------------------

// Shared buffer between producer and consumer
std::queue<int> buffer;

// Maximum number of items the buffer can hold at a time
const unsigned int MAX_BUFFER = 5;

// Mutex to protect shared access to the queue
std::mutex mtx;

// ------------------ Semaphores Explanation ------------------
//
// Semaphores are synchronization primitives that maintain a count.
// They control access to shared resources by multiple threads.
//
// We'll use TWO semaphores:
// 1. emptySlots  → counts how many empty spaces are left in the buffer.
// 2. fullSlots   → counts how many items are available for the consumer.
//
// Initially:
// - Buffer is empty, so there are MAX_BUFFER empty slots.
// - There are 0 full slots (no items yet).
//
// The template argument <MAX_BUFFER> sets the *maximum count* the semaphore can reach.
//

std::counting_semaphore<MAX_BUFFER> emptySlots(MAX_BUFFER);  // Initially all slots empty
std::counting_semaphore<MAX_BUFFER> fullSlots(0);            // Initially no items

// -------------------------------------------------------------
// ------------------ PRODUCER FUNCTION ------------------------
// -------------------------------------------------------------
void producer() {
    for (int i = 1; i <= 10; ++i) {

        // Wait (decrement) on emptySlots.
        // If emptySlots == 0, this thread blocks until consumer releases one.
        emptySlots.acquire();

        // Acquire mutex before modifying the shared queue.
        // std::lock_guard automatically unlocks when it goes out of scope.
        {
            std::lock_guard<std::mutex> lock(mtx);
            buffer.push(i);  // Add produced item to buffer
            std::cout << "Produced: " << i << std::endl;
        } // Mutex automatically released here.

        // Signal (increment) fullSlots → tells consumer that an item is ready.
        fullSlots.release();

        // Simulate production delay
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

// -------------------------------------------------------------
// ------------------ CONSUMER FUNCTION ------------------------
// -------------------------------------------------------------
void consumer() {
    for (int i = 1; i <= 10; ++i) {

        // Wait (decrement) on fullSlots.
        // If fullSlots == 0, it means there are no items to consume → block until producer adds something.
        fullSlots.acquire();

        int item;
        {
            // Lock mutex to safely access the shared buffer.
            std::lock_guard<std::mutex> lock(mtx);

            // Remove item from buffer front (FIFO)
            item = buffer.front();
            buffer.pop();
            std::cout << "Consumed: " << item << std::endl;
        } // Mutex automatically released.

        // Signal (increment) emptySlots → tells producer that there is now one more free slot.
        emptySlots.release();

        // Simulate consumption delay
        std::this_thread::sleep_for(std::chrono::milliseconds(800));
    }
}

// -------------------------------------------------------------
// ------------------ MAIN FUNCTION ----------------------------
// -------------------------------------------------------------
int main() {
    // Create producer and consumer threads
    std::thread prod(producer);
    std::thread cons(consumer);

    // Wait for both threads to complete their work
    prod.join();
    cons.join();

    std::cout << "All items produced and consumed successfully!" << std::endl;
    return 0;
}
