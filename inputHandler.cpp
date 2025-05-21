#include "inputHandler.h"
#include <iostream>
#include <filesystem>
#include <vector>
#include <limits>
#include <algorithm> // for std::sort

bool getInput(std::string &truckAndPalletsFile, std::string &palletsFile, int &approachSelection) {
    truckAndPalletsFile = "../data_sets/" + menuDataSetSelection() + ".csv";
    if (truckAndPalletsFile == "../data_sets/.csv") return false;

    palletsFile = truckAndPalletsFile.substr(0, 13) + "Pallets" + truckAndPalletsFile.substr(truckAndPalletsFile.size() - 7);

    approachSelection = menuApproachSelection();
    return true;
}

std::string menuDataSetSelection() {
    std::cout << "Delivery Truck Pallet Packing Optimization\n\n";
    std::cout << "Choose the Data Set from the list:\n";

    std::vector<std::string> files;

    for (const auto& entry : std::filesystem::directory_iterator("../data_sets/")) {
        if (entry.is_regular_file()) {
            std::string filename = entry.path().filename().string();

            if (filename.rfind("TruckAndPallets", 0) == 0) {
                files.push_back(filename.substr(0, filename.size() - 4));
            }
        }
    }

    if (files.empty()) {
        std::cout << "No data sets found in ../data_sets/ directory.\n";
        return "";
    }

    std::sort(files.begin(), files.end());

    int choice = -1;
    while (true) {
        int i = 1;
        for (const auto& name : files) {
            std::cout << "[" << i << "] " << name << '\n';
            i++;
        }
        std::cout << "\nPress 0 to quit\n";

        std::cout << "\nChoice: ";

        if (!(std::cin >> choice)) {
            // input was not an integer
            std::cout << "Invalid input. Please enter a number.\n\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (choice == 0) {
            return "";
        }

        if (choice < 1 || choice > static_cast<int>(files.size())) {
            std::cout << "Invalid choice. Please select a valid number from the list.\n\n";
            continue;
        }

        // valid choice
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear rest of line
        return files[choice - 1];
    }
}

int menuApproachSelection() {
    int choice = -1;

    while (true) {
        std::cout << "Choose the algorithmic approach:\n";
        std::cout << "[1] Exhaustive (Brute-Force) Approach\n";
        std::cout << "[2] Dynamic Programming Approach\n";
        std::cout << "[3] Approximation Algorithms (Greedy Approach)\n";
        std::cout << "[4] Integer Linear Programming Algorithm (ILP)\n\n";
        std::cout << "Choice: ";

        if (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter a number.\n\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (choice < 1 || choice > 4) {
            std::cout << "Invalid choice. Please enter a number between 1 and 4.\n\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear rest of line
        return choice;
    }
}
