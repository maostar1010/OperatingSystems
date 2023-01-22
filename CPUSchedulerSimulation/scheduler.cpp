// -------------------------------------------------------------------------------------
// Editted and submitted by: Mary Mo (ga.mo@ucalgary.ca)
// -------------------------------------------------------------------------------------
//
// (c) 2021, Pavol Federl, pfederl@ucalgary.ca
// Do not distribute this file.

#include "scheduler.h"
#include "common.h"
#include <iostream>
#include <vector>

void simulate_rr( int64_t quantum, int64_t max_seq_len, std::vector<Process> & processes, std::vector<int> & seq) {
    int64_t curr_time=0, cpu = -1, remaining_slice = quantum, remaining_bursts[processes.size()];
    std::vector<int> rq, jq; // jq = only to ensure all processes have been completed
    seq.clear();

    // initialize jq
    for (uint64_t i = 0; i < processes.size(); i++) {
        jq.push_back(i);
        // initialize remaining_bursts;
        remaining_bursts[i] = processes[i].burst;
    }

    if (!processes.empty()) curr_time = processes[0].arrival_time;
    if (curr_time != 0) seq.push_back(-1);

    while (!jq.empty() || !rq.empty()) {
       
        // int64_t i = processes[jq[0]].arrival_time;
        while (processes[jq[0]].arrival_time == curr_time) {
            if (jq.empty()) break;
            rq.push_back(jq[0]);
            jq.erase(jq.begin());
        }
        
        if (!rq.empty()) {// when rq is NOT empty

            // new cpu updated
            cpu = rq[0];        
            // set the start time for the process
            if (processes[cpu].start_time == -1) processes[cpu].start_time = curr_time;
            
            // update sequence
            if (seq.empty() || seq.back() != cpu) {
                seq.push_back(cpu);
            }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// OPTIMIZATION 2

            int64_t rep = -1;
            // check the next arrival time
            if (!jq.empty()) {
                if (rq.empty()) rep = (processes[jq[0]].arrival_time - curr_time) / (quantum);
                else rep = (processes[jq[0]].arrival_time - curr_time) / (quantum*rq.size());
            }
            
            // check how many slices can be completed for each process in rq
            // without the processes running out of their remaining bursts
            int64_t s = -1;
            for (int i : rq) {
                s = remaining_bursts[i] / (quantum*rq.size());
                if (s < 0) break;
                if (rep > s || rep == -1) rep = s;
            }

            if (rep < 1) rep = 1;

            if (rep > 1) {
                for (size_t i = 0; i < rq.size(); i++ ) {
                    remaining_bursts[rq[i]] -= rep*quantum;
                    if (processes[rq[i]].start_time == -1) processes[rq[i]].start_time = curr_time + quantum*i;
                }
                curr_time += rep*rq.size()*quantum;
                
                int c = 0;
                while (rep > 0) {
                    if (c > max_seq_len) rep = 0;
                    for (int i : rq) {
                        // seq.push_back(i);
                        if (seq.empty() || seq.back() != i) {
                            seq.push_back(i);
                        }
                    }
                    rep--;
                    c++;
                }
            }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// OPTIMIZATION 1
            // // if the process doesn't end in the given time slice, skip to the end of the time slice
            else if (remaining_bursts[cpu]-remaining_slice > 0) {
                
                remaining_bursts[cpu] -= quantum;
                rq.erase(rq.begin()); // pop rq
                
                while (processes[jq[0]].arrival_time < curr_time+quantum) {    
                    if (jq.empty()) break;
                    rq.push_back(jq[0]);
                    jq.erase(jq.begin());
                    
                }

                rq.push_back(cpu); // add it back to rq at the back
                curr_time += quantum;

            } else { // if the process ends within the time slice
                
                processes[cpu].finish_time = curr_time + remaining_bursts[cpu];
                rq.erase(rq.begin());

                while (processes[jq[0]].arrival_time < curr_time+remaining_bursts[cpu]) {    
                    if (jq.empty()) break;
                    rq.push_back(jq[0]);
                    jq.erase(jq.begin());
                }
                curr_time += remaining_bursts[cpu];
                remaining_bursts[cpu] = 0;
            }

            

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            
        } else { // when rq is empty and jq is not empty
            if (seq.empty() || seq.back() != -1) {
                seq.push_back(-1);
            }
            // remaining_slice = quantum;
            curr_time = processes[jq[0]].arrival_time;
        }
    }
    
    if (max_seq_len < (int)seq.size()) seq.resize(max_seq_len);
}