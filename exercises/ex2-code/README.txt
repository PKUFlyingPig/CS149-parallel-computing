This directory contains three versions of the barrier synchronization
code for the first part of Exercise 2.  The verions are:

rconverge1: Uses the three-barrier solution of Lecture 05, Slide #48.

rconverge2: Uses a single-barrier solution, based on software pipelining

rconverge3: Also a single-barrier solution, similar to that of Slide #49

All three versions can be compile (on a Linux machine) using the make command.

All three take up to two arguments:

    B: The batch size (= number of threads)  [Default = 16]
    T: The target threshold. 	   	     [Default = 0.250]

Increasing B or decreasing T will increase the number of phases the program runs.

See the comments in the Makefile for information on making the
programs print less information.
