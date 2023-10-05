# Data Structure Experiment -- A crude front-end of a compiler
## Environment:
    Operating System: wsl2 (Debian 12.2.0-14)
    Architecture: x86_64
    Byte Order: Little Endian
    Compiler: c++ (Debian 12.2.0-14) 12.2.0
    cmake version 3.25.1

## How to run
1. You can just run the execute.sh file in the terminal
and it will compile and run the program for you.
2. Or you can execute the Front_End_Compiler file in the terminal directly.

## Scanner
My scanner can only recognize the basic tokens of C. The pointer, struct, union, and the comma operator can not be recognized correctly.
## Parser
I used a [n-ary tree library](https://github.com/kpeeters/tree.hh) to implement my parser.
## So-called intermediate code generator
It just generate the code which is the same as the source code.
## More
Maybe not robust.
Still has lots of details to add.


