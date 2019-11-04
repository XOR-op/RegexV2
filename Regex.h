//
// Created by vortox on 25/10/19.
//

#ifndef REGEXV2_REGEX_H
#define REGEXV2_REGEX_H

#include "RPN.h"
#include "DFA.h"
namespace regu{
    class Regex{
    public:
        explicit Regex(const std::string& s):dfa(NFA(RPN(s))){}
        bool match(const std::string& expression){
            size_t cur=dfa.start;
            for(char c:expression){
                auto it=dfa.mapping[cur].find(c);
                if(it==dfa.mapping[cur].end())return false;
                cur=it->second;
            }
            return dfa.accepted.find(cur)!=dfa.accepted.end();
        }
    private:
        DFA dfa;
    };
}
#endif //REGEXV2_REGEX_H
