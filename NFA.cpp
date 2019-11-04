//
// Created by vortox on 13/10/19.
//

#include <sstream>
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include "NFA.h"

using namespace regu;
using std::size_t;

struct two_nodes{
    size_t start;
    size_t end;
};
size_t addStatus(std::vector<NFAstatus>& arr){
    size_t t=arr.size();
    arr.emplace_back(NFAstatus());
    return t;
}
void make_null_transition(std::vector<NFAstatus>& arr,size_t from,size_t to){
    arr[from].addTransition(NULL_TRANS,to);
}
two_nodes subToNFA(std::istringstream& istm,std::vector<NFAstatus>& arr) {
    char c;
    istm >> c;
    switch (c) {
        case '|': {
            auto start_s = addStatus(arr);
            auto end_s = addStatus(arr);
            auto a = subToNFA(istm, arr);
            auto b = subToNFA(istm, arr);
            make_null_transition(arr,start_s, a.start);
            make_null_transition(arr,start_s, b.start);
            make_null_transition(arr,a.end, end_s);
            make_null_transition(arr,b.end, end_s);
            return {start_s, end_s};
        }
        case '+': {
            auto a = subToNFA(istm, arr);
            auto b = subToNFA(istm, arr);
            make_null_transition(arr,b.end, a.start);
            return {b.start, a.end};
        }
        case '*': {
            auto start_s = addStatus(arr);
            auto end_s = addStatus(arr);
            auto sub = subToNFA(istm, arr);
            make_null_transition(arr,sub.end, sub.start);
            make_null_transition(arr,sub.end, end_s);
            make_null_transition(arr,start_s, sub.start);
            make_null_transition(arr,start_s, end_s);
            return {start_s, end_s};
        }
        case '\\': {
            istm >> c;
            // fall through
        }
        default:
            auto start_s = addStatus(arr);
            auto end_s = addStatus(arr);
            arr[start_s].addTransition(c,end_s);
            return {start_s, end_s};
    }
}

regu::NFA::NFA(std::string suffix_expression) {
    std::reverse(suffix_expression.begin(), suffix_expression.end());
    std::istringstream istm(suffix_expression);
    two_nodes t=subToNFA(istm,this->stats);
    this->start=t.start;
    this->end=t.end;
}



