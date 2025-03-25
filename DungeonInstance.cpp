//
// Created by janam on 3/24/2025.
//

#include "DungeonInstance.h"

std::mutex DungeonInstance::instanceMutex;

DungeonInstance::DungeonInstance(int instanceID): id(instanceID), active(false), partiesServed(0), duration(0)
{

}

void DungeonInstance::start(int t1, int t2)
{
    {
        std::lock_guard<std::mutex> lock(DungeonInstance::instanceMutex);
        active = true;
        partiesServed++;
        duration = getRandomTime(t1, t2);
        std::cout << "[Dungeon " << id << "] Party entered, clearing in "
                      << duration << " seconds.\n";
    }

    std::this_thread::sleep_for(std::chrono::seconds(duration));

    {
        std::lock_guard<std::mutex> lock(DungeonInstance::instanceMutex);
        active = false;
        std::cout << "[Dungeon " << id << "] Party finished.\n";
    }
}

int DungeonInstance::getRandomTime(int t1, int t2)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(t1, t2);
    duration = dis(gen);
    return duration;
}

bool DungeonInstance::isActive() {
    return active;
}

int DungeonInstance::getPartiesServed() {
    return partiesServed;
}


