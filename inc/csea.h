#ifndef __CSEA_H
#define __CSEA_H

#include "castle.h"
#include "knight.h"
#include <vector>

using namespace std;

/**
 * @brief Struct to pass the arguments to the CSEA function
 */
struct CSEAArgs {
  int population_size;  // Number of castles in the population
  int dimension;        // Dimension of the knight's chromosome
  int max_evaluations;  // Maximum number of evaluations
  double mutation_rate; // Mutation rate for the knights
  double sigma;         // Standard deviation for Gaussian mutation
  double epsilon;       // Threshold for considering two knights equal

  CSEAArgs(int pop_size, int dim, int max_gen = 1000, double mut_rate = 0.005,
           double sig = 100.0, double eps = 1e-6)
      : population_size(pop_size), dimension(dim), max_evaluations(max_gen),
        mutation_rate(mut_rate), sigma(sig), epsilon(eps) {}
};

/**
 * @brief Struct to hold the result of the CSEA
 */
struct CSEAResult {
  Knight best_knight; // The best knight found by the algorithm
  double fitness;     // Fitness value of the best knight
  int generation;     // The generation in which the best knight was found
  int evaluations;    // Number of evaluations performed
};

/**
 * @brief Castle Siege Evolutionary Algorithm (CSEA)
 *
 * @param args The arguments for the CSEA
 * @return Result of running the CSEA
 */
CSEAResult csea(const CSEAArgs &args);

/**
 * @brief Generate first generation of castles
 *
 * @param population_size Number of castles to generate
 * @param dimension Dimension of the knight's chromosome
 * @return Vector of castles representing the first generation
 */
vector<Castle> generate_initial_population(int population_size, int dimension);

/**
 * @brief Generate a new generation of knights from the current population of
 * castles. Cross the castles to get knights and mutate some of them
 *
 * @param population The current population of castles
 * @param args The arguments for the CSEA
 * @return Vector of knights representing the new generation
 */
vector<Knight> generate_new_generation(const vector<Castle> &population,
                                       const CSEAArgs &args);

/**
 * @brief Siege the castles with the new generation of knights.
 *
 * @param population The current population of castles
 * @param knights The new generation of knights
 * @param best Reference to the best knight found so far
 */
void siege_castles(vector<Castle> &population, const vector<Knight> &knights,
                   CSEAResult &best);

/**
 * @brief Form alliances between castles based on their kings
 *
 * @param population The current population of castles
 * @param epsilon The threshold for considering two knights equal
 * @param best Reference to the best knight found so far
 * @return Vector of castles after forming alliances
 */
vector<Castle> form_alliances(const vector<Castle> &population, double epsilon,
                              CSEAResult &best);

/**
 * @brief Complete the population of castles by adding the best knight found if
 * it's not already in the population. Add random castles if population is not
 * full.
 *
 *
 * @param population The current population of castles
 * @param args The arguments for the CSEA
 * @param best Reference to the best knight found so far
 */
void complete_population(vector<Castle> &population, const CSEAArgs &args,
                         CSEAResult &best);

/**
 * @brief Get an upper bound of evaluations in a generation
 *
 * @param args The arguments for the CSEA
 * @return The upper bound of evaluations
 */
int get_evaluations_upper_bound(const CSEAArgs &args);

#endif // __CSEA_H
