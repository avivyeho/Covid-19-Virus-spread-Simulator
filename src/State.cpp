

#include "State.h"

State::State() {}

State::State(std::string state_name, bool finite) : state_name(move(state_name)), final_state(finite) {}

void State::setInputToState(std::string& input, std::string& stack_top, std::string& stack_operation, State *resulting_state) {
    std::vector<std::string>* vec = new std::vector<std::string>;
    std::pair<std::vector<std::string>, State*>* p = new std::pair<std::vector<std::string>, State*>;

    vec->push_back(input);
    vec->push_back(stack_top);
    vec->push_back(stack_operation);

    p->first = *vec;
    p->second = resulting_state;

    input_to_state.push_back(*p);
}
