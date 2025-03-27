//
// Created by janam on 3/24/2025.
//

#include "DungeonManager.h"

#include <algorithm>


DungeonManager::DungeonManager() : instanceSemaphore(0) {
}

void DungeonManager::queuePlayers()
{
    std::lock_guard<std::mutex> lock(queueMutex);

    for (int i = 0; i < tanks; i++) tankQueue.push(i);
    for (int i = 0; i < healers; i++) healerQueue.push(i);
    for (int i = 0; i < dps; i++) dpsQueue.push(i);
}

void DungeonManager::processQueue() {
    while (true) {
        std::unique_lock<std::mutex> queueLock(queueMutex);

        // Wait for an available dungeon instance
        instanceAvailable.wait(queueLock, [this]() {
            for (DungeonInstance &instance : instances) {
                if (!instance.isActive()) {
                    return true;
                }
            }
            return false;
        });

        // Wait for a party to be available
        partyAvailable.wait(queueLock, [this]() {
            return (tankQueue.size() > 0 && healerQueue.size() > 0 && dpsQueue.size() >= 3);
        });

        // Form party only after both conditions are met
        tankQueue.pop();
        healerQueue.pop();
        dpsQueue.pop();
        dpsQueue.pop();
        dpsQueue.pop();

        // Assign party to an available dungeon instance
        for (DungeonInstance &instance : instances) {
            if (!instance.isActive()) {
                instance.setActive(true);
                queueLock.unlock(); // Unlock before launching the dungeon

                instanceThreads.emplace_back(&DungeonInstance::start, &instance, t1, t2);

                break;
            }
        }
    }
}

void DungeonManager::notifyInstanceAvailable() {
    std::lock_guard<std::mutex> lock(queueMutex);
    instanceAvailable.notify_all();  // 🔹 Signal a free instance
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

    std::cout << "\n=== Queue Summary ===\n";
    std::cout << "Remaining tanks: " << tankQueue.size() << std::endl;
    std::cout << "Remaining healers: " << healerQueue.size() << std::endl;
    std::cout << "Remaining DPS: " << dpsQueue.size() << std::endl;

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
    maxInstance = getValidInput("Enter max concurrent dungeon instances: ");
    tanks = getValidInput("Enter number of tanks in queue: ");
    healers = getValidInput("Enter number of healers in queue: ");
    dps = getValidInput("Enter number of DPS in queue: ");
    t1 = getValidInput("Enter min dungeon time (t1): ");
    t2 = getValidInput("Enter max dungeon time (t2): ");

    instanceSemaphore.release(maxInstance);

    for (int i = 0; i < maxInstance; i++) {
        instances.emplace_back(i + 1, this);
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