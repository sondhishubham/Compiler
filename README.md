# Compiler

Run make to get the compiler.

Run ./cc <file.c> to run the compiler on file.c

You can run the example .cc examples/helloworld.c

It will generate a file cc.ll

Run the command lli cc.ll to execute the program.




To check correctness:

Run "clang -emit-llvm -S file.c -o file.ll", it will generate a file "file.ll"

Run the command to lli file.ll to execute the program and use it for comparison.
