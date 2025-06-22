#ifndef __KNIGHT_H
#define __KNIGHT_H

#include <iostream>
#include <limits>
extern "C" {
#include "../cec17.h"
}
#include "random.hpp"
#include <vector>

using namespace std;

/**
 * @brief Class to represent a knight chromosome
 */
class Knight {
private:
  vector<double> chromosome; // Vector to hold the chromosome values

  /** @brief BLX-alpha crossover operation
   *
   * @param parent1 First parent chromosome
   * @param parent2 Second parent chromosome
   * @return A new chromosome resulting from the crossover
   */
  Knight blx_alpha(const Knight &parent1, const Knight &parent2) const;

  /** @brief Gaussian mutation operation
   *
   * @param mutation_rate The rate of mutation. Probability of each gene being
   * mutated
   * @param radius The range of random values for mutation
   */
  void gaussian_mutation(double mutation_rate = 0.005, double radius = 100.0);

public:
  /** @brief Constructor to initialize the knight with a given dimension
   *
   * @param dimension The size of the chromosome
   * @param randomize If true, initializes the chromosome with random values
   * @param sigma The radius for random initialization
   */
  Knight(int dimension, bool randomize = true, double radius = 100.0);

  /** @brief Copy constructor
   *
   * @param other The knight to copy from
   */
  Knight(const Knight &other) : chromosome(other.chromosome) {}

  /** @brief Assignment operator
   *
   * @param other The knight to assign from
   * @return Reference to this knight
   */
  Knight &operator=(const Knight &other) {
    if (this != &other) {
      chromosome = other.chromosome;
    }
    return *this;
  }

  /** @brief Equality operator
   *
   * @pre The chromosomes must be of the same size
   * @param other The knight to compare with
   * @return True if the chromosomes are equal, false otherwise
   */
  bool operator==(const Knight &other) const {
    for (size_t i = 0; i < chromosome.size(); ++i) {
      if (abs(chromosome[i] - other.chromosome[i]) >
          numeric_limits<double>::epsilon()) {
        {
          return false;
        }
      }
    }
    return true;
  }

  /** @brief Is near operator
   *
   * @pre The chromosomes must be of the same size
   * @param other The knight to compare with
   * @param epsilon The tolerance for comparison
   * @return True if the chromosomes are near equal, false otherwise
   */
  bool is_near(const Knight &other, double epsilon = 1e-6) const {
    for (size_t i = 0; i < chromosome.size(); ++i) {
      if (abs(chromosome[i] - other.chromosome[i]) > epsilon) {
        return false;
      }
    }
    return true;
  }

  /** @brief Unequal operator
   *
   * @param other The knight to compare with
   * @return True if the chromosomes are not equal, false otherwise
   */
  bool operator!=(const Knight &other) const { return !(*this == other); }

  /** @brief Cross operation
   *
   * @param other The knight to cross with
   * @return A new knight resulting from the cross operation
   */
  Knight cross(const Knight &other) const { return blx_alpha(*this, other); }

  /** @brief Mutate the knight's chromosome
   *
   * @param mutation_rate The rate of mutation. Probability of each gene being
   * mutated
   */
  void mutate(double mutation_rate = 0.005) {
    gaussian_mutation(mutation_rate, 100.0);
  }

  /** @brief Return the fitness of the knight
   *
   * @return The fitness value
   */
  double fitness() const {
    double fitness = cec17_fitness(const_cast<double *>(chromosome.data()));

    // Calculate distance (max distance)
    double distance = 0.0;
    for(size_t i = 0; i < chromosome.size(); ++i) {
      distance = max(distance, abs(chromosome[i]));
    }
    if(distance > 100.0) {
      // Print chromosome if distance is greater than 100
      cout << "Chromosome exceeds bounds: ";
      for (const auto &gene : chromosome) {
        cout << gene << " ";
      }
      exit(1);
    }

    return fitness;
  }
};

#endif // __KNIGHT_H
