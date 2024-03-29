**WARNING:** Do not upload any files in this repository to public websites. If you want to clone this repository, please make sure to keep it private.

# Deadlock Detection

To compile all code, type:
```
$ make
```

To run the resulting code on file test1.txt:
```
$ ./deadlock < test1.txt
```

## Test files

These are the correct results for the test files included in this repo.

| filename   | correct `edge_index` | correct `dl_procs` | minimum timings     | good timings     |
| :---------- | :-------------: | :-----------: | :-----------------: | :--------------: |
| `test1.txt` | `-1`            | `[]`          | `0.0000s`           | `0.0000s`        |
| `test2a.txt`| `6`             | `[4,5,7]`     | `0.0000s`           | `0.0000s`        |
| `test2b.txt`| `5`             | `[5,7]`       | `0.0000s`           | `0.0000s`        |
| `test3a.txt`| `3`             | `[p7,p3]`     | `0.0000s`           | `0.0000s`        |
| `test3b.txt`| `-1`            | `[]`          | `0.0000s`           | `0.0000s`        |
| `test4.txt` | `3`             | `[12,7]`      | `0.0000s`           | `0.0000s`        |
| `test5.txt` | `6`             | `[2,77]`      | `0.0000s`           | `0.0000s`        |
| `test6.txt` | `9903`          | `[ab,cd,ef]`  |`8.9431s`            | `0.8771s`        |
| `test7.txt` | `29941`         | `[is,this,answer,the,correct]`  |`191.7872s`    | `8.0726s`        |


