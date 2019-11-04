//
// Created by vortox on 22/10/19.
//

#ifndef REGEXV2_UTILITY_H
#define REGEXV2_UTILITY_H

#include <string>
#include "NFA.h"
#include "DFA.h"
namespace regu {
    void DotToFile(const std::string& s, const std::string& name="testfile");
    std::string NFAtoDot(const NFA& nfa);
    std::string DFAtoDot(const DFA& dfa);
}
#endif //REGEXV2_UTILITY_H
