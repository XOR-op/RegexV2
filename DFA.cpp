//
// Created by vortox on 22/10/19.
//
#include <boost/functional/hash.hpp>
#include <set>
#include <unordered_map>
#include <queue>
#include <stack>
#include "DFA.h"
#include "NFA.h"

using std::size_t;
using std::vector;
using std::unordered_set;
using std::unordered_map;
using std::make_shared;
using std::shared_ptr;
using regu::NFA;
typedef std::set<size_t> DFAStatus;
typedef unordered_map<DFAStatus,unordered_map<char,DFAStatus>,boost::hash<DFAStatus>> DFAStatusMap;

vector<DFAStatus> e_closure(const NFA& nfa) {
    vector<DFAStatus> closure(nfa.stats.size());
    for (size_t i = 0; i < nfa.stats.size(); ++i) {
        std::queue<size_t> nfaQueue;
        nfaQueue.push(i);
        while (!nfaQueue.empty()) {
            size_t cur = nfaQueue.front();
            nfaQueue.pop();
            closure[i].insert(cur);
            for (auto j = 0; nfa.stats[cur].hasEdge(j); ++j) {
                if (nfa.stats[cur].transitions[j]==regu::NULL_TRANS&&closure[i].find(nfa.stats[cur].towards[j])==closure[i].end()) // every epsilon trans
                    nfaQueue.push(nfa.stats[cur].towards[j]);
            }
        }
    }
    return closure;
}


regu::DFA::DFA(const NFA& nfa) {
    auto eClosure=e_closure(nfa);
    unordered_map<DFAStatus,size_t,boost::hash<DFAStatus>> DFAtoID;
    size_t cnt=0;
    DFAStatusMap DFATransition;
    std::queue<DFAStatus> dfaQueue;
    dfaQueue.push(eClosure[nfa.start]);
    DFAtoID[eClosure[nfa.start]]=cnt++;
    while (!dfaQueue.empty()){
        auto t=dfaQueue.front();dfaQueue.pop();
        // search every forward edge
        std::priority_queue<std::pair<char,size_t>> que;
        for(size_t perNFAStatus:t){
            const auto& ref=nfa.stats[perNFAStatus];
            for(size_t i=0;ref.hasEdge(i);++i){
                if(ref.transitions[i]!=regu::NULL_TRANS) {
                    que.push({ref.transitions[i], ref.towards[i]});
                }
            }
        }
        // mapping
        while (!que.empty()){
            auto cur=que.top();que.pop();

            if(!que.empty()&&cur.first==que.top().first){
                // merge
                DFAStatus tmp=eClosure[cur.second];
                while (!que.empty()&&cur.first==que.top().first) {
                    tmp.merge(eClosure[que.top().second]);
                    que.pop();
                }
                if(DFATransition.find(tmp)==DFATransition.end()){
                    dfaQueue.push(tmp);
                    DFATransition[tmp]=unordered_map<char ,DFAStatus>();
                    DFAtoID[tmp]=cnt++;
                }
                DFATransition[t][cur.first]=tmp;
            } else{
                if(DFAtoID.find(eClosure[cur.second])==DFAtoID.end()){
                    DFAtoID[eClosure[cur.second]]=cnt++;
                    dfaQueue.push(eClosure[cur.second]);
                }
                DFATransition[t][cur.first]=eClosure[cur.second];
            }


        }

    }
    // DFATransition is the final DFA
    // convert DFATransition to numerical
    vector<bool > bfsRecord(cnt);
    this->start=0;
    this->mapping.resize(cnt);
    dfaQueue.push(eClosure[nfa.start]);
    while (!dfaQueue.empty()){
        size_t top_query_result=DFAtoID[dfaQueue.front()];
        if(dfaQueue.front().find(nfa.end)!=dfaQueue.front().end()){
            this->accepted.insert(top_query_result);
        }
        for(const auto& e:DFATransition[dfaQueue.front()]){
            size_t query_result=DFAtoID[e.second];
            if(!bfsRecord[query_result]){
                dfaQueue.push(e.second);
                bfsRecord[query_result]= true;
            }
            this->mapping[top_query_result][e.first]=query_result;
        }
        dfaQueue.pop();

    }

    int i=0;
}
