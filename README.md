# Virtual Memory Management Simulation

## Description

This project is a Memory Management Simulation that utilizes various page fault algorithms. The simulation aims to demonstrate how memory is managed and how page fault algorithms handle memory access in different scenarios.

## Environment

- OS: Ubuntu (Linux)

### Requirements to Install

- gcc compiler

## How to Compile and Run

To compile the program, use the following command:

```
gcc -o MemoryManagement main.c
```

To run the program, use the following command:

```
./MemoryManagement useLru InputFileName OutputFileName m n
```

- `useLru` (1 or 0): If set to 1, the program will use the LRU (Least Recently Used) algorithm for page fault management.
- `InputFileName`: The name of the input file containing memory access patterns.
- `OutputFileName`: The name of the output file where the simulation results will be stored.
- `m`: The size of secondary memory (e.g., hard disk space).
- `n`: The size of main memory (e.g., RAM).

Note: If the file names contain parentheses '(', ')' characters, make sure to enclose the `OutputFileName` in double quotes when executing the command.

## Example

To run the program with the LRU algorithm and other parameters:

```
./MemoryManagement 1 "input.txt" "output(LRU).txt" 1000 50
```
## Author
- [Gil Ben Hamo](https://github.com/gilbenhamo)
