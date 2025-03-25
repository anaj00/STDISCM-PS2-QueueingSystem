//
// Created by janam on 3/24/2025.
//

#include "DungeonManager.h"



DungeonManager::DungeonManager() : instanceSemaphore(maxInstance) {
    for (int i = 0; i < maxInstance; i++)
    {
        instances.emplace_back(i+1);
    }
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

        // Form party
        tankQueue.pop();
        healerQueue.pop();
        dpsQueue.pop();
        dpsQueue.pop();
        dpsQueue.pop();

        lock.unlock();

        instanceSemaphore.acquire(); // Wait for an available instance

        for (DungeonInstance &instance : instances) {
            if (!instance.isActive()) {
                instanceThreads.emplace_back(&DungeonInstance::start, &instance, t1, t2);
                break;
            }
        }
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
}

DungeonManager::~DungeonManager() {
    for (auto &t : instanceThreads) {
        if (t.joinable()) {
            t.join();
        }
    }
}


