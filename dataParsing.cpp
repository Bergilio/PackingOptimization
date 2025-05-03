#include "dataParsing.h"
#include <fstream>
#include <iostream>
#include <sstream>

void parseTruckInfo(const std::string& TruckAndPalletsFile, Truck& truck) {
    std::ifstream inputFile(TruckAndPalletsFile);

    if (!inputFile.is_open()) {
        std::cerr << "Could not open file " << TruckAndPalletsFile << std::endl;
        return;
    }

    std::string line;
    std::getline(inputFile, line);
    std::getline(inputFile, line);

    truck.capacity = std::stod(line.substr(0, line.find(',')));
    truck.pallets = std::stoi(line.substr(line.find(',') + 1));

}

void parsePallets(const std::string& PalletsFile, std::vector<Pallet>& pallets) {
    std::ifstream inputFile(PalletsFile);

    if (!inputFile.is_open()) {
        std::cerr << "Could not open file " << PalletsFile << std::endl;
        return;
    }

    std::string line;
    std::getline(inputFile, line);
    while (std::getline(inputFile, line)) {
        Pallet pallet;
        std::istringstream iss(line);
        pallet.id = std::stoi(line.substr(0, line.find(',')));
        pallet.weight = std::stod(line.substr(line.find(',') + 1));
        pallet.profit = std::stod(line.substr(line.find_last_of(',') + 1));
        pallets.push_back(pallet);
    }
}



void parseData(const std::string& TruckAndPalletsFile, const std::string& PalletsFile, Truck& truck, std::vector<Pallet>& pallets) {
    parseTruckInfo(TruckAndPalletsFile, truck);
    parsePallets(PalletsFile, pallets);
}