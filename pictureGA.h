//! a class for the GA engine

/*! ****************************************************************
  file: pictureGA.h
  
  This class defines the operations that are built into this GA
  object. Included are a public one to go(), and a bunch of private
  member functions to manipulate the generated individuals.
 **************************************************************** */
#include "individual.h"

class 
pictureGA {

 public:

  /*! \brief default constructor

    ask the user for the file that holds the target picture, open the
    file, read in the dimensions allocate space for the file, load
    it, and finally close the file. */
  pictureGA( int );

  /*! \brief Deallocate the targetPicture array and the generation
      that was created.*/
  ~pictureGA();

  /*! \brief The main function to get the GA to start 

    This function is the start of the GA engine. Note that the
    function to read in the picture is in a separate function called
    by the constructor.*/
  void go();
  
  /* Set the number of crossover points in the xPoints memebr variable*/
  void setXPoints( int num );


 private:

  /*! \brief Function to input the target picture that the GA is
      aiming to match. This function is called in the constructor.
    
      precondition: none

      postcondition: The picture will be loaded into the 2D array
      targetPicture, and the variables nRows and nCols will be set to
      the appropriate values.
   */
  void input_target_picture();

  /*! \brief Calculate the fitnesses of all the individuals of the
      generation.
    
      precondition: none

      postcondition: The average fitness of the individuals is returned.
   */
  double calculate_fitnesses();

  /*! \brief After ranking the population, create a roulette-style
      selection process to find the two best parents.
    
      precondition: none

      postcondition: none
   */
  void roulettify();

  /*! \brief After the next generation has been created, irridiate the
      individuals except the parents.
    
      precondition: none

      postcondition: All the individuals will be subject to mutation
      based on the input mutation rate.
   */
  void irradiate();

  /*! \brief After finding the two best parents, erase all others and
      populate the rest with their offspring.
    
      precondition: none

      postcondition: this function will use the "elite" strategy
   */
  void spawn();

  /* This prints the entire generation to console, used for testing only */
  void print_generation();

  /* Select the best two parents at each generation instead of rouletifying*/
  void bestTwo();

  unsigned int parent1;  //!< The first of the two best parents in roulettify 
  unsigned int parent2;  //!< The second of the two best parents in roulettify

  /*! \brief These variables of the target picture */
  int** targetPicture;
  unsigned int nRows;  //!< number of rows in the target picture
  unsigned int nCols;  //!< number of columns in the target picture, assuming rectangluar picture

  individual* generation; //!< The generation of individuals that is dynamically allocated.

  unsigned int numIndividuals;     //!< The number of individuals. 
  
  double mutationRate;    //!< We are keeping track of the mutation rate that is input from the user.
  int xPoints;  //!< We are keeping track of the number of crossover points that is input from the user.
};
