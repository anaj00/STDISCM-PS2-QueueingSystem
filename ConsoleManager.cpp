//
// Created by janam on 3/25/2025.
//

#include "ConsoleManager.h"
#include "Utility.h"

#include <iostream>

void ConsoleManager::run()
{
    std::string command;
    displaySplashScreen();

    dungeonManager.displayInitialization();
    dungeonManager.queuePlayers();
    std::thread queueThread(&DungeonManager::processQueue, &dungeonManager);
    queueThread.detach();

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // flush
    while (true) {
        std::cout << std::endl;
        std::cout << "> ";
        getline(std::cin, command);
        handleCommand(command);
    }

}

void ConsoleManager::displaySplashScreen()
{
    std::cout << "=== Welcome to Dungeon Party Manager! ===" << std::endl;
    displayDivider();
}

void ConsoleManager::handleCommand(std::string command) {

    if (command == "exit") {
        dungeonManager.displaySummary();
        std::cout << "Exiting the program..." << std::endl;
        exit(0);
        std::terminate();

    } else if (command == "help") {
        std::cout << "\n=== Commands ===\n";
        std::cout << "status - Check status of all parties" << std::endl;
        std::cout << "exit - Exit the program" << std::endl;
    }

    else if (command == "status")
    {
        std::cout << "\n=== Status ===\n";
        dungeonManager.displayStatus();
    }

    else {
        std::cout << "Invalid command. Please try again." << std::endl;
    }
}

