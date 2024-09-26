# Re-implementation of the Needleman & Wunsch algorithm
April 2023 (L3 BI)

## Introduction

This project aims to re-implement the Needleman & Wunsch global alignment algorithm using C.
Substitution scores are taken from the [BLOSUM62](https://www.ncbi.nlm.nih.gov/Class/FieldGuide/BLOSUM62.txt) matrix.

## Setup

To install the algorithm and its dependencies, you need to perform the following steps:

### Clone the repository

```bash
git clone https://github.com/gloriabenoit/Needleman-Wunsch.git

cd Needleman-Wunsch
```

## Usage (command line interface)

```bash
gcc -Wall GloriaBENOIT_ProjetAlignement.c -o GB_Proj

./GB_Proj fasta_file_1 fasta_file_2
```
