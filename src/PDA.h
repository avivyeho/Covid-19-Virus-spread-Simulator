

#ifndef PDA_PDA_H
#define PDA_PDA_H

#include <iostream>
#include <vector>
#include <string>
#include "State.h"
#include "CFG.h"


class PDA {
    State* start_state;
    std::vector<std::string> alphabet;
    std::vector<State*> possible_states;
    std::vector<std::string> stack;

    std::vector<std::string> stackAlphabet;
    std::string stackInit;
public:
    PDA();

    PDA(CFG* cfg);

    PDA(std::vector<std::string> alph, State* start);

    bool delta(State* q,std::string w);

    bool delta_start(std::string w);

    State *getStartState() const;
};


#endif //PDA_PDA_H
