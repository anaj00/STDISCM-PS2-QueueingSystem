//
// Created by janam on 3/24/2025.
//

#ifndef DUNGEONMANAGER_H
#define DUNGEONMANAGER_H
#include <condition_variable>
#include <queue>
#include <thread>
#include <iostream>


#include "DungeonInstance.h"


class DungeonManager {
private:
    int maxInstance = 0;
    int tanks = 0, healers = 0, dps = 0;
    int t1 = 0, t2 = 0;

    std::queue<int> tankQueue, healerQueue, dpsQueue;

    std::vector<DungeonInstance> instances;
    std::vector<std::thread> instanceThreads;

    std::mutex queueMutex;
    std::condition_variable partyAvailable;
    std::counting_semaphore<> instanceSemaphore{0};

public:
    DungeonManager();
    void queuePlayers();
    void processQueue();
    void displayStatus();
    void displaySummary();
    void displayInitialization();
    void displaySettings();
    ~DungeonManager();
};



#endif //DUNGEONMANAGER_H
