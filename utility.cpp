//
// Created by vortox on 22/10/19.
//
#include <unordered_set>
#include <iostream>
#include <queue>
#include <sstream>
#include <fstream>
#include "NFA.h"
#include "utility.h"
using std::endl;
void regu::DotToFile(const std::string& s, const std::string& name){
    std::string folder="/home/vortox/files/experiment/";
    std::ofstream ofstm(folder+name+".dot");
    ofstm<<s;
    ofstm.close();
    std::ostringstream cmd;
    cmd<<"dot -Tpng "<<folder<<name<<".dot -o "<<folder<<name+".png";
    system(cmd.str().c_str());
}
std::string regu::NFAtoDot(const NFA& nfa){
    std::unordered_set<size_t> all;
    std::queue<size_t> queue;
    std::ostringstream stm;
    std::ostringstream estm;
    stm << "digraph G {" << endl
        << "\trankdir=LR;"
        << "\tnode [shape=circle];" << endl;
    queue.push(nfa.start);
    all.insert(nfa.start);
    while (!queue.empty()) {
        auto sp = queue.front();
        queue.pop();
        if(sp==nfa.start){
            stm<<"\t"<<sp<<" [shape=Mrecord,label=\"s"<<sp<<"\"];"<<endl;
        } else if(sp==nfa.end){
            stm<<"\t"<<sp<<" [shape=doublecircle, label=\"s"<<sp<<"\"];"<<endl;
        } else{
            stm<<"\t"<<sp<<" [label=\"s"<<sp<<"\"];"<<endl;
        }
        for (int i=0;i<2;++i){
            if(nfa.stats[sp].transitions[i]==NO_TRANS)break;
            size_t toward=nfa.stats[sp].towards[i];
            if(all.find(toward) == all.end()){
                queue.push(toward);
                all.insert(toward);
            }
            estm << "\t"<<sp << " -> " << toward <<" [ label=\""<<(nfa.stats[sp].transitions[i]==NULL_TRANS?"&#949;\" style=dotted":(std::string(1,nfa.stats[sp].transitions[i])+"\""))<<"];"<<endl;
        }
    }
    stm<<endl<<estm.str()<<"}";
    return stm.str();
}

std::string regu::DFAtoDot(const regu::DFA& dfa) {

    std::unordered_set<size_t> all;
    std::queue<size_t> queue;
    std::ostringstream stm;
    std::ostringstream estm;
    stm << "digraph G {" << endl
        << "\trankdir=LR;"
        << "\tnode [shape=circle];" << endl;
    queue.push(dfa.start);
    all.insert(dfa.start);
    while (!queue.empty()) {
        auto sp = queue.front();
        queue.pop();
        if(sp==dfa.start){
            if(dfa.accepted.find(sp)!=dfa.accepted.end()){
                stm<<"\t"<<sp<<" [shape=doubleoctagon,label=\"s"<<sp<<"\"];"<<endl;
            } else
            stm<<"\t"<<sp<<" [shape=octagon,label=\"s"<<sp<<"\"];"<<endl;
        } else if(dfa.accepted.find(sp)!=dfa.accepted.end()){
            stm<<"\t"<<sp<<" [shape=doublecircle, label=\"s"<<sp<<"\"];"<<endl;
        } else{
            stm<<"\t"<<sp<<" [label=\"s"<<sp<<"\"];"<<endl;
        }
        for (auto e:dfa.mapping[sp]){
            if(all.find(e.second) == all.end()){
                queue.push(e.second);
                all.insert(e.second);
            }
            estm << "\t"<<sp << " -> " << e.second <<" [ label=\""<<e.first<<"\"];"<<endl;
        }
    }
    stm<<endl<<estm.str()<<"}";
    return stm.str();
}



