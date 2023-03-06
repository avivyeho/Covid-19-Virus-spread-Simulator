
#ifndef CELLS_CELLULARAUTOMATA_H
#define CELLS_CELLULARAUTOMATA_H

#include "Cell.h"
#include "PDA.h"

class CellularAutomata: public std::vector<std::vector<Cell>> {

    int neighbour(int x, int y);
    int neighbourCount(int x, int y);
    PDA* cellPDA;
    std::vector<Cell> Hospitals;

public:
    CellularAutomata(PDA* cellPDA,std::vector<int>* features);
    CellularAutomata(CellularAutomata* originalCA);
    CellularAutomata* run();
    void show();
};


#endif //CELLS_CELLULARAUTOMATA_H
