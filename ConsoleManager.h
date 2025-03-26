//
// Created by janam on 3/25/2025.
//

#ifndef CONSOLEMANAGER_H
#define CONSOLEMANAGER_H
#include <string>

#include "Utility.h"
#include "DungeonManager.h"


class ConsoleManager {
private:
    DungeonManager dungeonManager;

    void displaySplashScreen();
    void handleCommand(std::string command);

public:
    void run();

};

#endif //CONSOLEMANAGER_H
