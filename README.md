# Testbench-Generator
To use the tool, on the terminal use command lines:

g++ main.cpp -o main

to compile
then use

./main input_verilog_file_name

to run and dump the testbench on the terminal


We were not able to invoke from the command line using tbgen as tbgen is a python exclusive functionality and we used C++ for our project

Some problems we faced was not being able to read inputs with multiple bits such as [5:0] a, however, we were able to alter our code to include inputs of different sizes.

Since we were not able to find parse libraries for c++, the generator takes only modules that specifies input and output variables inside the brackets.

