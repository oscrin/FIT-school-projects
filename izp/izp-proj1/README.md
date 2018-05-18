## [IZP] Project 1 - Words recognition

### Description
This program takes input from stdin and classifies every given word to stdout as:
1.  word
2.  number
3.  date

For every word also checks for palindrome, similarly numbers are checked for prime numbers.

#### Word class

```sh
word: hello
```
or

```sh
word: anna (palindrome)
```
#### Number class

```sh
number: 123456
```
or

```sh
number: 41 (prime)
```

Program classifies only positive numbers with value lower than INT_MAX.

#### Date class
```sh
date: Tue 2015-09-01
```

### Installation
```sh
$ gcc -std=c99 -Wall -Wextra -pedantic proj1.c -o proj1
```
or
```sh
$ make
```

### Run Example
```sh
$ cat data.txt
Created on 2015-08-26
having 13 words out which 8 = 4+4 contains o.
```
```sh
$ proj1 <data.txt
word: Created
word: on
date: Wed 2015-08-26
word: having
number: 13 (prime)
word: words
word: out
word: which
number: 8
word: = (palindrome)
word: 4+4 (palindrome)
word: contains
word: o.
```
For more info see [zadani.pdf](/izp/izp-proj1/zadani.pdf).
