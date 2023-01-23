**WARNING:** Do not upload any files in this repository to public websites. If you want to clone this repository, please make sure to keep it private.

# Worst-fit Dynamic Partition Memory Simulator
The simulator approximates some of the functionalities of `malloc()` and `free()` in the standard C library, using worst-fit algorithm.

To compile all code, type:
```
$ make
```
## Input
* Page size (a positive integer) and a list of allocation and deallocation reqeuests

## Allocation request
* Each allocation request has two parameters - a tag and a size.
* The program scans the list of partitions from start to end and finds a free partition using worst-fit algorithm. If more than one partition qualifies, the partition with the smallest address is used. If the partition is bigger than the requested size, the partition is split in two - an occupied and a free partition. The tag specified in the request is stored in the occupied partition.
* When the simulator fails to find a suitably large free partition, it simulates asking the OS for more memory. The amount of memory requeested from OS is a multiple of `page_size`. The newly obtained memory is appended at the end of the list of paritions, and if appropriate, merged with the last free partition.

## Deallocation request
* Deallocation requests have a single parameter - a tag, and is represented by a negative number in the input list of requests.
* The simulator finds all allocated partitions with the given tag and mark them free. Any adjacent free partitions are merged. If there are no partitions with the given tag, the simulator ignores such deallocation request.

## Limits
* `requests.size()` should be in the range [0 ... 1,000,000]
* `page_size` should be in the range [1 ... 1,000,000]
* Each request's `tag` should be in the range [-10,000,000 ... 10,000,000]
* Each request's `size` should be in the range [1 ... 10,000,000]

## Sample test files
```
$ wc -l test*.txt
      10 test0.txt
      17 test1.txt
       1 test2.txt
       1 test3.txt
   68807 test4.txt
 1000000 test5.txt
 1000000 test6.txt
      16 test7.txt
 2068852 total
```
---
## Expected results
```
$ ./memsim 123 < test1.txt
pages requested:                58
largest free partition size:    129
largest free partition address: 221
elapsed time:                   0.001

$ ./memsim 321 < test2.txt
pages requested:                16
largest free partition size:    136
largest free partition address: 5000
elapsed time:                   0.000

$ ./memsim 111 < test3.txt
pages requested:                0
largest free partition size:    0
largest free partition address: 0
elapsed time:                   0.000

$ ./memsim 222 < test4.txt
pages requested:                896
largest free partition size:    995
largest free partition address: 5
elapsed time:                   0.005

$ ./memsim 333 < test5.txt
pages requested:                141824
largest free partition size:    11707
largest free partition address: 29781916
elapsed time:                   0.571

$ ./memsim 606 < test6.txt
pages requested:                3558653
largest free partition size:    8807
largest free partition address: 857672560
elapsed time:                   1.483

$ ./memsim 100000 < test7.txt
pages requested:                1
largest free partition size:    99894
largest free partition address: 106
elapsed time:                   0.000
```
