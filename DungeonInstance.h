//
// Created by janam on 3/24/2025.
//

#ifndef DUNGEONINSTANCE_H
#define DUNGEONINSTANCE_H
#include <mutex>
#include <iostream>
#include <thread>
#include <random>

#include "DungeonInstance.h"


class DungeonInstance {
private:
    int id;
    int duration;
    bool active;
    int partiesServed;
    static std::mutex instanceMutex;

public:
    DungeonInstance(int instanceID);
    void start(int t1, int t2);
    int getRandomTime(int t1, int t2);
    bool isActive();
    int getPartiesServed();
    int getInstanceID();
    void setActive(bool flag);
};

#endif //DUNGEONINSTANCE_H
