
#include "CFG.h"

CFG::CFG(const std::vector<std::string> &nonTerminals, const std::vector<std::string> &terminals,
         const std::vector<std::tuple<std::string, std::string>> &productionRules, const std::string &startSymbol)
        : nonTerminals(nonTerminals), terminals(terminals), productionRules(productionRules),
          startSymbol(startSymbol) {}

std::string CFG::showGrammar() {
    std::string output = "CFG \n non-terminal characters: \n";

    for(auto v : nonTerminals){
        output += "\t" + v + "\n";
    }

    output += "\n terminal characters: \n";
    for(auto s : terminals){
        output += "\t" + s + "\n";
    }

    output += "\n production rules: \n";
    for(auto prod : productionRules){
        output += "\t" + std::get<0>(prod) + "-->" + std::get<1>(prod) + "\n";
    }

    output += "\n start symbol: \n \t" + startSymbol;

    return output;
}

const std::vector<std::string> &CFG::getNonTerminals() const {
    return nonTerminals;
}

const std::vector<std::string> &CFG::getTerminals() const {
    return terminals;
}

const std::vector<std::tuple<std::string, std::string>> &CFG::getProductionRules() const {
    return productionRules;
}

const std::string &CFG::getStartSymbol() const {
    return startSymbol;
}

const std::vector<std::tuple<std::string, std::string>> CFG::getProductionRulesForNonTerminal(const std::string nonTerminal) const {
    std::vector<std::tuple<std::string, std::string>> result;

    for(auto prod : this->getProductionRules()){
        if(std::get<0>(prod) == nonTerminal){
            result.push_back(prod);
        }
    }

    return result;
}
