//
// Created by pedro on 01-05-2025.
//

#include "solver.h"
#include <algorithm>

Solver::Solver(double capacity, int numPallets, std::vector<Pallet> pallets) : capacity(capacity), numPallets(numPallets), pallets(pallets) {}

void Solver::bruteForce() {
    
}

void Solver::greedy() {
    std::vector<Pallet> sortedPallets = pallets;
    std::sort(sortedPallets.begin(), sortedPallets.end(), [](const Pallet &a, const Pallet &b) {
        return (a.profit/a.weight) > (b.profit/b.weight);
    });

    double maxProfit = 0;
    double totalWeight = 0;

    for (Pallet &p: sortedPallets) {
        if (totalWeight + p.weight <= capacity) {
            totalWeight += p.weight;
            maxProfit += p.profit;
        }
    }
}

void Solver::dynamicProgramming() {

}

void Solver::integerLinear() {

}

