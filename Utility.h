//
// Created by janam on 3/26/2025.
//

#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <limits>

// Utility function to display a divider
inline void displayDivider()
{
    std::cout << "\n==============================================\n";
}

inline int getValidInput(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        std::string input;
        std::cin >> input;

        // Check if input contains only digits (no negative or decimal)
        if (input.find_first_not_of("0123456789") == std::string::npos) {
            value = std::stoi(input);
            if (value >= 0) return value;  // Ensure non-negative
        }

        std::cout << "Invalid input. Please enter a positive integer.\n";
        std::cin.clear();  // Clear error state
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Discard invalid input
    }
}


#endif // UTILITY_H

