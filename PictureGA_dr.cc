// The driver file.
/********************************************************************
  file: PictureGA_dr.cpp
  
  This file contains the main driver of the program. Right now, it
  only creates an instance of the PictureGA object, calling its
  constructor.

  Names of the pair programming members:
    1) Driver - Ishwar Agarwal 
    2) Navigator - yannick dikouba ngog

  Comments team members want to add about collaboration:
    <put comments here if necessary>

  ******************************************************************** */

#include "pictureGA.h"
#include <iostream>
using namespace std;

int 
main( int argc, char** argv ) {
  int numIndividuals;

  cout << "How many individuals are there in the population?";
  cin >> numIndividuals;
  while (numIndividuals<2){
    cout << "At least two individuals required in the population, enter again ";
    cin >> numIndividuals;
  }

  pictureGA myPicture(numIndividuals);
  int xPoints;
  cout << " How many crossover points: ";
  cin >> xPoints;
  myPicture.setXPoints(xPoints);
  myPicture.go();
  return 0;
   
}
