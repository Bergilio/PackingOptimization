#include <vector>
#include "menus.h"
#include "dataParsing.h"

int main() {

    std::string TruckAndPalletsFile = "../data_sets/" + menuDataSetSelection() + ".csv";
    std::string PalletsFile = TruckAndPalletsFile.substr(0,13) + "Pallets" + TruckAndPalletsFile.substr(TruckAndPalletsFile.size()-7);
    int approachSelection = menuApproachSelection();

    TruckInfo truckInfo;
    std::vector<Pallet> pallets;

    parseData(TruckAndPalletsFile, PalletsFile, truckInfo, pallets);

    return 0;
}
