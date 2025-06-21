#ifndef __CSEA_CPP
#define __CSEA_CPP

#include "../inc/castle.h"
#include "../inc/csea.h"
#include "../inc/knight.h"
#include "../inc/random.hpp"
#include <vector>

using namespace std;
using Random = effolkronium::random_static;

CSEAResult csea(const CSEAArgs &args) {
  vector<Castle> population =
      generate_initial_population(args.population_size, args.dimension);
  sort(population.begin(), population.end());
  CSEAResult result = {population[0].get_king(), population[0].get_fitness(), 0,
                       args.population_size};

  // Calculate the upper bound of evaluations in a generation
  int evaluations_upper_bound = get_evaluations_upper_bound(args);

  while (result.evaluations + evaluations_upper_bound < args.max_evaluations) {
    // Generate a set of knight from the crossing of the castles
    vector<Knight> new_knights = generate_new_generation(population, args);

    // For each knight, try to siege the castles
    siege_castles(population, new_knights, result);

    // Form alliances between castles
    population = form_alliances(population, args.epsilon, result);

    // Complete the population with the best knight if not already present
    complete_population(population, args, result);

    result.generation++;
  }

  return result;
}

vector<Castle> generate_initial_population(int population_size, int dimension) {
  vector<Castle> population;
  for (int i = 0; i < population_size; ++i) {
    Knight knight(dimension);
    double fitness = knight.fitness();
    population.emplace_back(knight, fitness);
  }
  return population;
}

vector<Knight> generate_new_generation(const vector<Castle> &population,
                                       const CSEAArgs &args) {
  vector<Knight> new_generation;

  // Crossover the castles to get new knights
  for (int i = 0; i < population.size(); ++i) {
    for (int j = i + 1; j < population.size(); ++j) {
      Knight child = population[i].crossover(population[j]);
      new_generation.push_back(child);
    }
  }

  // Mutate some of the knights
  for (auto &knight : new_generation) {
    knight.mutate(0.005);
  }

  return new_generation;
}

void siege_castles(vector<Castle> &population, const vector<Knight> &knights,
                   CSEAResult &best) {
  for (const auto &knight : knights) {
    double knight_fitness = knight.fitness();
    best.evaluations++;
    // Select a castle to siege
    int castle_index = Random::get<int>(0, population.size() - 1);
    Castle &castle = population[castle_index];
    bool sieged = castle.siege(knight, knight_fitness);

    // If the siege was successful, check if the new king is better than the
    // best found so far
    if (sieged && knight_fitness < best.fitness) {
      best.best_knight = knight;
      best.fitness = knight_fitness;
    }
  }
}

vector<Castle> form_alliances(const vector<Castle> &population, double epsilon,
                              CSEAResult &best) {
  vector<Castle> new_population;
  vector<bool> allied(population.size(), false);
  for (int i = 0; i < population.size(); ++i) {
    if (allied[i])
      continue; // Skip already allied castles

    const Castle &castle1 = population[i];

    for (int j = i + 1; j < population.size(); ++j) {
      if (allied[j])
        continue; // Skip already allied castles

      const Castle &castle2 = population[j];

      if (castle1.can_ally(castle2, epsilon)) {
        // Form an alliance and create a new castle
        Castle new_castle = castle1.alliance(castle2);
        best.evaluations++;
        new_population.push_back(new_castle);
        allied[i] = true;
        allied[j] = true;
        // Update the best knight if the new king is better
        if (new_castle.get_fitness() > best.fitness) {
          best.best_knight = new_castle.get_king();
          best.fitness = new_castle.get_fitness();
        }
      }
    }
    if (!allied[i]) {
      // If no alliance was formed, keep the original castle
      new_population.push_back(castle1);
    }
  }

  return new_population;
}

void complete_population(vector<Castle> &population, const CSEAArgs &args,
                         CSEAResult &best) {
  // Check if the best knight is already in the population
  bool found_best = false;
  for (const auto &castle : population) {
    if (castle.get_king() == best.best_knight) {
      found_best = true;
      break;
    }
  }

  // If not found, add it to the population
  if (!found_best) {
    population.push_back(Castle(best.best_knight, best.fitness));
  }

  // Fill the rest of the population with random castles if needed
  while (population.size() < args.population_size) {
    Knight knight(args.dimension);
    double fitness = knight.fitness();
    best.evaluations++;
    population.emplace_back(knight, fitness);
  }
}

int get_evaluations_upper_bound(const CSEAArgs &args) {
  int evals_from_crossing =
      args.population_size * (args.population_size - 1) / 2;
  int evals_from_alliances = args.population_size / 2;
  int evals_from_new_castles =
      evals_from_alliances; // Each alliance creates another castle
  return evals_from_crossing + evals_from_alliances + evals_from_new_castles;
}

#endif // __CSEA_CPP
