//
// Created by janam on 3/25/2025.
//

#ifndef CONSOLEMANAGER_H
#define CONSOLEMANAGER_H
#include <string>

#include "DungeonManager.h"


class ConsoleManager {
public:
    void run();

private:
    DungeonManager dungeonManager;

    void displaySplashScreen();
    void handleCommand(std::string command);

};

#endif //CONSOLEMANAGER_H
