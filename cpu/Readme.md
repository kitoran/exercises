#cpu

A largely untested  program that compiles a supercombinator with arithmetic operations and runs it. The program is read from file "program". Functions can't be applied to more arguments than their arity (for example, a program "(\x.x) (\y.y) 5" is illegal). Works with x64 and assumes System V AMD64 ABI calling convention.
