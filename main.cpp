#include <iostream>

#include "DungeonManager.h"

int main() {
    int n, t, h, d, t1, t2;

    std::cout << "Enter max concurrent dungeon instances: ";
    std::cin >> n;
    std::cout << "Enter number of tanks in queue: ";
    std::cin >> t;
    std::cout << "Enter number of healers in queue: ";
    std::cin >> h;
    std::cout << "Enter number of DPS in queue: ";
    std::cin >> d;
    std::cout << "Enter min dungeon time (t1): ";
    std::cin >> t1;
    std::cout << "Enter max dungeon time (t2): ";
    std::cin >> t2;

    DungeonManager manager(n, t, h, d, t1, t2);

    std::thread queueThread(&DungeonManager::processQueue, &manager);
    queueThread.join();

    manager.displaySummary();

    return 0;
}