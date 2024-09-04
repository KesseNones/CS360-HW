Readme for CS 360 Assignment I 
Written by Jesse Jones

Compiling:
	A make file is provided with the source code. 
To compile the program, simply type "make" in the terminal 
when in the same directory as the source code. 
This will automatically compile all of the code 
and create an executable called "wordpairs".

Running the Program:
	To run this program, 
first compile it following the instructions above, 
then run the executable in this format:

./wordpairs <-count> fileNameOne <fileNameTwo> <fileNameThree> ...

	The parameters in angle brackets are optional.
The count parameter tells the program how many word pairs it will print 
in descending order, ranging from 1 to the signed int limit. 
Though if there are fewer items to print than the count given, 
all word pairs will be printed in the hashtable. If no count is given, 
the program will print every word pair in the hashtable.
If an invalid count is given, an error will be thrown 
and the program will exit.

	The only mandatory parameter is one file to parse though (fileNameOne).
If the one file is invalid, it will through an error. 
The other files that can be listed as arguments are optional.
If one of the files given is invalid, an error will be thrown.
Lastly, if no arguments are given an error will be thrown.

Misc. Information:   
	This folder contains the header files 
and c files for getWord and crc64. 
	These files were not made by Jesse Jones but were used 
in the main program files for file parsing and hash code making.  
