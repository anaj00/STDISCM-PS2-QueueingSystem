//
// Created by janam on 3/24/2025.
//

#ifndef DUNGEONMANAGER_H
#define DUNGEONMANAGER_H
#include <condition_variable>
#include <queue>
#include <thread>

#include "DungeonInstance.h"


class DungeonManager {
private:
    int maxInstance;
    int tanks, healers, dps;
    int t1, t2;

    std::queue<int> tankQueue, healerQueue, dpsQueue;
    std::vector<DungeonInstance> instances;
    std::vector<std::thread> instanceThreads;

    std::mutex queueMutex;
    std::condition_variable partyAvailable;
    std::counting_semaphore<> instanceSemaphore;

public:
    DungeonManager(int n, int n_tanks, int n_healers, int n_dps, int minTime, int maxTime);
    void queuePlayers();
    void processQueue();
    void displaySummary();
    ~DungeonManager();
};



#endif //DUNGEONMANAGER_H
