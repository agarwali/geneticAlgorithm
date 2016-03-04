#include "individual.h"
#include <iostream>
#include <cstdlib> // for "NULL", "exit()"
using namespace std;
individual::individual() { 
  myGenome = NULL; 
  numGenes = 0;
  myFitness = 0.0;
  maxValue = 0;
  minValue = 0;
}

individual::individual( const unsigned int nGenes, 
			const int max, 
			const int min ) {
  allocate_genome( nGenes );
  myFitness = 0.0;
  maxValue = max;
  minValue = min;
}

individual::~individual() {
  if( myGenome ) delete myGenome;
}

int
individual::operator [] ( const unsigned int index ) const { 
  if( !myGenome ) return 0;
  if( index > numGenes ) return 0;
  return myGenome[index]; 
}

double 
individual::get_my_fitness() const {
  return myFitness;
}

void
individual::set_max_min_values( const int max, const int min ) {
  if( max < min ) return;
  maxValue = max;
  minValue = min;
}

void
individual::set_value( const int value, const unsigned int index ) {
  if( index >= numGenes ) return;
  myGenome[index] = value;
}

void 
individual::copy_genome( int* input, const unsigned int nGenes ) {
  if( nGenes != numGenes ) return;
  if( nGenes < 0 ) return;
  for( int i=0; i<nGenes; i++ ) myGenome[i] = input[i];
}

void 
individual::allocate_genome( const unsigned int nGenes ) {
  /// if the genome has already been allocated, skip because we want to
  /// prevent memory leaks.
  if( myGenome ) return;

  /// Allocate space for the genome now
  myGenome = new int[nGenes];
  
  /// if failed, something is REALLY wrong, so bail out of the entire
  /// program.
  if( !myGenome ) exit(-1);

  /// Assign the numGenes used later to nGenes.
  numGenes = nGenes;
}

void 
individual::randomize_genome() {

  /// If there is no genome space allocated, do not do anything. 
  if( !myGenome ) return;

  /// If the max and min values are incorrect, then do nothing.
  if( minValue > maxValue ) return;

  /// find the random number, divide it by RAND_MAX (defined in
  /// cstdlib) to find the range, multiply it by the difference
  /// between max and min values and then add it to minValue to get
  /// the correct scaled range.
  for( int i=0; i< numGenes; i++ ) {
    myGenome[i] = static_cast<int>(((rand()*1.0)/RAND_MAX)*(maxValue - minValue) + minValue);
    
  }
  
}


#include <cmath>
/// YOU MUST COMPLETE THIS FUNCTION
/// precondition: targetPicture is a valid 2D array of integers and nRows and nCols are
///               correct values for the number of rows and columns in the target picture
///               that this genome is compare against.
/// postcondition: this function changes myFitness to reflect the fitness of this 
///                individual as the percent of the gene that MATCHES the target 
///                picture after a gene by gene comparison. 
void 
individual::calculate_fitness( int ** targetPicture, 
			       const unsigned int nRows, 
			       const unsigned int nCols ) {
  /// if the genome is not defined, then skip this step.
  if( !myGenome ) return;
  // If the nRow*nCols is not equal to the size of the genome then return.
  if (nRows*nCols != numGenes) return;
  double difference = 0.0;
  // Traverse all the genes to compare with target picture
  for (int i=0; i<numGenes; i++){
    int row = i/nRows;  // This gets us the index for row number in target picture
    int col = i%nCols;  // This gets us the index for column number in the target picture
    // Add the absolute value of difference between targetPicture gene and individual's gene
    // each time in the loop and add it to the var difference
    difference += abs(targetPicture[row][col] - myGenome[i]);
  }
  if (difference == 0) difference=1;
  // Inverse the difference to get fitness because the lower the difference
  // the more the fitness of that individual
  myFitness = (1.0/difference);
  // cout << "difference is " << difference<< " & fitness is "<< myFitness <<"\n";
}

/// YOU MUST COMPLETE THIS FUNCTION
/// precondition: none, as whether the genome exists or not is checked
///               inside the function.
///
/// postcondition: given mRate probability, an element in the genome will 
///                be changed to a randomly allowed value.
void
individual::mutate( double const mRate ) {

  // Check to make sure that the genome exists before trying to
  // manipulate it.
  if( !myGenome ) return;
  
  // Select the number of genes to be mutated given mRate probability
  int numMutations = (rand() % numGenes) <(mRate*100);

  for (int i=0; i<numMutations; i++){
    // Find the lucky gene to mutated 
    int mutationIndex = rand() % numGenes;
    myGenome[mutationIndex] = static_cast<int>(((rand()*1.0)/RAND_MAX)*(maxValue - minValue) + minValue);
  }
}

void
individual::print_individual(){
  for (int i=0; i<numGenes; i++){
    cout << myGenome[i] << " ";
  }
}
