//
// Created by pedro on 01-05-2025.
//

#ifndef SOLVER_H
#define SOLVER_H

#include "dataParsing.h"
#include <vector>

class Solver {
private:
    int capacity;
    int numPallets;
    std::vector<Pallet> pallets;

    double maxProfit;
    std::vector<Pallet> solution = {};

    std::string truckFile;
    std::string palletsFile;

    void sortByRatioDesc();
    void sortByValueDesc();
    double greedyRatio(std::vector<Pallet>& solution);
    double greedyBiggest(std::vector<Pallet>& solution);
    std::string execPythonILP();

public:
    Solver(int capacity, int numPallets, std::vector<Pallet>& pallets, std::string &truckFile, std::string &palletsFile);
    void bruteForce();
    void dynamicProgramming();
    void greedy();
    void integerLinear();
    void printMaxValue();
    void printSolution();
};


#endif //SOLVER_H