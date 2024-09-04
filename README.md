# A compilation of my CS 360 (systems programming) hw implementations
# Jesse A. Jones

# **DISCLAIMER** 
All PDFs in these folders are not mine and were made by Professor
Ben McCamish. They're mostly there to provide some indication of 
what each assignment involved beyond the main README you're looking at.

# **WARNING!!!!!**
These folders and the code contained therein are only meant to document
my abilities in C. 

If you use these to answer your WSUV CS 360 homework, 
I will hunt you down and start rambling about my programming language until
you die of boredom. Do you want that? I didn't think so! So, don't cheat
with this stuff, M'kay?

# Contents
Contained in this repo are the eight CS 360 homework assignments
I completed throughout fall semester in 2022. 

All of these programs are compiled and cleaned up using make
and an appropriate make file each. Compilation is always
done with the command `make` and cleanup is always done with 
`make clean`.

The programs are as follows:

## 1hw
A program involving parsing text files and counting word pairs,
displaying the resulting counts of word pairs in sorted descending
order.

This program involved implementing a hash table and some other data
structures and was a major first step in working with C for the class.

The program is also the only one among these 8 
that has native documentation in README.txt
a file explaining how to run it.

This was also probably the hardest program of the 8 to make.

### Running
Instructions for running this can be found in the README.txt file

## 2hw
This program involved using more in-depth file IO to see if an item
was located in a dictionary file. Nothing too special but it exists.

### Running
Type `make` in the directory and then run the generated EXE to see 
the results.

## 3hw
This involved using file IO and recursion to count files in a directory
and avoid things like sym links. This was an earlier exposure I had to
recursion and it was a massive headache at the time.

### Running
Compile using make and then run the EXE to see the results of counting
up the files in a specific directory.

## 4hw 
The first multi-process program of the bunch that involved effectively
recreating the unix pipe command, in order to allow a chain of execution
spawned from one program that called multiple programs in reliant sequence.
It was neat if a bit painful.

### Running
Compile and run like so:
```
./4EXE <arg1> : <arg2>
```

`<arg1>` is the first program to run in the sequence and 
`<arg2>` is the second.

## 5hw 
An implementation of the classic "Dining Philosophers" problem made using
processes spawned by the `fork()` function and semaphores to keep track
of resources the philosophers were using.

This program was an uncertain pain in the thorax but it was neat to 
be introduced to a more advanced multi-processing program.

### Running
Simply compile with make and run the 
exe to see the philosophers dine and think.

## 6hw 
Another implementation of Dining Philosophers, this time using 
threads and mutexes!

### Running
Same story here: compile and run the EXE.

## 7hw 
The second-hardest assignment only surpassed by the first one
due to needing to beat a time and the fact that it was the act of 
combining multi-threading with the quicksort algorithm. Ew.

### Running
Just compile and run this gross program.

## 8hw 
The networking one.

Basically, involved having a server program and client program connecting
over LAN using sockets and communicating some basic information.
This program served later as the basis for the final project of this
class in a repo also on my GitHub right here:

https://github.com/KesseNones/CS360FinalProject

### Running
Compile with `make` and run the exe like so:
```
./8EXE <server|client>
```
The `server` version will create a server that listens for clients
and the `client` version will spin up a basic client to try and
connect to a server.

Together, these two active at once will result in the server
detecting a new client and displaying the IP address the client
connected from and then writing the current time to the client
before disconnecting the client.

# Conclusion
CS 360 was a huge pain of a class but definitely a huge source 
of my abilities in the C programming language. 
















