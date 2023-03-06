

#include "CellularAutomata.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include <cmath>

CellularAutomata::CellularAutomata(PDA *cellPDA,std::vector<int>* features) {
    this->cellPDA = cellPDA;

    this->resize(13);
    for(auto &it: *(this)){
        it.resize(11);
    }

    // *****************************************
    // Making the age-distribution vector
    std::string inputFileAges = "../init_age.txt";
    std::ifstream inFile2;
    inFile2.open(inputFileAges);

    // We start by reading the given values, which are supposed to be percentages (ex: 0.010)  starting with the
    // percentage for group 0-10, then 10-20, ...
    double a;
    std::vector<double> agePercentages;
    while (inFile2 >> a) {
        agePercentages.push_back(a);
    }
    inFile2.close();
    // For these percentages, we want to store the int value * 10 (so 0.010 --> 10)
    for (int k = 0; k < agePercentages.size(); ++k) {
        agePercentages[k] = int(agePercentages[k] * 1000);
    }
    // Now, for each agePercentage we fill in the randAgeArray with the amount of ages given by that bracket,
    // calculating the age value by (pseudo-) randomly selecting a value [0-9] to add to the current decimal value
    // (starting with 0*10, then 1*10, ...
    std::vector<int> randAgeArray;
    for (int i = 0; i < agePercentages.size(); ++i) {
        for (int v = 0; v < agePercentages[i]; ++v) {
            int val = i * 10 + rand()%10;
            randAgeArray.push_back(val);
        }
    }
    //******************************

    int x,i,j;
    std::ifstream inFile;
    inFile.open("../init_pop.txt");

    while (inFile >> x >> i >> j) {
        if (x == 0){
            this->begin()[i][j].inhabitedZone = false;
        }else{
            this->begin()[i][j].features = *features;
            this->begin()[i][j].xPos = j;
            this->begin()[i][j].yPos = i;
            for(int k = 0; k < x ; k++){
                Individual* person = new Individual;
                // chance to determine person's essential worker status
                int chance = rand()%10 +1;
                if(chance == 1){
                    person->essential = true;
                }

                this->begin()[i][j].population.push_back(person);
                int it = rand()%randAgeArray.size();
                person->age = randAgeArray[it];
            }
        }
    }


    //insert 5 dead cells in matrix
    this->begin()[4][5].DeadOrAlive =1;
    this->begin()[4][5].infectionCount =1;
    this->begin()[4][5].population[0]->infected =true;
    this->begin()[5][4].DeadOrAlive =1;
    this->begin()[5][4].infectionCount =1;
    this->begin()[5][4].population[0]->infected =true;
    this->begin()[5][6].DeadOrAlive =1;
    this->begin()[5][6].infectionCount =1;
    this->begin()[5][6].population[0]->infected =true;

    inFile.close();

    int r,c,cap;
    std::ifstream inFile3;
    inFile3.open("../init_hospital.txt");

    while (inFile3 >> r >> c >> cap) {
        this->begin()[r][c].hospital.Active = true;
        this->begin()[r][c].hospital.capacity =cap;
        this->begin()[r][c].hospital.column =c;
        Hospitals.push_back(this->begin()[r][c]);
    }

    inFile3.close();

}

void CellularAutomata::show() {
    for (int i = 0; i < this->size(); ++i) {
        for (int j = 0 ; j < this->begin()[i].size(); j++) {
            if(!this->begin()[i][j].inhabitedZone){
                std::cout << "x ";
            }else{
                // Actual infected count
//                std::cout << this->begin()[i][j].infectionCount << " ";
                // percentage
                double infPct = 100.0* this->begin()[i][j].infectionCount/this->begin()[i][j].population.size();
                std::string pctStr = std::to_string(infPct);
                pctStr.resize(4);
                std::cout << pctStr << " ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "------------------- " << std::endl;
}

int CellularAutomata::neighbour(int x, int y){

    if(y >= 0 && y < this->size() && x >= 0 && x < this->begin()[1].size())
        if(this->begin()[y][x].inhabitedZone){
            return this->begin()[y][x].DeadOrAlive;
        }else{
            return 0;
        }
    return 0;
}

int CellularAutomata::neighbourCount(int x, int y){

    return neighbour(x - 1, y - 1) + neighbour(x, y - 1) + neighbour(x + 1, y - 1) +
           neighbour(x - 1, y) + neighbour(x + 1, y) +
           neighbour(x - 1, y + 1) + neighbour(x, y + 1) + neighbour(x + 1, y + 1);
}

CellularAutomata* CellularAutomata::run(){

    CellularAutomata* newCA = new CellularAutomata(this);

    for (int i = 0; i < this->size() ; ++i) {
        for (int j = 0; j < this->begin()[i].size() ; ++j) {
            // Skip cell if not inhabited
            if(!this->begin()[i][j].inhabitedZone){
                continue;
            }
            int state = this->begin()[i][j].DeadOrAlive;
            // Update cell if it's already infected
            if(state == 1){
                newCA->begin()[i][j].cellIteration(&Hospitals);
                continue;
            }
            // If not, count the infected neighbours
            int neighbour_dead = neighbourCount(j, i);
            // Make a string of I's for the PDA, each I representing 1 infected neighbour
            std::string PDAInput = "O";
            for(int k = 0; k < neighbour_dead; k++){
                PDAInput += "I";
            }
            bool infected = this->cellPDA->delta_start(PDAInput);
            if(infected){
                // Start each newly infected cell with 1 infectious person
                newCA->begin()[i][j].DeadOrAlive = 1;
                newCA->begin()[i][j].infectionCount = 1;
                newCA->begin()[i][j].population[0]->infected = true;
            }
        }
    }

    return newCA;
}

CellularAutomata::CellularAutomata(CellularAutomata *originalCA) {
    this->resize(13);
    for(auto &it: *(this)){
        it.resize(11);
    }

    for(int i = 0 ; i < this->size(); i++){
        for(int j = 0; j < this->begin()[i].size(); j++){
            this->begin()[i][j] = originalCA->begin()[i][j];
        }
    }
    this->cellPDA = originalCA->cellPDA;
    this->Hospitals = originalCA->Hospitals;
}


