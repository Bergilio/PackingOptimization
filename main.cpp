#include <iostream>
#include <vector>
#include <chrono>
#include "inputHandler.h"
#include "dataParsing.h"
#include "solver.h"

void printSolution(Truck& truck, std::vector<Pallet>& pallets, int approachSelection) {
    Solver solver(truck.capacity, truck.pallets, pallets);

    auto start = std::chrono::high_resolution_clock::now();

    if (approachSelection == 1) {
        solver.bruteForce();
    }
    else if (approachSelection == 2) {
        solver.dynamicProgramming();
    }
    else if (approachSelection == 3) {
        solver.greedy();
    }
    else if (approachSelection == 4) {
        solver.integerLinear();
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> elapsed = end - start;

    std::cout << "\n-----------------Solution-----------------\n\n";
    solver.printMaxValue();
    solver.printSolution();
    std::cout << "Execution time: " << elapsed.count() << " ms\n";
    std::cout << "\n------------------------------------------\n\n";
}

int main() {
    while (true) {
        std::string truckAndPalletsFile;
        std::string palletsFile;
        int approachSelection;

        Truck truck;
        std::vector<Pallet> pallets;


        if (!getInput(truckAndPalletsFile, palletsFile, approachSelection)) break;
        parseData(truckAndPalletsFile, palletsFile, truck, pallets);
        printSolution(truck, pallets, approachSelection);
    }

    return 0;
}
