
Description of the program:
The Program is an implementation of an A* algorithm for the graph traversal, using the input from 2 files: locations.txt and connections.txt
locations.txt contains the name of a node followed by the X and Y coordinate. 

connections.txt contains the name of the node from which the connections are drawn, the number of nodes the origin is connected to and the names of nodes the origin connects to.

The program allows for a variety of output of A* algorithm traversal between the two nodes as well as to select the nodes that you would want to exclude from your final path. 

If you choose the step-by-step option, the program will output the step process by which it decides on the shortest path and the final path and total distance. 

You can also select the heuristic according to which the algorithm will work: "Straight line distance" or "Fewest Cities".

Instructions on Running the program:

To compile with C++ in the terminal enter:
g++ -Wall driver.cpp

./a.out if on UNIX shell 
a.exe if on Windows cmd

1. Select the starting location.
2. Select the finish location. 
3. Type the cities you do not want to include in your final path. Separate them by space or enter and press q and enter to continue.
4. Select Y for step-by-step or N for not. 
5. Select the heuristic you would like to use. 

The program was initially compiled on a Windows 10 64-bit machine with the following compiler:

g++ (MinGW.org GCC-8.2.0-3) 8.2.0
Copyright (C) 2018 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
