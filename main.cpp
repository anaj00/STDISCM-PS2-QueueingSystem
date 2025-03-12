#include <iostream>
#include <thread>
#include <semaphore>
#include <queue>
#include <vector>
#include <random>
#include <chrono>
#include <mutex>

using namespace std;

// Semaphore for controlling max concurrent instances
std::counting_semaphore<> dungeonSemaphore(0); // Will be initialized with n dynamically
std::mutex outputMutex; // To prevent race conditions in console output

// Player Queues (FIFO)
queue<int> tankQueue;
queue<int> healerQueue;
queue<int> dpsQueue;

// Global stats
int totalPartiesServed = 0;
int totalTimeSpent = 0;

// Random time generator for dungeon clear time
int getRandomTime(int t1, int t2) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(t1, t2);
    return dis(gen);
}

// Dungeon Instance Thread
void dungeonInstance(int instanceID, int t1, int t2) {
    while (true) {
        dungeonSemaphore.acquire(); // Wait for a party to be assigned

        // Simulate dungeon clearing time
        int clearTime = getRandomTime(t1, t2);
        {
            std::lock_guard<std::mutex> lock(outputMutex);
            cout << "[Instance " << instanceID << "] Party started! Clearing in " << clearTime << " seconds...\n";
        }
        this_thread::sleep_for(chrono::seconds(clearTime));

        // Update global stats
        {
            std::lock_guard<std::mutex> lock(outputMutex);
            totalPartiesServed++;
            totalTimeSpent += clearTime;
            cout << "[Instance " << instanceID << "] Party finished! (Time: " << clearTime << "s)\n";
        }
    }
}

// Function to check if a party can be formed
bool formParty() {
    if (tankQueue.size() >= 1 && healerQueue.size() >= 1 && dpsQueue.size() >= 3) {
        tankQueue.pop();
        healerQueue.pop();
        dpsQueue.pop();
        dpsQueue.pop();
        dpsQueue.pop();
        return true;
    }
    return false;
}

// Party Assignment Manager
void assignParties(int n) {
    while (true) {
        if (formParty()) {
            dungeonSemaphore.release(); // Allow a dungeon instance to proceed
        } else {
            break; // Exit if no more parties can be formed
        }
    }
}

int main() {
    // User input
    int n, t, h, d, t1, t2;
    cout << "Enter max concurrent dungeon instances (n): ";
    cin >> n;
    cout << "Enter number of tanks in queue (t): ";
    cin >> t;
    cout << "Enter number of healers in queue (h): ";
    cin >> h;
    cout << "Enter number of DPS in queue (d): ";
    cin >> d;
    cout << "Enter min dungeon clear time (t1): ";
    cin >> t1;
    cout << "Enter max dungeon clear time (t2): ";
    cin >> t2;

    // Initialize semaphore with max concurrent dungeons
    dungeonSemaphore.release(n);

    // Fill role queues
    for (int i = 0; i < t; i++) tankQueue.push(1);
    for (int i = 0; i < h; i++) healerQueue.push(1);
    for (int i = 0; i < d; i++) dpsQueue.push(1);

    // Start dungeon instances
    vector<thread> instanceThreads;
    for (int i = 0; i < n; i++) {
        instanceThreads.emplace_back(dungeonInstance, i, t1, t2);
    }

    // Assign parties
    assignParties(n);

    // Allow some time before ending (simulating a live system)
    this_thread::sleep_for(chrono::seconds(5));

    // Display final summary
    {
        std::lock_guard<std::mutex> lock(outputMutex);
        cout << "\n--- Dungeon Summary ---\n";
        cout << "Total Parties Served: " << totalPartiesServed << endl;
        cout << "Total Time Spent in Dungeons: " << totalTimeSpent << " seconds\n";
    }

    // Cleanup threads
    for (auto& th : instanceThreads) {
        th.detach(); // Running indefinitely, so we detach
    }

    return 0;
}
