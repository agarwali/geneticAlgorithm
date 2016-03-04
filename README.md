## Synopsis
A vanilla genetic algorithm with variable mutation rate, and multi-point crossover that generates a "solution" genome close to a target picture.

## Code Example

Below is the list of important files and their purpose:
* individual.h - header file for individual class that defines the individual "solution" genome to the problem
* pictureGA.h - header file for pictureGA class that defines the operations that are built into this GA
  object. 
* PictureGA_dr.cc - This file contains the main driver of the program. 
* picture.in - contains target picture

## Acknowledgements
This was started as a class project for Computational Intelligence class. The main structure of the classes were given by the course instructor Dr.Mario Nakazawa. I implemented the functions of the classes.

## Installation
To run this application you will need

* Xcode 7 or later, on OS X.
* GCC 5 or later, on Linux.

```bash
git clone https://github.com/agarwali/geneticAlgorithm
cd geneticAlgorithm
make
```

To run the GA you nee to run compiled ./ga file. Also you need picture.in, which is a target picture.





