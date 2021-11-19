# Packman-School-Project
The code from a 2nd year school project where we had to write a C program which could encode and decode text files using a Huffman Tree

This program was written for a project in the class CSCI-243: Mechanics of Programming on November 19th, 2021. In this project, I was tasked with writing a program called "Packman", which would either take a text file and encode it into binary, or take a binary file and decode it back into text, utilizing a Huffman Tree. Aside from the files from the initial commit, everything was written by me, as indicated in the headers of each file.

There are 3 ways Packman can be used, listed below as they were written in the rubric of the project:

### Encoding from a textfile to a codefile

 >*$ packman textfile codefile*

If the textfile content *does not begin with a magic number*, then the program interprets the string as the name of a file whose content is to be encoded.

If the textfile does not exist, is zero length, or is unreadable, the program produces an error message reporting the problem to stderr and terminate with EXIT_FAILURE after first cleaning up any dynamic memory it may already have allocated.

If codefile is a *file name string* and the codefile does not exist, the program creates the codefile. If codefile exists and is writable, then the program *overwrites* the codefile.

If the codefile is not writable for any reason, the program produces an error message reporting the problem to stderr and terminate with EXIT_FAILURE after first cleaning up any dynamic memory it may have allocated.

### Decoding a codefile to stdout
>*$ packman codefile -*

If the first argument is a *file name string*, and its file content *begins with the magic number*, then the program decides to decode the encoded file content.

If the codefile does not exist or is not readable, then the program produces an error message reporting the problem to stderr and terminate with EXIT_FAILURE after first cleaning up any dynamic memory it may have allocated.

When the second command line argument is the *hyphen* character, then the program writes the decoded result to stdout.

### Decoding a codefile to a textfile
>*$ packman codefile textfile*

If the first argument is a *file name string*, and its file content *does contain the magic number*, then the program interprets the string as a codefile name and decides to decode the encoded file.

If the codefile does not exist or is not readable, then the program produces an error message reporting the problem to stderr and terminate with EXIT_FAILURE after first cleaning up any dynamic memory it may have allocated.

If the second command line argument is a *file name string*, and the file does not exist, the program creates the file. If textfile exists and is writable, then the program overwrites textfile.

If the textfile is not writable for any reason, the program produces an error message reporting the problem to stderr and terminate with EXIT_FAILURE after first cleaning up any dynamic memory it may have allocated.
