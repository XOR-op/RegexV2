//
// Created by vortox on 12/10/19.
//

#include <stack>
#include <unordered_map>
#include<algorithm>
#include "RPN.h"

enum {
    NOTPOP,CHOICE, LINK, CLOSURE, BRACKET, ESCAPE
};
const char LINK_SIGN='+';

std::string regu::RPN(const std::string& s) {
    static std::unordered_map<char, int> ranks({
                                                       {'|', CHOICE},
                                                       {LINK_SIGN, LINK},
                                                       {'*', CLOSURE},
                                                       {'(',NOTPOP},
                                                       {')',BRACKET},
                                                       {'\\', ESCAPE}
                                               });
    std::string suff_s;
    std::stack<char> stk;
    std::stack<char> op;

    for (auto iter = s.begin(), stopI = s.end(); iter < stopI; ++iter) {
        switch (*iter){
            case '\\':{
                ++iter;
                stk.push(*iter);
                stk.push('\\');
                if(*iter=='|')stk.push(LINK_SIGN); // work as out-of-switch statement
                break;
            }
            case '(':{
                op.push('(');
                continue;
            }
            case ')':{
                op.pop();// pop LINK_SIGN
                while (op.top() != '(') {
                    stk.push(op.top());
                    op.pop();
                }
                op.pop();
                break;
            }
            case '*':{
                op.pop();
                stk.push('*');
                break;
            }
            default:{
                auto pair_p = ranks.find(*iter);
                if (pair_p == ranks.end()||*iter==LINK_SIGN) {
                    // not operator
                    stk.push(*iter);
                    if(*iter==LINK_SIGN)stk.push('\\');

                } else {
                    // c in ranks
                    if(!op.empty()&&ranks[op.top()]>pair_p->second){
                        // pop operators
                        if(*iter=='|'&&op.top()==LINK_SIGN){
                            op.pop();
                        }
                        while (!op.empty()&&ranks[op.top()]>pair_p->second){
                            stk.push(op.top());
                            op.pop();
                        }
                        op.push(*iter);
                    } else{
                        op.push(*iter);
                    }
                }
            }
        }
        if(*iter!='|')op.push(LINK_SIGN);

    }
    if(!op.empty())   // process "" string
        op.pop();   // pop LINK_SIGN
    while (!op.empty()){
        stk.push(op.top());
        op.pop();
    }
    while (!stk.empty()){
        suff_s.push_back(stk.top());
        stk.pop();
    }
    std::reverse(suff_s.begin(),suff_s.end());
    return suff_s;
}

std::string regu::ProcessConcat(std::string s) {
    std::string sub;
    size_t offset=0;
    for(offset = s.find('[', offset); offset != std::string::npos; offset=s.find('[',offset)) {
        std::stack<char> stk;
        auto record=offset++;
        for(;s[offset]!=']';++offset){
            if(s[offset]!='-')stk.push(s[offset]);
            else{
                ++offset; // skip '-'
                for(char c= static_cast<char>(stk.top()+1);c<=s[offset];++c)stk.push(c);
            }
        }
        sub.push_back('(');
        while (!stk.empty()){
            sub.push_back(stk.top());stk.pop();
            sub.push_back('|');
        }
        sub.pop_back();sub.push_back(')');
        s.erase(record,offset+1);
        s.insert(record,sub);
        offset=record+sub.size();
    }
    return s;
}

