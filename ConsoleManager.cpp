//
// Created by janam on 3/25/2025.
//

#include "ConsoleManager.h"

#include <iostream>

void ConsoleManager::run()
{
    std::string command;
    displaySplashScreen();

    dungeonManager.displayInitialization();
    std::thread queueThread(&DungeonManager::processQueue, &dungeonManager);
    queueThread.detach();

    while (true) {
        getline(std::cin, command);
        handleCommand(command);
    }

}

void ConsoleManager::displaySplashScreen()
{
    std::cout << "Welcome to Party Manager!" << std::endl;
}

void ConsoleManager::handleCommand(std::string command) {
    if (command == "exit") {
        std::cout << "Exiting the program..." << std::endl;
        exit(0);
        std::terminate();

    } else if (command == "help") {
        std::cout << "\nCommands:" << std::endl;
        std::cout << "clear/cls - Clear the screen" << std::endl;
        std::cout << "marquee - Switch to marquee screen" << std::endl;
        std::cout << "screen -ls - List all screens" << std::endl;
        std::cout << "screen -s [processName] - Create a new process screen" << std::endl;
        std::cout << "scheduler-test - Start the scheduler test" << std::endl;
        std::cout << "scheduler-stop - Stop the scheduler test" << std::endl;
        std::cout << "report-util - Save the report" << std::endl;
        std::cout << "exit - Exit the program" << std::endl;
    }
    else {
        std::cout << "Invalid command. Please try again." << std::endl;
    }
}

