//
// Created by pedro on 01-05-2025.
//

#include "solver.h"

#include <iostream>

Solver::Solver(double capacity, int numPallets, std::vector<Pallet>& pallets) : capacity(capacity), numPallets(numPallets), pallets(pallets) {}

void Solver::bruteForce() {
    solution = pallets;
}

void Solver::dynamicProgramming() {}

void Solver::greedy() {}

void Solver::integerLinear() {}

void Solver::printMaxValue() {
    std::cout << "Maximum profit: " << maxProfit << '\n';
}

void Solver::printSolution() {
    std::cout << "Pallets id: ";
    for (int i = 0; i < solution.size(); i++) {
        std::cout << '<'<< solution[i].id << ">";
    }
    std::cout << '\n';
}
