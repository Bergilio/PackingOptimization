#ifndef DATAPARSING_H
#define DATAPARSING_H

#include <string>
#include <vector>

struct TruckInfo {
    double capacity;
    int pallets;
};

struct Pallet {
    int id;
    double weight;
    double profit;
};

void parseTruckInfo(const std::string& TruckAndPalletsFile, TruckInfo& truckInfo);
void parsePallets(const std::string& PalletsFile, std::vector<Pallet>& pallets);
void parseData(const std::string& TruckAndPalletsFile, const std::string& PalletsFile, TruckInfo& truckInfo, std::vector<Pallet>& pallets);

#endif
