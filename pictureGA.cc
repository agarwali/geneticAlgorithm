#include "pictureGA.h"
#include <fstream>
#include <string>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <cmath>
using namespace std;

pictureGA::pictureGA( int popSize ) {
  numIndividuals = 0;
  generation = NULL;
  /*! 
    1. get the target picture with the default filename "picture.in"
    2. allocate the initial population and then randomize them.
  */

  /*! Notes:
     1. Need to find the max and min allowed values for the genome
     from somewhere! Should it be from reading the target picture, or
     raw possible greyscale values, assuming a single byte per pixel?
  */
  input_target_picture();

  // time to create the population. Allocate space and randomize
  generation = new individual[popSize];
  for( int i=0; i<popSize; i++ ) {
    generation[i].set_max_min_values(255, 0);
    generation[i].allocate_genome( nRows * nCols );
    generation[i].randomize_genome();
  }
  numIndividuals = popSize;
  // we initialize mutation rate as -1 so that we know it is not set
  // by user yet. We use that to check when we ask the user for mutation
  // rate in the irradiate function.
  mutationRate =-1.0;

}

pictureGA::~pictureGA() {
  if( targetPicture ) {
    for( int i=0; i<nRows; i++ ) 
      if( targetPicture[i] ) delete [] targetPicture[i];
    delete [] targetPicture;
  }
  if( generation ) delete [] generation;
}

void 
pictureGA::go() {
  int numGeneration = 0;
  double fitnessDifference = 0;
  do {
    //print_generation();
    fitnessDifference = calculate_fitnesses();
    cout << "fitness of generation "<< numGeneration <<" is "<< fitnessDifference<<"\n";
    //roulettify();
    bestTwo();
    spawn();
    irradiate();
    numGeneration ++;
  }//while ( numGeneration<20 );
 while ( fitnessDifference < 0.9 );

}

/***********************************************************
 *  PRIVATE MEMBER FUNCTIONS
 ***********************************************************/
void
pictureGA::print_generation(){
    for (int i=0; i<numIndividuals; i++){
        cout << "individual" <<i << " ";
        generation[i].print_individual();
        cout << "\n";
    }


}
void
pictureGA::input_target_picture() {

  ifstream inFile;

  string filename;

  cout << "What is the name of the input file?";
  cin >> filename;
  inFile.open( filename.c_str());
  
  if( inFile.fail() ) {
    cout << "Error opening " << filename << endl;
    exit(1);
  }

  /*! 
    1. Open the file after asking the use250124r for the filename. There are
    several error-checking going on.
    2. Read the size of the image (number of rows and columns ) as
    the first two numbers in this file.
    3. Allocate the rows
    4. For each row, create the columns and then read the values int them
  */
  inFile >> nRows;
  inFile >> nCols;

  targetPicture = new int*[nRows];
  if( !targetPicture ) {
    cout << "Error allocating targetPicture!\n";
    exit(2);
  }

  for( int i=0; i<nRows; i++ ) {
    targetPicture[i] = new int[nCols];
    for( int j=0; j<nCols; j++ )  {
      inFile >> targetPicture[i][j];
    }
  }

  inFile.close();
}

double
pictureGA::calculate_fitnesses() {
  /// YOU MUST IMPLEMENT THIS FUNCTION
  // This function is completed
  // cout << "Calculating total fitness\n";
  double totalFitness = 0.0;
  // traverse through each individual
  for (int i=0; i<numIndividuals; i++){
    // calculate eacj individuals fitness
    generation[i].calculate_fitness(targetPicture, nRows, nCols);
    // add the the calculated fitness to the total fitness
    totalFitness += generation[i].get_my_fitness();
  }
  // return the totalfitness divided by the number of individuals
  return totalFitness/((numIndividuals)*1.0);
}

void
pictureGA::roulettify() {
  
  // cout << "Roulettifying\n";
  double* rouletteValues = new double[numIndividuals];
  double rouletteSoFar(0);
  /// YOU MUST COMPLETE THIS FUNCTION

  // find the total fitness before dividing.
  double totalFitness = 0;
  for( int i =0; i<numIndividuals; i++ ) 
    totalFitness+=generation[i].get_my_fitness();

  for( int i =0; i<numIndividuals; i++ ) 
    rouletteValues[i] = 0.0;

  for( int i =0; i<numIndividuals; i++ ) {
    rouletteSoFar += generation[i].get_my_fitness()/totalFitness;
    rouletteValues[i] = rouletteSoFar;
  }

  // Once the roulette values are in place, find the parent indices
  parent1 = 0;
  parent2 = 0;
  
  // generate a random number between 0 and 1
  double randomNum = rand()*1.0/RAND_MAX;
  // traverse through all the individuals roulettevalues
  // if the roulettevalue of an individual is greater than
  // the random number, we have found our lucky parent1.
  for (int i=0; i<numIndividuals; i++){
    if (rouletteValues[i]>=randomNum){
        parent1 = i;
        parent2 = parent1; // assigned parent 2 to become parent 1, so that it can enter the next loop
        break;
    }
  }
  // repeat the same process above to select parent 2
  // until we find a new parent 2 which is not equal to parent 1.
  while (parent1==parent2){
  randomNum = rand()*1.0/RAND_MAX;
    for (int i=0; i<numIndividuals; i++){
      if (rouletteValues[i]>=randomNum){
        parent2 = i;
        break;
      }
    }
  }
  cout<< "parent1 "<< parent1<<"\n";
  cout<< "parent2 "<< parent2<<"\n";
  


  // To find P1 and P2, we need to generate two random numbers from 0
  // to 100. Because the total of all the candidates' fitness is 100%,
  // picking somewhere in the range would give the parents. A nuance
  // is that the parents have to be different, so the indices must be
  // checked to be different.

  /// YOU MUST COMPLETE THE REST OF THIS FUNCTION
  // We completed this
}

void 
pictureGA::spawn() {
  unsigned int child1(0), child2(1);       // children indices
  unsigned int p1(parent1), p2(parent2);   // parent indices
  unsigned int crossPoint(0);              // initialize a crossoverPoint
  int nGenes = nRows*nCols;                // calculate the number of genes for an individual
  while (child1<numIndividuals && child2<numIndividuals){ 
    // Find the next empty spot for child 1
    while (child1 == child2 || child1 == p1 || child1 == p2){
      child1++;
    }
    // Find the next empty spot for child 2   
    while (child1 == child2 || child2 == p1 || child2 == p2){
      child2++;
    }
    // Double check if the child1 and child2 is in the range of numIndividuals
    if (child1<numIndividuals && child2<numIndividuals) {
    // Travese through all the genes in an individual to create new offsprings
      //////////////////////////////////////////////////  
      //We start generating random crossover points here
      //////////////////////////////////////////////////
      // Intialize an int array with the size cross over points
      // that is already input from user
      int pointArray[xPoints];
        // Go through each position in the crossover points array and generate 
        // a random integer within the range of size of genes
      pointArray[0]= rand() % nGenes;  
      for (int xP=1; xP<xPoints; xP++){
          // generate a random number within the range of nGenes
          pointArray[xP]= rand() % nGenes;
          // create a flag=false to check if the randomly generate int is unique
          bool unique = false;  
          // while we do not have a unique random number, which means that
          // the generated number is equal to atleast one other number in pointArray
          // we will keep generating another random number at the same position in pointArray
          while (!unique){
            int i = xP-1;   // Set a marker at the index just before xP(the current index in the pointArray) 
            // Loop through all other indices before xP till we reach the beginning of the pointArray
            while (i != -1){
              if (pointArray[xP]==pointArray[i]) break;    // break the while loop since the current number is not unique 
              i = i-1;
            }
            if (i == -1) unique = true;    // change flag because the loop did not break above and we know that the number is unique
            if (!unique) pointArray[xP]= rand() % nGenes;   // generate random number again if not unique
          }
        }
      //////////////////////////////////////////  
      // Done generating random crossover points
      //////////////////////////////////////////  
      
      // sort the array using built-in sort algorithm  
      sort(pointArray, pointArray + xPoints);
      ////////////////////////////////
      // Start Crossover for childs //
      ////////////////////////////////

      // crossTime is a flag that tells us if it is time to switch the genes
      double crossTime = false;
      // xP is a marker for the pointArray
      int xP = 0;
      // Loop through all the genes in an individual to perform crossover
      for (int j=0; j<nGenes; j++){
        // check if the current index is equal to one of the crossover points generated above
        if (j == pointArray[xP]){
          crossTime = !crossTime;   // bang crossTime because we found another crossPoint
          xP++;
        } 
        if (!crossTime){
          // copy the genes as it is from the parent if it is not crossover time
          generation[child1].set_value(generation[p1][j], j);
          generation[child2].set_value(generation[p2][j], j);
          } else {
              // copy the genes by switching the parents since it is crossover time
              generation [child1].set_value(generation[p2][j], j);
              generation [child2].set_value(generation[p1][j], j); 
            }
        //////////////////////////////
        // End Crossover for childs //
        //////////////////////////////       
      }
    }
    child1+=2;
    child2+=2;
  }
  // Now we will generate a random crossover points array for parents of the current generation
  // Using the same approach used above for children 
  int pointArray[xPoints];
        // Go through each position in the crossover points array and generate 
        // a random integer within the range of size of genes
      pointArray[0]= rand() % nGenes;  
      for (int xP=1; xP<xPoints; xP++){
          // generate a random number within the range of nGenes
          pointArray[xP]= rand() % nGenes;
          // create a flag=false to check if the randomly generate int is unique
          bool unique = false;  
          // while we do not have a unique random number, which means that
          // the generated number is equal to atleast one other number in pointArray
          // we will keep generating another random number at the same position in pointArray
          while (!unique){
            int i = xP-1;   // Set a marker at the index just before xP(the current index in the pointArray) 
            // Loop through all other indices before xP till we reach the beginning of the pointArray
            while (i != -1){
              if (pointArray[xP]==pointArray[i]) break;    // break the while loop since the current number is not unique 
              i = i-1;
            }
            if (i == -1) unique = true;    // change flag because the loop did not break above and we know that the number is unique
            if (!unique) pointArray[xP]= rand() % nGenes;   // generate random number again if not unique
          }
        }
      //////////////////////////////////////////  
      // Done generating random crossover points
      //////////////////////////////////////////  
      
      // sort the array using built-in sort algorithm  
      sort(pointArray, pointArray + xPoints);

  // Now we will do the cross over for the parents
  for (int j=0; j<nGenes; j++){
      ////////////////////////////////
      // Start Crossover for Parents //
      ////////////////////////////////

      // crossTime is a flag that tells us if it is time to switch the genes
      double crossTime = false;
      // xP is a marker for the pointArray
      int xP = 0;
      // Loop through all the genes in an individual to perform crossover
      for (int j=0; j<nGenes; j++){
        // check if the current index is equal to one of the crossover points generated above
        if (j == pointArray[xP]){
          crossTime = !crossTime;   // bang crossTime because we found another crossPoint
          xP++;
        } 
        if (!crossTime){
          // Do nothing because we keep the genes as it is and not switch 
          
          } else {
              // copy the genes by switching the genes since it is crossover time
              int temp = generation[p1][j]; // first copy parent1's gene in a temp variable
              generation [p1].set_value(generation[p2][j], j); // set parent1's gene to be parent2's
              generation [p2].set_value(temp, j); // set parent2's gene to be temp
            }
        //////////////////////////////
        // End Crossover for Parents //
        //////////////////////////////       
      }
   } 
}

void 
pictureGA::irradiate() {
  //cout<< "Mutating\n";
  /// YOU MUST COMPLETE THIS FUNCTION.
  ///
  /// Whatever mutation policy is up to you, but you must explain what
  /// you did in the comments.
  /// This function is completed but we are not sure if this is the right way to do it
  // cout <<"Mutation Rate init " << mutationRate<< "\n";
  
  // First we check if the mutation rate is set
  if (mutationRate==-1.0){
    cout << "What is the maximum mutation rate?";
    cin >> mutationRate;
    // check if mutation rate is out of range 0 to 1
    // if out of range then ask input again
    while (mutationRate<0 || mutationRate>1){
    cout << "Mutation rate should be between 0 and 1, please enter again ";
    cin >> mutationRate;
    }
  }
  
  // find the total fitness before dividing.
  double totalFitness = 0;
  for( int i =0; i<numIndividuals; i++ ) 
    totalFitness+=(generation[i].get_my_fitness());
  // Calculate the average fitness of the generation
  double avgFitness = totalFitness/numIndividuals;
  // Calculate the total difference of each individual's fitness with the average fitness of the generation
  double fitnessDifference(0);
  for ( int i=0; i<numIndividuals; i++ )
    fitnessDifference += fabs(avgFitness - generation[i].get_my_fitness());
  // Now finally calculate muation rate for this generation by
  // finding the average fitness difference and subtracting it from 1
  // becuase the less the avg fitness difference, we know the generation is converging
  // so we speed up muation
  double thisMutationRate = mutationRate * (1-(fitnessDifference/numIndividuals));  
  // call the mutation function for each individual for the user input mutation rate
  for (int i=0; i<numIndividuals; i++){
    generation[i].mutate(thisMutationRate);
  }
  
}

void
pictureGA::bestTwo() {
  // create a variable 'fitness' with the lowest fitness
  double fitness(0);
  // Loop through all the individuals
  // Whenever there is an individual with a greater fitness than 'fitness' var
  // We set parent to be that individual
  for (int i=0; i<numIndividuals; i++){
    if (generation[i].get_my_fitness()>fitness){
      fitness = generation[i].get_my_fitness();
      parent1 = i;
    }
  }
  // repeat the same process above to get parent2
  // but don't set parent2 to be parent1
  double fitness2(0);
  for (int i=0; i<numIndividuals; i++){
    if (generation[i].get_my_fitness()>fitness2 && i!=parent1){
      fitness2 = generation[i].get_my_fitness();
      parent2 = i;
    }
  }
}

void
pictureGA::setXPoints(int num){
  xPoints = num;
}
