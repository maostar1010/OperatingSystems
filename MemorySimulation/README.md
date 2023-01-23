**WARNING:** Do not upload any files in this repository to public websites. If you want to clone this repository, please make sure to keep it private.

# Worst-fit Dynamic Partition Memory Simulator

To compile all code, type:
```
$ make
```

## Limits
* requests.size() in range [0 ... 1,000,000]
* page_size in range [1 ... 1,000,000]
* Each request's tag in range [-10,000,000 ... 10,000,000]
* Each request's size in range [1 ... 10,000,000]

## Sample test files
```
$ $ wc -l test*.txt
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
