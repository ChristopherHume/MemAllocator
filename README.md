# MemAllocator
A pseudo memory allocator program written in C to simulate the operating system's allocation of memory

The user gives the program a specific amount of memory that is allowed to be allocated. >>
./allocator (memory amount)

Then the user executes the following commands to alter the memory that is saved.

Allocate memory using First Fit, Best Fit or Worst Fit. >>
RQ (program name) (memory amount) (F/B/W)
  
Release memory for a specific program. >>
  RL (program name)

Display current memory allocation. >>
    STAT
