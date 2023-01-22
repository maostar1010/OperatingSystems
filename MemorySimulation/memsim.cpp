// -------------------------------------------------------------------------------------
// Editted and submitted by: Mary Mo (ga.mo@ucalgary.ca)
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
#include <unistd.h>
#include <iterator>


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
};

using LI = list<Partition>::iterator;

struct scmp { 
    bool operator()(const LI & c1, const LI & c2) const {
        if (c1->size == c2->size) 
            return c1->addr < c2->addr; 
        else 
            return c1->size > c2->size; 
    } 
};

struct Simulator {
    int64_t ps; // page size
    list<struct Partition> memory;
    unordered_map<int, vector<LI>> busy;
    set<LI,scmp> free_partition;

    Simulator(int64_t page_size)
    {
        // constructor
        ps = page_size;
    }
    void allocate(int tag, int size)
    {
        if (memory.empty()) { // if memory is empty, 
        // need to add the first free space of page_size
            memory.push_back(Partition{10000001, ps, 0, true});
            free_partition.insert(--memory.end());
            count ++;
        }

        LI it;
        if (!free_partition.empty() && (*free_partition.begin())->size >= size ) { // largest free space found to store the requested process
            it = *free_partition.begin(); // it = where we will store the process
            free_partition.erase(free_partition.begin());
        } else { // either no free partition or existing free partitions are not big enough
            if (memory.back().free) {
                it = --memory.end();
                free_partition.erase(it);
                
                count+=(int64_t)ceil((double)(size-memory.back().size)/(double)ps);
                memory.back().size += ceil((double)(size-memory.back().size)/(double)ps)*ps;
            } else {
                count+=(int64_t)ceil((double)size/(double)ps);
                memory.push_back(Partition{10000001, (int64_t)ceil((double)size/(double)ps)*ps, memory.back().addr + memory.back().size, true});
            }
            it = --memory.end();
        }

        if (it->size > size) {
            memory.insert(it, Partition{tag, size, it->addr, false});
            it->size -= size;      
            it->addr += size;      
            free_partition.insert(it);
            it--;
        } else {
            it->tag = tag;
            it->free = false;
        }
        busy[tag].push_back(it);
    }
    
    void deallocate(int tag)
    {
        if (busy[tag].empty()) return;
        if (busy.empty()) return;
        if (memory.empty()) return;

        for (auto part : busy[tag]) {
            part->tag = 10000001;
            part->free = true;
            
            if (part != memory.begin() && prev(part)->free) {
                
                free_partition.erase(prev(part));
                prev(part)->size += part->size;
                part = prev(part);
                memory.erase(next(part));
            }
            if (part != memory.end() && next(part)->free) {
                
                free_partition.erase(next(part));
                part->size += next(part)->size;
                memory.erase(next(part));
            }
            free_partition.insert(part);
        }
        busy[tag].clear();
        for (auto p : busy[tag]) {
            p->free = true;
            p->tag = 10000001;
        }

    }
    MemSimResult getStats()
    {
        MemSimResult result;
        result.max_free_partition_size = free_size;
        result.max_free_partition_address = free_addr;
        result.n_pages_requested = count;
        return result;
    }

    void print_memory(){
        if (free_partition.empty()) {
            return;
        }
        free_size = (*free_partition.begin())->size;
        free_addr = (*free_partition.begin())->addr;      
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
    Simulator sim(page_size);
    for (const auto & req : requests) {
        if (req.tag < 0) {
            sim.deallocate(-req.tag);
        } else {
            sim.allocate(req.tag, req.size);
        }
    }
    sim.print_memory();
    return sim.getStats();
}
