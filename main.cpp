extern "C" {
#include "cec17.h"
}
#include "inc/csea.h"
#include "inc/random.hpp"
#include <iostream>
#include <vector>

using namespace std;
using Random = effolkronium::random_static;

int main() {
  int seed = 0;

  int num_samples = 1;

  // Available dimensions: 10, 30, 50
  vector<int> dims = {10};
  int population_size = 25;
  double mutation_rate = 0.005;
  double sigma = 100.0;
  double epsilon = 1e-6;
  CSEAArgs csea_args(population_size, 0, 0, mutation_rate, sigma, epsilon);

  for (auto dim : dims) {
    csea_args.dimension = dim;
    csea_args.max_evaluations = 10000 * dim;
    cout << "Running CSEA with dimension: " << dim
         << ", max evaluations: " << csea_args.max_evaluations << endl;

    for (int funcid = 1; funcid <= 2; funcid++) {
      for (int i = 0; i < num_samples; ++i) {

        cec17_init("CSEA", funcid, dim);
        // cec17_print_output(); // Comment to print in console, uncomment to
        // print in file

        Random::seed(seed + i);
        auto result = csea(csea_args);

        cout << "Fitness[F" << funcid << "]: " << scientific
             << cec17_error(result.fitness)
             << ", Number generations: " << result.generation << endl;
      }
    }
  }
}
