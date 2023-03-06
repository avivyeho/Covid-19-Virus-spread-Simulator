
#include "PDA.h"
#include <algorithm>

PDA::PDA() {}

PDA::PDA(std::vector<std::string> alph, State *start) : alphabet(move(alph)), start_state(start) {}

bool PDA::delta(State *q,std::string w) { // RETURN BOOL
     /* CFG to implement:
     * ({S,H},{I},R,{S})
     * where R :
     *      S --> HIII      (with # I's the min. amount of infected neighbours)
     *      H --> HI        (when not at end of input string)
     *      H --> S         (when at end of input string)
    */
    std::string char_string_representation;
    this->stack = {stackInit};
    bool operationFound = true;
    for (char c : w) {
        if(!operationFound){
            continue;
        }
        operationFound = false;

        // First, check the top of the stack. if it's a non-terminal, replace it
        if(find(alphabet.begin(),alphabet.end(),stack.back()) == alphabet.end()){
            // Find the 'transition' on input epsilon (e) with stack_top equal to current stack top
            for(auto trans : q->input_to_state){
                if(trans.first[0] == "e" and trans.first[1] == stack.back()){
                    // Pop the stack top & push the stack operation onto the stack (if not epsilon)
                    stack.pop_back();
                    if(trans.first[2] != "e"){
                        std::string pushToStack = trans.first[2];
                        for(auto push : pushToStack){
                            std::string strVal;
                            strVal.append(1,push);
                            stack.emplace_back(strVal);
                        }
                    }
                }
            }
        }

        // Now, we read the next character
        char_string_representation = c;
        // Find the first transition which reads this character
        for(auto trans : q->input_to_state){
            if(operationFound){
                continue;
            }
            if(trans.first[0] == char_string_representation){
                // If the stack top is equal to the asked stack top
                if(trans.first[1] == stack.back()){
                    operationFound = true;
                    // Pop the stack
                    stack.pop_back();
                    // And push the new stack top (if not epsilon)
                    if(trans.first[2] != "e"){
                        std::string pushToStack = trans.first[2];
                        for(auto push : pushToStack){
                            std::string strVal;
                            strVal.append(1,push);
                            stack.emplace_back(strVal);
                        }
                    }
                }
            }
        }
    }

    // String is accepted if we end in a final state
    if(!operationFound){
        return false;
    }
    if (q->final_state) {
        return true;
    }else{
        // If the stack has size 1, we can have the init. stack symbol (which results in true)
        // OR another non-terminal symbol which transitions to pop itself & push only the init. stack symbol
        if(stack.size() == 1){
            // Or if the stack top is the initial stack symbol
            if(stack.back() == this->stackInit){
                return true;
            }else{
                // Else, we need to look for every transition on input 'e' (epsilon) with the current stack top,
                // IF one of these changes the stack top for the stack init symbol (and the stack is
                std::string stack_top = stack.back();
                // IF the stack top is not part of the alphabet (meaning it's non-terminal
                if(std::find(alphabet.begin(),alphabet.end(),stack_top) == alphabet.end()){
                    for(auto trans : q->input_to_state){
                        // IF the transition is on input epsilon, with the current stack_top as expected
                        // stackTop, AND if the transition pushes stackInit onto the stack, we know the string is accepted
                        if(trans.first[0] == "e" and trans.first[1] == stack_top and trans.first[2] == stackInit){
                            return true;
                        }
                    }
                }else{
                    return false;
                }
            }
        }else{
            return false;
        }
    }
    return false;
}

PDA::PDA(CFG *cfg) {
    // a PDA made from a CFG consists of only 1 state, which is non-final
    this->start_state = new State("q",false);

    // Every terminal symbol is part of the PDA alphabet
    for(auto symbol : cfg->getTerminals()){
        this->alphabet.push_back(symbol);
    }

    // Every terminal AND non-terminal symbol is part of the PDA stack alphabet
    this->stackAlphabet = this->alphabet;
    for(auto symbol : cfg->getNonTerminals()){
        stackAlphabet.push_back(symbol);
    }

    // the starting stack symbol is the same as the starting symbol of the CFG
    this->stackInit = cfg->getStartSymbol();
    this->stack = {cfg->getStartSymbol()};

    // TODO: convert production rules to transitions
    // for each nonterminal A (with production rule A --> a)
    //                       add the rule δ(q,ε,A) --> (q,a)
    std::string epsilon = "e";
    for(auto nonTerminal : cfg->getNonTerminals()){
        // Loop over each production rule for current non-terminal
        for(auto prod : cfg->getProductionRulesForNonTerminal(nonTerminal)){
            // Add transition from only state to itself, with
            // nonTerminal as input, epsilon as stack_top, the result of the prod. rule as stack_operation
            this->start_state->setInputToState(epsilon,nonTerminal,std::get<1>(prod),this->start_state);
        }
    }

    // for each terminal a, add following rule: δ(q,a,a) = (q,ε)
    for(auto terminal : cfg->getTerminals()){
        this->start_state->setInputToState(terminal,terminal,epsilon,this->start_state);
    }
}

State *PDA::getStartState() const {
    return start_state;
}

bool PDA::delta_start(std::string w) {
    return delta(start_state,w);
}
