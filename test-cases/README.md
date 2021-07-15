# README

## Purpose
Automate the geneartion of test cases for the X86 instruction emulator.

## Compilation

### Install boost library
    sudo apt-get install libboost-all-dev
### Compile
    g++ -std=gnu++17 -o generate-test-input.out generate-test-input.cpp

## Usage

1. Create a new directory inside test-cases directory. 
2. Place the C program file to be used for test case generation inside the above directory.
3. Run the following command from inside the test-cases directory.
    
        ./generate-test-input.out <c-file-path> <section-name>

    * `c-file-path` : absolute or relative (to test-cases directory) path of the C program file used to generate the test input.
    * `section-name` : section from the disassmbly to be used for generating test input. 
        eg:
        main

## Outputs

* `<c-file-name>.out` : 32-bit instruction format compiled output of given C program file. (*gitignored* for convenience).
* `<c-file-name>.disassembly` : disassembly of `<c-file-name>.out`.
* `section-<section-name>` : extrated part from `<c-file-name>.disassembly` corresponding to the section.
* `input-<section-name>` : hex instructions corresponding to the section.
