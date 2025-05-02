#include <vector>
#include "menus.h"
#include "dataParsing.h"
#include "solver.h"

int main() {

    std::string TruckAndPalletsFile = "../data_sets/" + menuDataSetSelection() + ".csv";
    std::string PalletsFile = TruckAndPalletsFile.substr(0,13) + "Pallets" + TruckAndPalletsFile.substr(TruckAndPalletsFile.size()-7);
    int approachSelection = menuApproachSelection();

    Truck truck;
    std::vector<Pallet> pallets;

    parseData(TruckAndPalletsFile, PalletsFile, truck, pallets);

    Solver solver(truck.capacity, truck.pallets, pallets);

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


    return 0;
}
