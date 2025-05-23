//
// Created by pedro on 01-05-2025.
//

#include "solver.h"
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <sstream>

// Private Methods (auxiliary methods)

void Solver::sortByRatioDesc() {
    std::sort(pallets.begin(), pallets.end(), [](const Pallet& a, const Pallet& b) {
        double ratioA = a.profit / a.weight;
        double ratioB = b.profit / b.weight;
        if (ratioA == ratioB) return a.weight < b.weight;
        return ratioA > ratioB; // descending order
    });
}

double Solver::greedyRatio(std::vector<Pallet>& solution) {
    sortByRatioDesc();

    int currentWeight = 0;
    double profit = 0;

    for (int i = 0; i < numPallets; i++) {
        if (currentWeight + pallets[i].weight <= capacity) {
            solution.push_back(pallets[i]);
            currentWeight += pallets[i].weight;
            profit += pallets[i].profit;
        }
    }

    return profit;
}

void Solver::sortByValueDesc() {
    std::sort(pallets.begin(), pallets.end(), [](const Pallet& a, const Pallet& b) {
        if (a.profit == b.profit) return a.weight < b.weight;
        return a.profit > b.profit; // descending order
    });
}

double Solver::greedyBiggest(std::vector<Pallet>& solution) {
    sortByValueDesc();

    int currentWeight = 0;
    double profit = 0;

    for (int i = 0; i < numPallets; i++) {
        if (currentWeight + pallets[i].weight <= capacity) {
            solution.push_back(pallets[i]);
            currentWeight += pallets[i].weight;
            profit += pallets[i].profit;
        }
    }

    return profit;
}

std::string Solver::execPythonILP() {
    std::array<char, 128> buffer;
    std::string result;

    // Use full path to python3 to avoid "not found" errors
    std::string cmd = "python3 ../ilp_solver.py \"" + truckFile + "\" \"" + palletsFile + "\" 2>&1";

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    return result;
}

bool Solver::isTimeoutExceeded(const std::chrono::steady_clock::time_point& startTime, double timeoutSeconds) {
    auto now = std::chrono::steady_clock::now();
    double elapsed = std::chrono::duration<double>(now - startTime).count();
    return elapsed > timeoutSeconds;
}


//Public Methods (solution callers)

Solver::Solver(int capacity, int numPallets, std::vector<Pallet>& pallets, std::string &truckFile, std::string &palletsFile) : capacity(capacity), numPallets(numPallets), pallets(pallets), truckFile(truckFile), palletsFile(palletsFile) {}

void Solver::bruteForce() {
    maxProfit = 0;
    solution.clear();

    // There are 2^numPallets subsets
    unsigned long long totalSubsets = 1ULL << numPallets;

    auto startTime = std::chrono::steady_clock::now();

    for (int subset = 0; subset < totalSubsets; ++subset) {
        if (subset % 1000 == 0 && isTimeoutExceeded(startTime, 120)) {
            std::cout << "Timeout reached, stopping brute force early." << std::endl;
            break;
        }

        double currentWeight = 0;
        double currentProfit = 0;
        std::vector<Pallet> currentSolution;

        for (int i = 0; i < numPallets; ++i) {
            if (subset & (1 << i)) {
                currentWeight += pallets[i].weight;
                if (currentWeight > capacity) {
                    break;
                }
                currentProfit += pallets[i].profit;
                currentSolution.push_back(pallets[i]);
            }
        }

        if (currentWeight <= capacity) {
            if (currentProfit > maxProfit) {
                maxProfit = currentProfit;
                solution = currentSolution;
            }
            else if (currentProfit == maxProfit && currentSolution.size() < solution.size()) {
                // Tie on profit, but fewer pallets
                solution = currentSolution;
            }
        }
    }
}

// a vector was used in this approach
void Solver::dynamicProgramming() {
    // dp[i][w] = max profit using first i pallets with max weight w
    std::vector<std::vector<double>> dp(numPallets + 1, std::vector<double>(capacity + 1, 0));

    // Build the DP table bottom up
    for (int i = 1; i <= numPallets; i++) {
        int wt = pallets[i-1].weight;
        double val = pallets[i-1].profit;
        for (int w = 0; w <= capacity; w++) {
            if (wt <= w) {
                dp[i][w] = std::max(dp[i-1][w], dp[i-1][w - wt] + val);
            } else {
                dp[i][w] = dp[i-1][w];
            }
        }
    }

    maxProfit = dp[numPallets][capacity];

    // Reconstruct the solution
    solution.clear();
    int w = capacity;
    for (int i = numPallets; i > 0 && w >= 0; i--) {
        if (dp[i][w] != dp[i-1][w]) {
            solution.push_back(pallets[i-1]);
            w -= pallets[i-1].weight;
        }
    }
}




void Solver::greedy() {
    std::vector<Pallet> greedyRatioPallets;
    std::vector<Pallet> greedyBiggestPallets;

    double ratioSolution = greedyRatio(greedyRatioPallets);
    double biggestSolution = greedyBiggest(greedyBiggestPallets);

    if (ratioSolution > biggestSolution) {
        maxProfit = ratioSolution;
        solution = greedyRatioPallets;
    }
    else if (biggestSolution > ratioSolution) {
        maxProfit = biggestSolution;
        solution = greedyBiggestPallets;
    }
    else {
        // If profit is the same, choose solution with fewer pallets
        if (greedyBiggestPallets.size() > greedyRatioPallets.size()) {
            maxProfit = ratioSolution;
            solution = greedyRatioPallets;
        }
        else {
            maxProfit = biggestSolution;
            solution = greedyBiggestPallets;
        }
    }
}

void Solver::integerLinear() {
    if (truckFile.empty() || palletsFile.empty()) {
        throw std::runtime_error("Input files not set for ILP.");
    }

    std::string output = execPythonILP();

    // Split output into lines
    std::istringstream iss(output);
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(iss, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }

    if (lines.size() < 2) {
        throw std::runtime_error("Unexpected ILP output format (too few lines).");
    }

    // The last line contains the pallet indices
    std::string indicesLine = lines.back();
    lines.pop_back();

    // The second last line contains the max profit
    std::string profitLine = lines.back();

    // Parse maxProfit
    std::istringstream profitStream(profitLine);
    if (!(profitStream >> maxProfit)) {
        throw std::runtime_error("Failed to parse max profit from ILP output");
    }

    // Parse chosen pallet indices
    std::istringstream indicesStream(indicesLine);
    std::vector<int> chosenIndices;
    int idx;
    while (indicesStream >> idx) {
        chosenIndices.push_back(idx);
    }

    // Clear current solution
    solution.clear();

    // Add pallets from chosenIndices to solution
    for (int i : chosenIndices) {
        if (i >= 0 && i < (int)pallets.size()) {
            solution.push_back(pallets[i]);
        } else {
            std::cerr << "Warning: ILP returned invalid pallet index: " << i << std::endl;
        }
    }
}


void Solver::printMaxValue() {
    std::cout << "Maximum profit: " << maxProfit << '\n';
}

void Solver::printSolution() {
    std::sort(solution.begin(), solution.end(), [](const Pallet& a, const Pallet& b) {
        return a.id < b.id;
    });

    std::cout << "Pallets id: ";
    for (int i = 0; i < solution.size(); i++) {
        std::cout << '<' << solution[i].id << ">";
    }
    std::cout << '\n';
}
