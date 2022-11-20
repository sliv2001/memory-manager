# Software requirement specification for memory manager

## 1. Introduction

### 1.1 Purpose
This document covers the requirements for a Nucleus-like memory manager. The document is based on IEEE Std 830-1993.

### 1.2 Scope
The document gives the specification of software requirements for the Nucleus-like memory manager. This software is a shell over the system manager; it provides basic memory manager functions (allocating and freeing) and additional functions: reading and writing, with a number of protective features.
 
The main goal of this product is to implement a memory access interface that is protected against most of the defects that lead to "Segmentation Fault" errors.

### 1.3 Definitions, acronyms, and abbreviations
1. Memory Manager (MM) - software product designed for basic memory operations
2. Memory Unit - any random-sized memory element structured according to the given requirements
3. External System - any software product that uses this MM as an integral part

### 1.4 References
[1] IEEE Std 830-1998, IEEE Recommended Practice for Software Requirements Specifications

[2] ISO/IEC 9899:2011, Programming languages — С

## 2. Overall description

### 2.1 Product perspective
This memory manager can only be used as a component of a larger software product, providing memory access, for which it provides an interface of `n_allocate`, `n_free`, `n_read`, `n_write` functions.

MM access to memory is provided by the C language functions `malloc`, `calloc`, `free`.
The list of supported devices includes all devices where dynamic memory accesses are implemented with the help of `malloc` and `free` functions. Besides, devices must have libc library implementation, which is used by current MM. 

### 2.2 Product functions
The functionality of the product includes:

* Memory allocation
* Read/Write to memory address with protection against dereferencing of invalid references
* Freeing memory at a pointer address

### 2.3 Constraints
The following constraints must be met when implementing the project:
- The memory available for allocation is limited to the capacity of the system and the machine running the product
- The memory allocated by the MM is directly accessible, i.e. the MM operates with real memory addresses without making any mappings

## 3. Specific requirements

### 3.1 Functions
1.	The product should implement the `n_alloc` function, which allocates a block of memory of a given size or generates an error if the allocation is not possible.
2.	The product should implement `n_read` function which performs reading of a specified number of bytes at a given address; also, there should be implemented `n_write` function which similarly performs reading.
3.	The product should implement the function `n_free`, which frees the required block of memory if it has been allocated.
4.	The product should have error handling: all product functions are of int type and output 0 if the operation is successful, or an error code otherwise.
5.	Error handling includes the following defects: 
    1. Dereferencing and comparison of invalid references
    2. Read/write outside the object boundaries
	
### 3.2 Non-Functional requirements
1. The product must be usable on any external system which supports dynamic memory access and has an implementation of the libc library.
2.	The product must be implemented as a set of C source and header files.
3.	Each block of memory allocated by the `n_alloc` function must consist of a header that contains the length of the allocated block minus the length of the header and the block available for write/read.