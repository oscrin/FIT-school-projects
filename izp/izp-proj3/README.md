## [IZP] Project 3 - Simple cluster analysis

Simple cluster analysis over given data.

### Installation
```sh
$ gcc -std=c99 -Wall -Wextra -pedantic proj3.c -o proj3
```
or
```sh
$ make
```

### Parameters
```sh
$ ./proj3 <FILENAME> <NUMBER_OF_CLUSTERS>
```
### Input File Format
```sh
$ cat data.txt 
count=20
40 86 663
43 747 938
47 285 973
49 548 422
52 741 541
56 44 854
57 795 59
61 267 375
62 85 874
66 125 211
68 80 770
72 277 272
74 222 444
75 28 603
79 926 463
83 603 68
86 238 650
87 149 304
89 749 190
93 944 835
```
### Run Example
```sh
$ ./proj3 data.txt 5
Clusters:
cluster 0: 40[86,663] 47[285,973] 56[44,854] 61[267,375] 62[85,874] 66[125,211] 68[80,770] 72[277,272] 74[222,444] 75[28,603] 86[238,650] 87[149,304]
cluster 1: 43[747,938] 93[944,835]
cluster 2: 49[548,422]
cluster 3: 52[741,541] 79[926,463]
cluster 4: 57[795,59] 83[603,68] 89[749,190]
```
