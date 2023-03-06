

#ifndef CELLS_CELL_H
#define CELLS_CELL_H

#include <vector>
#include "Individual.h"

struct HospitalsCap{
    bool Active = false;
    int capacity =0;
    int column;
};

class Cell {
public:
    int xPos;
    int yPos;
    int DeadOrAlive =0;       //state of cell
    bool inhabitedZone = true;
    std::vector<Individual*> population;
    double spread_factor = 2;
    double infectionCount = 0;
    std::vector<int> features;

    HospitalsCap hospital;

    void cellIteration(std::vector<Cell>* hospitals);

    void updateFeatures(std::vector<Cell>* hospitals);

    void situationOfEssentialWorkers();

    void measures();

    void infrastructure();

    void HospitalCapabilities(std::vector<Cell>* hospitals, int r, int c);

    void AgeDifference();
};


#endif //CELLS_CELL_H
