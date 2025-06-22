#ifndef __KNIGHT_CPP
#define __KNIGHT_CPP

#include "../inc/knight.h"
#include "../inc/random.hpp"

#include <vector>

using namespace std;
using Random = effolkronium::random_static;

Knight::Knight(int dimension, bool randomize, double radius) {
  chromosome.resize(dimension, 0.0);
  if (randomize) {
    for (int i = 0; i < dimension; ++i) {
      chromosome[i] = Random::get<double>(-radius, radius);
    }
  }
}

Knight Knight::blx_alpha(const Knight &parent1, const Knight &parent2) const {
  int dimension = parent1.chromosome.size();
  Knight child(dimension, false);

  for (int i = 0; i < dimension; ++i) {
    double min_val = min(parent1.chromosome[i], parent2.chromosome[i]);
    double max_val = max(parent1.chromosome[i], parent2.chromosome[i]);
    double range = max_val - min_val;
    double alpha = Random::get<double>(-0.5 * range, 0.5 * range);
    child.chromosome[i] = min_val + alpha;
  }

  return child;
}

void Knight::gaussian_mutation(double mutation_rate, double sigma) {
  int dimension = chromosome.size();

  // Calculate the number of genes to mutate based on the mutation rate
  int fixed_mutations = static_cast<int>(dimension * mutation_rate);
  double optional_mutation = dimension * mutation_rate - fixed_mutations;
  fixed_mutations += Random::get<int>(0, 1) < optional_mutation ? 1 : 0;

  // Select genes to mutate
  normal_distribution<double> distribution(0.0, sigma);
  for (int i = 0; i < fixed_mutations; ++i) {
    int index = Random::get<int>(0, dimension - 1);
    double mutation_value = Random::get(distribution);
    chromosome[index] += mutation_value;

    // Ensure the mutated value is within bounds
    if (chromosome[index] < -100.0) {
      chromosome[index] = -100.0;
    } else if (chromosome[index] > 100.0) {
      chromosome[index] = 100.0;
    }
  }
}

#endif // __KNIGHT_CPP
