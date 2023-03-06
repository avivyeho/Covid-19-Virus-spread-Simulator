

#ifndef PDA_STATE_H
#define PDA_STATE_H

#include <vector>
#include <map>

class State {
public:
    std::string state_name;
    std::vector<std::pair<std::vector<std::string>, State*>> input_to_state;
    bool final_state;

    State();

    State(std::string state_name, bool finite=false);

    void setInputToState(std::string& input, std::string& stack_top, std::string& stack_operation, State* resulting_state);

};


#endif //PDA_STATE_H
