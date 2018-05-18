## [IZP] Project 2 - Iterative calculations

### Description
Program calculates natural logarithm using only +, -, *, / operations.
It also counts number of iteration for taylor polynom or continued fraction for given precision.

### Installation
```sh
$ gcc -std=c99 -Wall -Wextra -pedantic proj2.c -o proj2
```
or
```sh
$ make
```
### Run Syntax
```sh
./proj2 --log X N
```
or

```sh
./proj2 --iter MIN MAX EPS
```
X - number to count

N - number of iterations

MIN - lower value of interval

MAX - higer value of interval

EPS - precision

### Run Example
```sh
$ ./proj2 --log 1.131401114526 4
       log(1.1314) = 0.123456789012
    cf_log(1.1314) = 0.123456789012
taylor_log(1.1314) = 0.123452108537
```
```sh
$ ./proj2 --iter .31 3 1e-6
       log(0.31) = -1.1711829815
       log(3) = 1.09861228867
continued fraction iterations = 6
    cf_log(0.31) = -1.17118249665
    cf_log(3) = 1.09861206812
taylor polynomial iterations = 30
taylor_log(0.31) = -1.17118199531
taylor_log(3) = 1.09861197106
```
For more info see [zadani.pdf](/izp/izp-proj2/zadani.pdf).
