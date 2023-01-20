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
#include <vector>
#include <unordered_map>
#include <set>
#include <cmath>
// #include <unistd.h>

using namespace std;
int64_t count=0;
int64_t total=0;
int64_t free_size = 0;
int64_t free_addr = 0;

struct Partition {
    int64_t tag;
    int64_t size; 
    int64_t addr;
    bool free;
    // Partition(int t, int s, int a)
    // {
    // // Constructor with tag; occupied
    // tag = t;
    // size = s;
    // addr = a;
    // free = false;
    // }
};

using LI = list<struct Partition>::iterator;

bool cmp(const LI &a, const LI &b) {
    if (a->size == b->size) return a->addr < b->addr;
    return a->size > b->size;
}
using SN = set<LI, decltype(&cmp)>;

// I recommend you implement the simulator as a class. This is only a suggestion.
// If you decide not to use this class, please remove it before submitting it.
struct Simulator {
    int64_t ps; // page size
    list<struct Partition> memory;
    unordered_map<int, vector<LI>> busy;
    SN free_partition;

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
    // - if no suitable partition found:
    //     - get minimum number of pages from OS, but consider the
    //       case when last partition is free
    //     - add the new memory at the end of partition list
    //     - the last partition will be the best partition
    // - split the best partition in two if necessary
    //     - mark the first partition occupied, and store the tag in it
    //     - mark the second partition free

        // cout << "Requested tag = " << tag << ", and requested size = " << size << endl;

        if (memory.empty()) { // if memory is empty, 
        // need to add the first free space of page_size
            memory.push_back(Partition{10000001, ps, 0, true});
            free_partition.insert(--memory.end());
            count ++;
            // cout << "Memory now has a free space of size " << memory.back().size 
                // << ". Its free member has the value of " << memory.back().free << ".\n";
            // memory now has a free space of size ps
        }

        LI it = --memory.end();
        int tmp = size-1;
        // cout << "tmp = " << tmp << ", it->size = " << it->size << endl;
        // iterate through memory and find the largest free space that is 
        // big enough for this request
        for (LI part = memory.begin(); part != memory.end(); part++) { // iterate through memory
            if ( (part->free) && part->size > tmp) {
                tmp = part->size;
                it = part;
                // cout << "tmp = " << tmp << ", it->size = " << it->size << endl;
            }
        }
        if (it->free == false) { // if no appropriate free space was found 
        // and memory.back() is not free
            count+=(int64_t)ceil((double)size/(double)ps);
            memory.push_back(Partition{10000001, (int64_t)ceil((double)size/(double)ps)*ps, memory.back().addr + memory.back().size, true});
            it++;
            // free_partition.insert(it);
        } else if (it->size < size) {// no big enough space and memory.back() is free
            // free_partition.erase(--memory.end());
            count+=(int64_t)ceil((double)(size-memory.back().size)/(double)ps);
            memory.back().size += ceil((double)(size-memory.back().size)/(double)ps)*ps;
            // free_partition.insert(--memory.end());
        }

        // now it is pointing to the largest free space that is also big enough for the request
        // cout << "it: " << it->tag << " " << it->size << " " << endl;

        if (it->size > size) {
            memory.insert(it, Partition{tag, size, it->addr, false});
            // free_partition.erase(it);
            it->size -= size;      
            it->addr += size;      
            // free_partition.insert(it);
            it--;
        } else {
            // free_partition.erase(it);
            it->tag = tag;
            it->free = false;
        }
        busy[tag].push_back(it);

    }
    void deallocate(int tag)
    {
        // Pseudocode for deallocation request:
        // - for every partition
        //     - if partition is occupied and has a matching tag:
        //         - mark the partition free
        //         - merge any adjacent free partitions
        for (auto part : busy[tag]) {
            part->tag = 10000001;
            part->free = true;
            if (part != memory.begin() && prev(part)->free) {
                // free_partition.erase(part);
                prev(part)->size += part->size;
                part = prev(part);
                memory.erase(next(part));
            }
            if (part != memory.end() && next(part)->free) {
                // free_partition.erase(next(part));
                part->size += next(part)->size;
                memory.erase(next(part));
            }
            // free_partition.insert(part);
        }
        busy[tag].clear();
        // for (auto p : busy[tag]) {
        //     p->free = true;
        //     p->tag = 10000001;
        // }

    }
    MemSimResult getStats()
    {
    // let's guess the result... :)
    MemSimResult result;
    result.max_free_partition_size = free_size;
    result.max_free_partition_address = free_addr;
    result.n_pages_requested = count;
    return result;
    }
    void check_consistency()
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
    }
    void print_memory(){
        for (Partition part : memory) {
            // cout << part.tag << " " << part.size << " " << part.addr << " " << part.free << endl;
            total += part.size;
            if (part.free && part.size > free_size) {
                free_size = part.size;
                free_addr = part.addr;
            }
        }
        cout << "count = " << count << endl << "memory size = " << count*ps << endl 
            << "total = " << total << endl;
        
    }
};

// re-implement the following function
// ===================================
// parameters:
//    page_size: integer in range [1..1,000,000]
//    requests: array of requests
// return:
//    some statistics at the end of simulation
MemSimResult mem_sim(int64_t page_size, const std::vector<Request> & requests)
{
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
    }
    sim.print_memory();
    return sim.getStats();
}
