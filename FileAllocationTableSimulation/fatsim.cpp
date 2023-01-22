// -------------------------------------------------------------------------------------
// Editted and submitted by: Mary Mo (ga.mo@ucalgary.ca)
// -------------------------------------------------------------------------------------
//
// (c) 2021, Pavol Federl, pfederl@ucalgary.ca
// Do not distribute this file.

#include "fatsim.h"
#include <cstdio>
#include <queue>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
 
struct Node {
  int64_t size;
  int64_t id;
};

// reimplement this function
std::vector<int64_t> fat_check(const std::vector<int64_t> & fat)
{
  // Create adjacency list
  std::unordered_map<int64_t, std::vector<int64_t>> adj_list;

  // Create vector/queue/stack of terminating blocks
  std::vector<int64_t> ter;

  for (uint64_t i = 0; i < fat.size(); i++) {
    adj_list[fat[i]].push_back(i);
    if (fat[i] == -1) ter.push_back(i);
  }

  // While terminating blocks is not empty
  std::queue<Node> update;
  std::vector<int64_t> res;
  while (!ter.empty()) {
  // take ONE of them, set its block length to 1
  // and push it to an empty queue
    // initialize the Node
    Node block(Node{1, ter.back()});
    // block.id = ter.front();
    ter.pop_back();
    // block.size = 1;
    update.push(block);
    int64_t max = 1;

    //   While queue is not empty, pop front, add each element of the node's
    //   adjacency list to queue, with length +1 more than current node
    while (!update.empty()) {
      for (auto k : adj_list[update.front().id]) {
        // Node bl;
        // bl.id = k;
        // bl.size = update.front().size+1;
        update.push(Node{update.front().size+1, k});
    //   keep track of max length
        if (max < update.front().size+1) max = update.front().size+1;
      }
      update.pop();
    }
    //   Once queue is empty, add max length to results vector.
    res.push_back(max);
  }

  std::sort(res.begin(), res.end());
  return res;
}

