//
// Created by janam on 3/24/2025.
//

#include "DungeonManager.h"
#include "Utility.h"

DungeonManager::DungeonManager() : instanceSemaphore(0) {
}

void DungeonManager::queuePlayers()
{
    std::lock_guard<std::mutex> lock(queueMutex);

    for (int i = 0; i < tanks; i++) tankQueue.push(i);
    for (int i = 0; i < healers; i++) healerQueue.push(i);
    for (int i = 0; i < dps; i++) dpsQueue.push(i);

    std::cout << "[Queue] Players added. Matching parties...\n";
    partyAvailable.notify_all();
}

void DungeonManager::processQueue() {
    while (true) {
        std::unique_lock<std::mutex> lock(queueMutex);
        partyAvailable.wait(lock, [this]() {
            return (tankQueue.size() > 0 && healerQueue.size() > 0 && dpsQueue.size() >= 3);
        });

        std::cout << "[Debug] Party found! Forming a party...\n";

        // Form party
        tankQueue.pop();
        healerQueue.pop();
        dpsQueue.pop();
        dpsQueue.pop();
        dpsQueue.pop();

        lock.unlock();  // Unlock queue so more parties can form

        std::cout << "[Debug] Waiting for available dungeon instance...\n";
        instanceSemaphore.acquire();

        std::cout << "[Debug] Dungeon slot available, looking for empty instance...\n";

        // 🔹 Lock instance selection to prevent multiple threads from picking the same instance
        std::lock_guard<std::mutex> instanceLock(queueMutex);
        for (DungeonInstance &instance : instances) {
            if (!instance.isActive()) {
                instance.setActive(true);  // ✅ Mark instance as active **before** assigning it!
                std::cout << "[Debug] Assigning party to Dungeon " << instance.getInstanceID() << "\n";
                instanceThreads.emplace_back(&DungeonInstance::start, &instance, t1, t2);
                break;
            }
        }

        std::cout << "[Debug] Party processing completed, checking for next party...\n";
    }
}





void DungeonManager::displayStatus()
{
    std::lock_guard<std::mutex> lock(queueMutex);

    // Display queue status
    std::cout << "\n=== Queue Status ===\n";
    std::cout << "Tanks waiting: " << tankQueue.size() << std::endl;
    std::cout << "Healers waiting: " << healerQueue.size() << std::endl;
    std::cout << "DPS waiting: " << dpsQueue.size() << std::endl;

    // Display dungeon instance status
    std::cout << "\n=== Dungeon Instance Status ===\n";
    for (auto &instance : instances) {
        std::cout << "[Dungeon " << instance.getInstanceID() << "] Status: "
                  << (instance.isActive() ? "Active" : "Empty") << std::endl;
    }
}

void DungeonManager::displaySummary() {
    int totalParties = 0;
    for (auto &instance : instances) {
        totalParties += instance.getPartiesServed();
    }

    std::cout << "\n=== Dungeon Summary ===\n";
    for (auto &instance : instances) {
        std::cout << "[Dungeon " << instance.getPartiesServed()
                  << "] Parties Served: " << instance.getPartiesServed() << "\n";
    }
    std::cout << "Total Parties Served: " << totalParties << "\n";
}

void DungeonManager::displayInitialization()
{
    std::cout << "\n=== Initialization ===\n";
    std::cout << "Enter max concurrent dungeon instances: ";
    std::cin >> maxInstance;
    std::cout << "Enter number of tanks in queue: ";
    std::cin >> tanks;
    std::cout << "Enter number of healers in queue: ";
    std::cin >> healers;
    std::cout << "Enter number of DPS in queue: ";
    std::cin >> dps;
    std::cout << "Enter min dungeon time (t1): ";
    std::cin >> t1;
    std::cout << "Enter max dungeon time (t2): ";
    std::cin >> t2;

    instanceSemaphore.release(maxInstance);

    for (int i = 0; i < maxInstance; i++) {
        instances.emplace_back(i + 1);
    }

    displaySettings();
    displayDivider();
}

void DungeonManager::displaySettings()
{
    std::cout << "\n=== Settings ===\n";
    std::cout << "n = " << maxInstance << std::endl;
    std::cout << "Tanks = " << tanks << std::endl;
    std::cout << "Healers = " << healers << std::endl;
    std::cout << "DPS = " << dps << std::endl;
    std::cout << "Min time = " << t1 << std::endl;
    std::cout << "Max time = " << t2 << std::endl;

}
DungeonManager::~DungeonManager() {
    for (auto &t : instanceThreads) {
        if (t.joinable()) {
            t.join();
        }
    }
}