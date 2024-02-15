/***************************************************************************
 * FILE: ser_pi_calc.c
 * DESCRIPTION:  
 *   Serial pi Calculation - C Version
 *   This program calculates pi using a "dartboard" algorithm.  See
 *   Fox et al.(1988) Solving Problems on Concurrent Processors, vol.1
 *   page 207.  
 * AUTHOR: unknown
 * REVISED: 02/23/12 Blaise Barney
***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>  
#include <stdexcept>
#include <string>

#include "mpi.h"

using namespace std;

void srandom (unsigned seed);  
double dboard (int darts);

#define DARTS 10000   	/* number of throws at dartboard */
#define ROUNDS 100    	/* number of times "darts" is iterated */

//pass darts and rounds from command line
int main(int argc, char *argv[])
{
double pi;          	/* average of pi after "darts" is thrown */
double avepi;       	/* average pi value for all iterations */
int i, n, darts, rounds;
int rank,size, my_rounds;
double start_time, end_time, elapsed_time, max_elapsed_time, all_avepi;

MPI_Init(&argc, &argv);

MPI_Comm_rank(MPI_COMM_WORLD, &rank);

MPI_Comm_size(MPI_COMM_WORLD, &size);


start_time=MPI_Wtime();



// Read in number of darts and rounds from the cli with error checking
string arg = argv[1];
try {
  size_t pos;
  darts = stoi(arg, &pos);
  if (pos < arg.size()) {
    cerr << "Trailing characters after number: " << arg << '\n';
  }
} catch (invalid_argument const &ex) {
  cerr << "Invalid number: " << arg << '\n';
} catch (out_of_range const &ex) {
  cerr << "Number out of range: " << arg << '\n';
}

arg = argv[2];
try {
  size_t pos;
  rounds = stoi(arg, &pos);
  if (pos < arg.size()) {
    cerr << "Trailing characters after number: " << arg << '\n';
  }
} catch (invalid_argument const &ex) {
  cerr << "Invalid number: " << arg << '\n';
} catch (out_of_range const &ex) {
  cerr << "Number out of range: " << arg << '\n';
}

my_rounds = int(rounds / size); 

if (rounds%size != 0 && rank == 0) {
   my_rounds += rounds%size;
}

//printf("Rank : %3d , myrounds = %3d , start = %10.8f\n", rank, my_rounds, start_time);



//printf("Starting serial version of pi calculation using dartboard algorithm...\n");
//srandom (5);            /* seed the random number generator */
avepi = 0;
for (i = 0; i < my_rounds; i++) {
   /* Perform pi calculation on serial processor */
   pi = dboard(darts);
   avepi = ((avepi * i) + pi)/(i + 1); 
   /*   
   printf("   After %3d throws, average value of pi = %10.8f\n",
         (DARTS * (i + 1)),avepi);    
   */
}

//printf("\nReal value of PI: 3.1415926535897 \n");

//average all pi calculations
MPI_Reduce(&avepi, &all_avepi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);



end_time = MPI_Wtime();

elapsed_time = end_time-start_time;


//find the longest time on all ranks the code took to run
MPI_Reduce(&elapsed_time, &max_elapsed_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

if (rank == 0) {
   printf("Elapsed wall time: %f seconds to calculate PI = %f \n", max_elapsed_time, all_avepi/size); 
   // Output results to a CSV file with header
    ofstream outFile("results.csv", ios::app);  // Open file in append mode
    if (outFile.is_open()) {
        // Add header if the file is empty
        if (outFile.tellp() == 0) {
            outFile << "cores, wall_time ,pi, darts, rounds" << endl;
        }
        outFile << size << "," << max_elapsed_time << "," << all_avepi/size << "," << darts << "," << rounds << endl;
        outFile.close();
        cout << "Results appended to results.csv" << endl;
    } else {
        cerr << "Unable to open the output file." << endl;
        return 1;
    }

}



MPI_Finalize();


}


/*****************************************************************************
 * dboard
 *****************************************************************************/
#define sqr(x)	((x)*(x))
long random(void);

double dboard(int darts)
{
   double x_coord,       /* x coordinate, between -1 and 1  */
          y_coord,       /* y coordinate, between -1 and 1  */
          pi,            /* pi  */
          r;             /* random number scaled between 0 and 1  */
   int score,            /* number of darts that hit circle */
       n;
   unsigned int cconst;  /* must be 4-bytes in size */
/*************************************************************************
 * The cconst variable must be 4 bytes. We check this and bail if it is
 * not the right size
 ************************************************************************/
   if (sizeof(cconst) != 4) {
      printf("Wrong data size for cconst variable in dboard routine!\n");
      printf("See comments in source file. Quitting.\n");
      exit(1);
      }
   /* 2 bit shifted to MAX_RAND later used to scale random number between 0 and 1 */
   cconst = 2 << (31 - 1);
   score = 0;

/***********************************************************************
 * Throw darts at board.  Done by generating random numbers
 * between 0 and 1 and converting them to values for x and y
 * coordinates and then testing to see if they "land" in
 * the circle."  If so, score is incremented.  After throwing the
 * specified number of darts, pi is calculated.  The computed value
 * of pi is returned as the value of this function, dboard.
 ************************************************************************/

   for (n = 1; n <= darts; n++) {
      /* generate random numbers for x and y coordinates */
      r = (double)random()/cconst;
      x_coord = (2.0 * r) - 1.0;
      r = (double)random()/cconst;
      y_coord = (2.0 * r) - 1.0;

      /* if dart lands in circle, increment score */
      if ((sqr(x_coord) + sqr(y_coord)) <= 1.0)
         score++;
      }

   /* calculate pi */
   pi = 4.0 * (double)score/(double)darts;
   return(pi);
} 
