// -------------------------------------------------------------------------------------
// this is the only file you need to edit
// -------------------------------------------------------------------------------------
//
// (c) 2022, Pavol Federl, pfederl@ucalgary.ca
// Do not distribute this file.

#include "memsim.h"
#include <cassert>
#include <iostream>
#include <list>
#include <cmath>
#include <iterator>
#include <iostream>

using namespace std;
typedef std::list<struct Partition>::iterator PartitionRef;
int count = 0; 
int64_t total=0; // for checking purposes

// Linked List implementation
// Hash Table implementation for deallocation optimization
// Balanced Tree implementation for allocation optimization - also adjust deallocation

struct Partition {
  int tag, size, addr;
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

struct Simulator {

  list<struct Partition> mem;
  int64_t page;

  Simulator(int64_t page_size)
  {
    // constructor
    page = page_size;
  }
  void allocate(int tag, int size)
  {
    // for the very first time allocating a memory, request a page from the OS
    if (mem.size() == 0) {
      Partition p(page, 0); // free partition with the size of one page
      mem.push_back(p);
      count++;
      cout << "1) count = " << count << endl;
      cout << "   p.size = " << mem.back().size << endl;
    }

    // Pseudocode for allocation request:
    // - search through the list of partitions from start to end, and
    //   find the largest partition that fits requested size
    //     - in case of ties, pick the first partition found
    PartitionRef ptt; // finding the free partition with the largest size
    int ss = 0;
    for (PartitionRef p = mem.begin(); p != mem.end(); p++) {
      if ((p->free) && p->size >= size) {
        if (ss == 0){
          ptt = p;
          ss++;
          cout << "11" << endl;
        } else if (ptt->size < p->size) {
          ptt = p;
          cout << "22" << endl;
        }
      }
    }

    // - if no suitable partition found:
    if (ss == 0) {  
    //     - get minimum number of pages from OS, but consider the
    //       case when last partition is free
    //     - add the new memory at the end of partition list
    //     - the last partition will be the best partition
      // int rqs;
      cout << "00" << endl;
      if (mem.back().free) {
        // mem.pop_back();
        // Partition p( ceil((size-mem.back().size)/page) * page, prev(mem.end()-1)->addr + prev(mem.end()-1)->size); 
        // rqs = size - mem.back().size;
        // merge
        count += ceil((double)(size-mem.back().size)/(double)page);
        cout << "2) count = " << count << endl;
        cout << "   number of pages added: " << ceil((double)(size-mem.back().size)/(double)page) << endl;
        cout << "   ummmm? " << ceil((double)(size-mem.back().size)/(double)page) << endl;
        mem.back().size += ceil((size-mem.back().size)/page)*page;

      } else {
      
        Partition p( ceil((double)size/(double)page) * page, prev(mem.end())->addr + prev(mem.end())->size); 
        mem.push_back(p);
        count += ceil(size/page);
        cout << "3) count = " << count << endl;
        // rqs = size;
      }

      // rqs = ceil(rqs/page);
      
      ptt = --mem.end();

      // count += ceil(size/page);
    }

    // - split the best partition in two if necessary
    //     - mark the first partition occupied, and store the tag in it
    //     - mark the second partition free
    if (ptt->size > size) {
      Partition p(ptt->size - size, ptt->addr + size);
      mem.insert(ptt, p);
      ptt->size = size;
    }
    ptt->tag = tag;
    cout << "   mem.back().size = " << mem.back().size << endl;

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
    total = 0;
    for (Partition part : mem) {
      total += part.size;
    }
    cout << "current mem size = " << total << endl;
    

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
  // if you decide to use the simulator class, you probably do not need to make
  // any changes to the code below, except to uncomment the call to check_consistency()
  // before submitting your code
  Simulator sim(page_size);
  for (const auto & req : requests) {
    if (req.tag < 0) {
      sim.deallocate(-req.tag);
    } else {
      sim.allocate(req.tag, req.size);
    }
    sim.check_consistency();
    // after processing every request, check the consistency of all data structures
  }

  ////////////////////////////////////////////////
  // total = total/page_size;
  cout << "total =  " << total << std::endl;
  cout << "count = " << count << std::endl;
  for (Partition part : sim.mem) {
    cout << part.tag << " " << part.size << " " << part.addr << endl;
  }
  ////////////////////////////////////////////////

  return sim.getStats();
}
