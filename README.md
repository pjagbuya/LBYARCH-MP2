# LBYARCH 2: MP2 GrayScale Single Precision Image to Unsigned 8-Bit Integer

Running the program:
1.) input1.txt
2.) Y

For the following tests, the following becnhmark results are produced with the corresponding execution times. Each run has a different randomized input within that dimension over. It will repeat further 30 times for the average execution time. 
Enter matrix dimensions: 10x10, 100x100, and 1000x1000. Each program has been run 30 times with the same corresponding randomized inputs. Three sample runs are provided to see how the run of each code in C and x86-64 assembly is running.
Running tests for matrix size 10 x 10:

				ASM				    C Lang
        Run 1: Time taken for asm: 0.004700ms      |      Time taken in c: 0.445800ms
        Run 2: Time taken for asm: 0.004700ms      |      Time taken in c: 0.033500ms
        Run 3: Time taken for asm: 0.003700ms      |      Time taken in c: 0.032400ms

Time average asm = 0.003630ms
Time average c = 0.046357ms
===========================

Running tests for matrix size 100 x 100:

        Run 1: Time taken for asm: 0.352000ms      |      Time taken in c: 6.471900ms
        Run 2: Time taken for asm: 0.350700ms      |      Time taken in c: 6.529800ms
        Run 3: Time taken for asm: 0.354700ms      |      Time taken in c: 6.538400ms

Time average asm = 0.367500ms
Time average c = 6.653730ms
===========================

Running tests for matrix size 1000 x 1000:

        Run 1: Time taken for asm: 42.306500ms      |      Time taken in c: 726.811500ms
        Run 2: Time taken for asm: 44.212200ms      |      Time taken in c: 725.536700ms
        Run 3: Time taken for asm: 41.629300ms      |      Time taken in c: 721.764600ms

Time average asm = 48.836947ms
Time average c = 736.098403ms
===========================

## Analysis
A bit of background beneath the processes in ``*imgCvtGrayFloatToInt()*`` and ``*imgCvtGrayFloatToInt_c()*``. Functions belonging to x86-64 asm and C respectively. Array of 2-Dimensions was implemented in C, and it is how it was allocated. Making ``*imgCvtGrayFloatToInt()*`` need to process two pointers and then correspondingly converting it via multiplying to 255.0 then back to integer. The C programming function would need additional logic comparing the round to nearest and an additional ties to even condition.

In the 10x10 instance, it is apparent that the assembly implementation of the function ``*imgCvtGrayFloatToInt()*`` is about about 91.17% faster than the C programming implementation. This can be due to the fact the C program needs a bit more rounding logic to align with the round to nearest ties to even that the result of cvtss2si from SIMD functions that happen in the assembly language.

Same conclusion can be met with 100x100, only for this instance its a bit more prevalent. Around 94.48% faster on average than the results produce from the C program implementation.

Lastly, the 1000x1000 which means the resulting averages make the x86-64 function be 93.34% faster than the C function. Making the percantage of improvements that ``*imgCvtGrayFloatToInt()*`` cause to be averaged around 93.33% better than the C function implementation.


Inputs are randomized and displayed only for 10x10
![alt](./pics/1.png)
![alt](./pics/"Pasted image 20241202000530".png)
![[Pasted image 20241202000509.png]]



# Correctness test

For the correctness test we will be using python's round function as their integer rounding follows round to nearest, ties to even conditions. It is run under PyCharm Community 2024 as the IDE, or simply use the command python main.py. The code implementation of python follow produce as so:
![[Pasted image 20241202005119.png]]
Our main focus for checking is whether pythons calculated result would be equal the result of either from C or assembly. Using the numpy function for equal, this line for `print(np.array_equal(new, given))` we will easily get a brief indication on the lower dimension of 10x10 that the result is true or false at the last line of the output.

# Results of Correctness:

## Given Calculated x86-64 asm
![[Pasted image 20241202001234.png]]
Results of Correctness:

## Given Calculated C Program
![[Pasted image 20241202001318.png]]

Overall, both functions produced accurate information that is expected to follow under the assumption: Map Grayscale float to 255.0 then follow round to nearest integer then ties to even. Following the proportion below
![[Pasted image 20241202003630.png]]
That simplifies to the equation:  `255f = i


# Short video
