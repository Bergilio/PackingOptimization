//
// Created by pedro on 01-05-2025.
//

#ifndef SOLVER_H
#define SOLVER_H

#include "dataParsing.h"
#include <vector>

class Solver {
private:
    double capacity;
    int numPallets;
    std::vector<Pallet> pallets;

    double maxProfit = 0.0;
    std::vector<Pallet> solution = {};
public:
    Solver(double capacity, int numPallets, std::vector<Pallet>& pallets);
    void bruteForce();
    void dynamicProgramming();
    void greedy();
    void integerLinear();
    void printMaxValue();
    void printSolution();
};


#endif //SOLVER_H
