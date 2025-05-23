#include <iostream>
#include <vector>
#include <chrono>
#include <filesystem>
#include "dataParsing.h"
#include "solver.h"
#include <fstream>
#include <sstream>

void runAllApproaches(const std::string& truckAndPalletsFile, const std::string& palletsFile) {
    Truck truck;
    std::vector<Pallet> pallets;

    parseData(truckAndPalletsFile, palletsFile, truck, pallets);

    std::vector<std::string> approachNames = {
        "Brute Force", "Dynamic Programming", "Greedy", "ILP"
    };

    // Extract dataset number from filename
    std::string baseName = std::filesystem::path(truckAndPalletsFile).filename().string();
    std::string datasetNumber = baseName.substr(baseName.find_last_of("0123456789") - 1, 2); // Gets last 2 digits

    for (int approach = 1; approach <= 4; ++approach) {
            Solver solver(truck.capacity, truck.pallets, pallets, const_cast<std::string&>(truckAndPalletsFile), const_cast<std::string&>(palletsFile));

        auto start = std::chrono::high_resolution_clock::now();

        switch (approach) {
            case 1: solver.bruteForce(); break;
            case 2: solver.dynamicProgramming(); break;
            case 3: solver.greedy(); break;
            case 4: solver.integerLinear(); break;
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;

        // Redirect output to file
        std::ostringstream filename;
        filename << "../output/Solution_" << datasetNumber << "_" << approach << ".txt";
        std::ofstream outFile(filename.str());

        // Save current cout buffer
        std::streambuf* coutBuf = std::cout.rdbuf();
        std::cout.rdbuf(outFile.rdbuf());

        // Output to file
        std::cout << "Dataset: " << truckAndPalletsFile << "\n";
        std::cout << "Approach: " << approachNames[approach - 1] << "\n";
        solver.printMaxValue();
        solver.printSolution();
        std::cout << "Execution time: " << elapsed.count() << " ms\n";

        // Restore cout
        std::cout.rdbuf(coutBuf);

        // Output timing to terminal
        std::cout << "Dataset " << datasetNumber << " | " << approachNames[approach - 1]
                  << " completed in " << elapsed.count() << " ms\n";
    }

    std::cout << "---------------------------------------------\n\n";
}

int main() {
    std::string dataDir = "../data_sets/";
    std::vector<std::string> datasetFiles;

    for (const auto& entry : std::filesystem::directory_iterator(dataDir)) {
        if (entry.is_regular_file()) {
            std::string filename = entry.path().filename().string();
            if (filename.rfind("TruckAndPallets", 0) == 0 && filename.find(".csv") != std::string::npos) {
                datasetFiles.push_back(filename);
            }
        }
    }

    if (datasetFiles.empty()) {
        std::cerr << "No datasets found in " << dataDir << "\n";
        return 1;
    }

    for (const auto& truckFile : datasetFiles) {
        std::string fullTruckPath = dataDir + truckFile;
        std::string palletsFile = dataDir + "Pallets" + truckFile.substr(truckFile.size() - 7);
        runAllApproaches(fullTruckPath, palletsFile);
    }

    return 0;
}
