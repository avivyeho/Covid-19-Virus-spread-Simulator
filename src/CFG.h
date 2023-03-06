
#ifndef PDA_CFG_H
#define PDA_CFG_H


#include <vector>
#include <string>
#include <tuple>

class CFG {
private:
    std::vector<std::string> nonTerminals;  // V
    std::vector<std::string> terminals;     // Σ
    std::vector<std::tuple<std::string,std::string>> productionRules;   // Relation: V --> (V u Σ)*
    std::string startSymbol;    // Must be element of nonTerminals
public:
    CFG(const std::vector<std::string> &nonTerminals, const std::vector<std::string> &terminals,
        const std::vector<std::tuple<std::string, std::string>> &productionRules, const std::string &startSymbol);

    std::string showGrammar();

    const std::vector<std::string> &getNonTerminals() const;

    const std::vector<std::string> &getTerminals() const;

    const std::vector<std::tuple<std::string, std::string>> &getProductionRules() const;

    const std::string &getStartSymbol() const;

    const std::vector<std::tuple<std::string,std::string>> getProductionRulesForNonTerminal(const std::string nonTerminal) const;
};


#endif //PDA_CFG_H
