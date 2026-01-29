# File Indexer

This is a simple C++ program that recursively scans a folder and searches for a given word in all text files.

## How to build

make

# How to run

./indexer <folder_path> <word>

# Example

./indexer testdata test

# Expected output

Found in:
- testdata/b.txt
- testdata/subfolder/c.txt

# Project structure

indexer/
    src/
        main.cpp         # Program entry point
        FileScanner.h    # Class declaration for FileScanner
        FileScanner.cpp  # Class implementation for FileScanner
    Makefile             # Build instructions
    README.md            # Project documentation
    testdata/            # Example files for testing