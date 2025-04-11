#include "menus.h"
#include <iostream>
#include <filesystem>
#include <vector>

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

    std::sort(files.begin(), files.end());

    int i = 1;
    for (const auto& name : files) {
        std::cout << "[" << i << "] " << name << '\n';
        i++;
    }

    int choice;
    std::cout << "\nChoice: ";
    std::cin >> choice;
    std::cout << '\n';

    return files[choice - 1];
}

int menuApproachSelection() {
    int choice;

    std::cout << "Choose the algorithmic approach:\n";
    std::cout << "[1] Exhaustive (Brute-Force) Approach\n";
    std::cout << "[2] Dynamic Programming Approach\n";
    std::cout << "[3] Approximation Algorithms (Greedy Approach)\n";
    std::cout << "[4] Integer Linear Programming Algorithm (ILP)\n\n";

    std::cout << "Choice: ";
    std::cin >> choice;
    std::cout << '\n';

    return choice;
}
