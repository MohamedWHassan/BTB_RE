# Branch Target Buffer Reverse Engineering for RISC V
## Description
The repository contains michrobenchmark for branch predictors that generates a series of conditional branches.
The user can control the number of branches (B) and the distance (D) between them. The distance between branches indicates the index that each branch will correspond to in the BTB, while the number of branches indicates if the entries in the BTB are going to be filled or not. \
The michrobenchmark uses [perf](https://perfwiki.github.io/main/) to calculate the branch missrate at the end of the experiment. In normal conditions when B = number of entries in BTB and D = D_f ,where a fitting distance results in a branch missrate of zero, BTB can be reverse engineered.  
## How To Run
Compile using the command
```
make all
```
Run the experiment using
```
bash experiment.bash
```



