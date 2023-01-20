#include "memsim.h"
#include <cassert>
#include <iostream>
#include <list>
#include <set>
#include <unordered_map>
#include <vector>
#include <cmath>
// #include <iterator>
#include <unistd.h>

using namespace std;
int64_t count = 0; 
int64_t total = 0; // for checking purposes

// Linked List implementation
// Hash Table implementation for deallocation optimization
// Balanced Tree implementation for allocation optimization - also adjust deallocation

struct Partition {
  int64_t tag, size, addr;
  bool free;
  Partition(int t, int s, int a)
  {
    // Constructor with tag; occupied
    tag = t;
    size = s;
    addr = a;
    free = false;

  }
  Partition(int s, int a)
  {
    // Constructor without tag; free
    size = s;
    addr = a;
    free = true;
  }
};

using MI = list<struct Partition>::iterator;

// =============== BST Data Structure =================
bool cmp(const MI &a, const MI &b) {
  if (a->size == b->size) return a->addr < b->addr; // if same size, smaller address first
  return a->size > b->size; // larger size first
}

using SS = set<MI, decltype(&cmp)>;
// ====================================================

struct Simulator {

  list<struct Partition> mem;
  unordered_map<int, vector<MI>> busy;
  SS fr(&cmp);
  int64_t ps; // page size

  Simulator(int64_t page_size)
  {
    // constructor
    ps = page_size;
  }
  void allocate(int tag, int size)
  {
    // Pseudocode for allocation request:
    // - search through the list of partitions from start to end, and
    //   find the largest partition that fits requested size
    //     - in case of ties, pick the first partition found

    // - split the best partition in two if necessary
    //     - mark the first partition occupied, and store the tag in it
    //     - mark the second partition free

    // very first time of allocating, need to allocate free space
    if (count == 0) {
      Partition p(ps, 0); // free space of one page size, address of 0
      count++; // 1 page requested
      mem.push_back(p);
      fr.insert((--mem.end()));
    } //else { 
    
    // now mem free space already initialized
    // here: mem.size() != 0
    if ((*(fr.begin()))->size < size) { // if not enough free space is left
      if (--mem.end()->free == true) { // last space is free
        fr.erase(--mem.end()); // erase from the free set
        count += ceil((double)(size-(--mem.end()->size))/(double)ps);
        --mem.end()->size += ceil((double)(size-(--mem.end()->size))/(double)ps)*ps;
      } else { // last space is not free
        count += ceil((double)size/(double)ps);
        Partition f(ceil((double)size/(double)ps)*ps, prev(mem.end())->addr+prev(mem.end())->size);
        mem.push_back(f);
      }
      fr.insert(--mem.end()); // insert back to the free set
    }

    // now there must be a free space that is big enough
    // if ()
      
  }

  void deallocate(int tag)
  {
    // Pseudocode for deallocation request:
    // - for every partition
    //     - if partition is occupied and has a matching tag:
    //         - mark the partition free
    //         - merge any adjacent free partitions
  }

  MemSimResult getStats()
  {
    // let's guess the result... :)
    MemSimResult result;
    result.max_free_partition_size = 123;
    result.max_free_partition_address = 321;
    result.n_pages_requested = 111;
    return result;
  }

  void check_consistency() // only for debugging purposes; comment it out later before submission
  {
    // you do not need to implement this method at all - this is just my suggestion
    // to help you with debugging

    // mem_sim() calls this after every request to make sure all data structures
    // are consistent. Since this will probablly slow down your code, you should
    // disable calling this in the mem_sim() function below before submitting
    // your code for grading.

    // here are some suggestions for consistency checks (see appendix also):

    // make sure the sum of all partition sizes in your linked list is
    // the same as number of page requests * page_size
    // make sure your addresses are correct

    // make sure the number of all partitions in your tag data structure +
    // number of partitions in your free blocks is the same as the size
    // of the linked list

    // make sure that every free partition is in free blocks

    // make sure that every partition in free_blocks is actually free

    // make sure that none of the partition sizes or addresses are < 1

    // ========================================================================
    // some of the above steps can be combined together
  }
};

// re-implement the following function
// ===================================
// parameters:
//    page_size: integer in range [1..1,000,000]
//    requests: array of requests
// return:
//    some statistics at the end of simulation
MemSimResult mem_sim(int64_t page_size, const std::vector<Request> & requests) {

  Simulator sim(page_size);
  for (const auto & req : requests) {
    if (req.tag < 0) {
      sim.deallocate(-req.tag);
    } else {
      sim.allocate(req.tag, req.size);
    }
    sim.check_consistency();
  }

  return sim.getStats();
}
