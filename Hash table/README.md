The information below is also in the following [file](https://github.com/x-ENIAC/MIPT_projects_2_sem/blob/master/Hash%20table/Report.pdf).

# Hash table optimization

## Intro

Hash tables are widely using in field of working with data. A hash table is a data structure that provides fast, instant access to elements, their search and insertion in constant O(1) time. A conflict occurs when two keys map to the same index. This situation is called a collision. There are several ways to deal with this: hash tables with open addressing and hash tables with chains. In this work, the author has chosen the second method. A hash table represents an array of linked lists. All keys mapped to the same index will be stored as linked list nodes in that index.


But this data structure is not always fast. This is due to the fact that the compiler does not always know how to optimize the cunning algorithms written by the programmer. On the other hand, he knows it. This is where the idea appears to speed up the hash table.


The author will speed up this data structure by rewriting the two functions in the assembler in two different ways.

## Materials

The input data is supposed to be an English-Russian dictionary with more than 6500 key-value pairs. To test the running time of the hash table, the author decided to create 100,000 random words and search for them in the table. Also, in this work, a fast and secure doubly linked list is used, which was previously written by the author. To measure the duration of the callgrind profiler was used the kcachegrind application. The work was done on Ubuntu 20.04.1 with an Intel Core i5 processor.

Below are the formulas for calculating the acceleration factors:

<img src="https://github.com/x-ENIAC/MIPT_projects_2_sem/blob/master/Hash%20table/Pictures_for_readme/1.jpg" alt="drawing1" width="200"/>

where time_1 and time_2 - results before and and after accelerations.

To calculate the total acceleration by the formula, we will use the following formula: 

<img src="https://github.com/x-ENIAC/MIPT_projects_2_sem/blob/master/Hash%20table/Pictures_for_readme/2.jpg" alt="drawing2" width="400"/>

where lines_assembler - the number of lines rewritten in assembler.


## Work progress

### Analysis of the program runtime

Let's analyze how the program functions work using callgrind.

<img src="https://github.com/x-ENIAC/MIPT_projects_2_sem/blob/master/Hash%20table/Pictures_for_readme/log_callgrind_1.jpg" alt="drawing3" width="600"/>

Profiling by the category "self" showed that the longest functions (that be written not by developers C++) is:

- get_hash_word: the function that calculate hash of the string. It just needs to be accelerated, because it is called very often.

- testing_hash_table: the function that creates 100 000 random strings and find its in hash  table. Obviously, it is useless to optimize.

- parsing_buffer: the function is called at the stage of text processing. Optimization is not required as it is performed only once. Also, this function is used locally (when loading data selected by the author).
  
- hash_table_is_contain_element: the function checks if the hash table contains an item or not. It is also highly desirable to speed it up due to frequent calls.

Great, we found two functions that we want to speed up. Let's get down to deal!

### Optimization №1

First, let's optimize the function that calculates the hash of words. Crc32 was chosen as the hashing algorithm, the polynomial 0x82f63b78 was used. The optimization took 11 lines in the assembler, in which the hash is calculated.


<img src="https://github.com/x-ENIAC/MIPT_projects_2_sem/blob/master/Hash%20table/Pictures_for_readme/asm_first_optimization.jpg" alt="drawing4" width="200"/>

### First comparison

You can see the optimization results (performance comparison) in the picture and table below.

With the optimization flag -O1, you can notice the following: according to formula (1), the program has accelerated 1.24 times, and Ded's number (formula (2)) is approximately equal to 112. This optimization has accelerated the work of the program by 19%.

Comparison of times before and after the first acceleration (-O1):

<img src="https://github.com/x-ENIAC/MIPT_projects_2_sem/blob/master/Hash%20table/Pictures_for_readme/log_callgrind_2.jpg" alt="drawing5" width="900"/>

Comparison of times before and after the first acceleration:

| Values | -O1 |
|----------------|----------------|
| coeff_boost | 1.24 |
| coeff_total_accelerate | 112 |
| Runtime without optimize (ms) | 56 |
| Runtime with first optimizes (ms) | 43 |

Let's try to speed up the following function.

### Optimization №2

As you can see from the analysis above, the next longest execution time function is hash_table_is_contain_element. This function contains a call to a hash function and a loop that loops through the list and looks for a search word. Note that the loop contains a call to the strcmp function, which checks the strings for equivalence. Let's be honest: this function is already sped up (it takes over 800 lines in assembler!). But if the word length is fixed and equal to a power of two, we can use the SSE registers. So it was decided to rewrite the entire search loop (including the strcmp that is used in the search loop) by assembler insertion using Intel syntax.

In the picture below, you can see an assembler insertion that replaces the search loop and strcmp. The number of lines written in assembler is 24.

<img src="https://github.com/x-ENIAC/MIPT_projects_2_sem/blob/master/Hash%20table/Pictures_for_readme/asm_second_optimization.jpg" alt="drawing6" width="600"/>

### Second comparison

Now let's compare programs with one and two optimizations. The second optimization has accelerated the work of the program by 1.6%. This picture and table illustrate the results of this acceleration.

Comparison of times before and after the first and both accelerations:

| Values | -O1 |
|----------------|----------------|
| coeff_boost | 1.02 |
| coeff_total_accelerate | 42 |
| Runtime without optimize (ms) | 43 |
| Runtime with first optimizes (ms) | 37 |

Comparison of times after the first and both accelerations (-O1):

<img src="https://github.com/x-ENIAC/MIPT_projects_2_sem/blob/master/Hash%20table/Pictures_for_readme/log_callgrind_3.jpg" alt="drawing7" width="900"/>

### Final comparison

Now let's compare the initial and final (with two accelerations) versions.

Comparison of times before and after accelerations:

| Values | -O1 |
|----------------|----------------|
| coeff_boost | 1.36 |
| coeff_total_accelerate | 36 |
| Runtime without optimize (ms) | 56 |
| Runtime with first optimizes (ms) | 37 |

Comparison of times before and after both accelerations (-O1):

<img src="https://github.com/x-ENIAC/MIPT_projects_2_sem/blob/master/Hash%20table/Pictures_for_readme/log_callgrind_4.jpg" alt="drawing8" width="900"/>


## Results

We got the following result: with the O1 flag the program accelerated by  19.9\%.

Results of this investigation can be divided into three parts:

* Boosting with assembly lines.
* Boosting calculating the hash using intrinsics.
* Boosting with using different flags.

That shows different ways of decreasing the time program works.

# References
* Intel website with intrinsics: https://software.intel.com/sites/landingpage/IntrinsicsGuide
* Documentation for using extended assembly: https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.htm
