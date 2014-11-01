Project 2 : What’s your schedule
========

CS361-010 - Operating Systems (Fall 2014).

Overview
--------

In this project, you will implement a CPU scheduling simulator which takes as input a file, and provides as output details of simulated process execution. 

Your simulator must be implemented in C and run under Solaris. 

Description 
--------

The input to your program will be a file containing process descriptions.  Your program will be written in two pieces, a dispatcher, and a replaceable scheduler.  Your program will be used as follows:

 

Executing the program with no parameters will print usage information as follows:

Usage: sched <input file> <scheduling algorithm> <scheduling parameters>               

Supported schedulers:

1) FCFS – Parameters: Quantum, Cost of half C.S.

2) SJNFP – Parameters: Cost of half C.S.

 

For example, if you implement a FCFS scheduler and an SJNFP scheduler, executing 

Sched input.txt FCFS 10 2 would start the fcfs scheduler with a quantum of 10 and cost of a full context switch of 4, and executing sched input.txt SJNFP 2 would start the SJNFP scheduler with the cost of a full context switch at 4.

 

The input file
-----------

The input file will be of your creation.  As the project gets closer to a due date, I will provide a test file to make sure that your program conforms to the expected input.  Input files should have one entry per line and be of the form Ta N C1 D1 C2 D2…CN-1 DN-1 CN  where

Ta = Arrive time of process A

N = # of CPU bursts for process A

Ci=CPU burst i

Di=I/O Wait time i

 Notice that all processes start and end with a CPU burst.             

Output
-----------

When a process enters the ready queue your simulator should print a message  Process “P5” has arrived.

When a process enters the CPU for any burst your simulator should print a message Process “P5” is running

When a process starts waiting for IO, your simulator should print a message Process “P5” is waiting

When a process is preempted, your simulator should print a message Process “P5” was preempted, and returned to Ready state

When a process has completed an IO burst, your simulator should print a message Process “P5” has completed IO and returned to Ready state

When a process is complete, your simulator should print a message

Process “P5” terminated: TAT=%d, Wait Time=%d, I/O Wait=%d, Response Time=%d

When all processes terminate, your simulator should print a message with CPU Utilization, Avg/Max TAT, Avg/Max Wait Time, Avg/Max Response Time, etc.

A process entering the ready queue can be one of the following: 

a.           a new process,

b.           a process returning from Blocked state, or

c.           a process preempted from the CPU

When these three events happen at the same time, the new process will enter the Ready Q first, followed by process returning from Blocked state, and finally by the preempted process

Assignment 
---------

You must complete the project with 2 scheduling algorithms, both FCFS.  One preemptive, one non-preemptive.

For 10 POINTS EXTRA CREDIT, you must complete the project with 3 scheduling algorithms, FSFS (preemptive and non-preemptive) and SJN (Fully preemptive).
