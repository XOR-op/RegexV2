//
// Created by vortox on 12/10/19.
//

#ifndef REGEX_STRUCTURE_H
#define REGEX_STRUCTURE_H

#include <vector>
#include <string>
using std::size_t;

namespace regu{
    /*
     *  NFAstatus: at most 2 status in and 2 out
     */
    const char NO_TRANS=0;
    const char NULL_TRANS=1;
    struct NFAstatus{
//        size_t id;
        size_t towards[2];
        char transitions[2];
        void addTransition(char trans_c,size_t to){
            size_t i=transitions[0]!=NO_TRANS;
            towards[i]=to;
            transitions[i]=trans_c;
        }
        NFAstatus()= default;
        bool hasEdge(size_t i)const { return i<2&&transitions[i]!=NO_TRANS;}
    };

    struct NFA{
        NFA(std::string suffix_expression);
        std::vector<NFAstatus> stats;
        size_t start;
        size_t end;
    };
}
#endif //REGEX_STRUCTURE_H
