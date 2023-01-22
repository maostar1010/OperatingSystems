// -------------------------------------------------------------------------------------
// Editted and submitted by: Mary Mo (ga.mo@ucalgary.ca)
// -------------------------------------------------------------------------------------
//
// (c) 2021, Pavol Federl, pfederl@ucalgary.ca
// Do not distribute this file.

// Q1 optimization using std::vector<int>

#include "deadlock_detector.h"
#include "common.h"
#include <iostream>

using VS = std::vector<std::string>;

struct Graph {
    std::vector<std::vector<int>> adj_list;
    std::vector<int> out_counts;
};

VS str; // store the process names as strings in the order of receipt

VS toposort(struct Graph & g) {

    std::vector<int> zero_outs; // Nodes that have no outgoing edges
    VS cycle;
    // copy counts of outgoing edges so we can modify
    std::vector<int> out = g.out_counts; 

    for (int i = 0; i < (int) out.size(); i++) {
        if (out[i] == 0) {
            zero_outs.push_back(i);
        }
    }

    while (!zero_outs.empty()) {
        int node = zero_outs.back();
        zero_outs.pop_back();

        for (int n : g.adj_list[node]) {

            out[n]--;
            if (out[n] == 0) {
                zero_outs.push_back(n);
            }
        }
    }

    for (int i = 0; i < (int)out.size(); i++) {
        if (out[i] != 0 && is_alnum(str[i])) {
            cycle.push_back(str[i]);
        }  
    }

    return cycle;
}

Result detect_deadlock(const std::vector<std::string> & edges) {
    Result result;
    result.edge_index = 0;

    Graph g;    
    Word2Int w2i;
    // VS str;

    // iterate through the vector "edges" and
    // populate the graph "g" accordingly 
    for (std::string line : edges) {
        // split(each string)
        VS v = split(line);
            // v[0] = process
            // v[2] = resource 
            // if v[1] == "->": requesting
            // if v[1] == "<-": assigned
        v[2] = v[2]+"*"; //append * to the name to differentiate from processes
        
        // convert string to int - process
        int a = w2i.get(v[0]);

        // if node is new, add it to out_counts
        if ((int)g.out_counts.size() == a) {
            g.out_counts.push_back(0);
            // store the process name in str
            str.push_back(v[0]);
        }
        
        // convert string to int - resource
        int b = w2i.get(v[2]);
        if ((int)g.out_counts.size() == b) {
            g.out_counts.push_back(0);
            // store the process name in str
            str.push_back(v[2]);
        }
        
        if (a+1 >= (int) g.out_counts.size() || b+1 >= (int)g.out_counts.size()) {
            int c = (a > b) ? a+1 : b+1;
            g.adj_list.resize(c);
            g.out_counts.resize(c);
        }

        if (v[1] == "->") {
            // add src node to dst node's adjacency list
            g.adj_list[b].push_back(a);
            g.out_counts[a]++;
        } else if (v[1] == "<-") {
            // add src node to dst node's adjacency list
            g.adj_list[a].push_back(b);
            g.out_counts[b]++;
        }
        
        // checking for cycle every time an edge is added
        result.dl_procs = toposort(g);

        // return if cycle is found
        if (!result.dl_procs.empty()) break;
        result.edge_index++;
    }
    if (result.dl_procs.empty()) result.edge_index = -1;

    return result;
}
