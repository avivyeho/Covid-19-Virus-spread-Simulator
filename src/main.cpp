#include <iostream>
#include <vector>
#include "CellularAutomata.h"
#include "PDA.h"
#include "State.h"
#include "CFG.h"
#include <chrono>

using namespace std;


int main(){
    std::vector<std::tuple<std::string,std::string>> prodRules;
    prodRules.emplace_back("S","HIIIO");
    prodRules.emplace_back("H","HI");
    prodRules.emplace_back("H","S");
    CFG* test = new CFG({"S","H"},{"I","O"},prodRules,"S");
    PDA* pdaFromCfg = new PDA(test);

    // Features:
    // Logically must init at 0 : Essential, populationAge, Hospital
    // {measures,Infrastructure,Hospital,Essential,VirusProp,populationAge}
    std::vector<int>* features = new std::vector<int>{5,6,0,0,1,0};
    CellularAutomata* c = new CellularAutomata(pdaFromCfg,features);

    int step =0;

    while (step < 20) {

        c->show();
        c = c->run();

        step++;
    }

    return 0;
}