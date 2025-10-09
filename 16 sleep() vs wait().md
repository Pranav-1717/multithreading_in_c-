# Sleep() vs Wait()


### Sleep()
```c++
std::this_thread::sleep_for(std::chrono::milliseconds(1000));
```

- Pauses the thread for a fixed duration.
- Thread **does not release any locks** it holds.
- Useful for **simulating delays** or throttling execution.

```c++
std::lock_guard<std::mutex> lock(mtx);
std::cout << "Producer working..." << std::endl;
std::this_thread::sleep_for(std::chrono::seconds(1)); // just a delay
```

### Wait()
```c++
std::unique_lock<std::mutex> lock(mtx);
cv.wait(lock, [] { return condition_met; });
```
- Pauses the thread until a **specific condition is met**.
- Used with **mutex** and **condition_variable**.
- **Releases the lock temporarily** while waiting, and automatically re-locks before continuing.
- Useful for **synchronizing threads**.

```c++
std::unique_lock<std::mutex> lock(mtx);
cv.wait(lock, [] { return !buffer.empty(); });  // wait until item available
int item = buffer.front();
buffer.pop();
```

---

## Comparison Table

| Feature | sleep() | wait() |
|---------|---------|--------|
| Purpose | Pause for fixed time | Pause until condition is met |
| Lock behavior | Keeps lock | Releases and re-acquires lock |
| Trigger to resume | Time expires | Condition + notification |
| Header | `<thread>` | `<condition_variable>` |
| Typical use | Simulate delay, throttle loops | Thread synchronization |

---

## Use Case Example

In **Producer–Consumer problems**:
- `wait()` is used to **coordinate producer and consumer threads** safely.
- `sleep()` is used only to **simulate work delay** if needed.

