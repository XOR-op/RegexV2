//
// Created by vortox on 22/10/19.
//

#ifndef REGEXV2_DFA_H
#define REGEXV2_DFA_H

#include <unordered_map>
#include <unordered_set>
#include "NFA.h"
namespace regu{
    struct DFA{
        DFA(const regu::NFA& nfa);
        std::vector<std::unordered_map<char,std::size_t>> mapping;
        std::size_t start;
        std::unordered_set<std::size_t> accepted;
    };


}
#endif //REGEXV2_DFA_H
