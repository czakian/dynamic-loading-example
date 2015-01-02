dynamic-loading-example
=======================

Building: 

Type make from the root directory of this repository. 

Running: 

Run by executing the generated executable: ./dynamic_loader.exe 5

This command will kick off a loop which prints the result of executing the dynamically loaded function from the loaded_module.so. 
In this case, the fibonnacci sequence number for the input is generated. In a different teriminal while you leave dynamic_loader.exe running,
you can flip between fibonnacci implementations in loaded_module.c by commenting out the different calls between fib() and fib_accum()
and recompile via make, and then watch the running executable dynamically pick up the new fibonnacci implementation. 
