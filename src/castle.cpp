#ifndef __CASTLE_CPP
#define __CASTLE_CPP

#include "../inc/castle.h"
#include "../inc/random.hpp"

using namespace std;
using Random = effolkronium::random_static;

bool Castle::siege(const Knight &knight, const double knight_fitness) {
  // The knight is stronger, so it can take the castle
  if (knight_fitness < fitness) {
    king = knight;
    fitness = knight_fitness;
    war_exhaustion = 0; // Reset war exhaustion
    return true;
  }

  // The knight is weaker, check if the castle can withstand the siege
  double siege_strength = sigmoid(war_exhaustion) * knight_fitness / fitness;

  double p = Random::get<double>(0.0, 1.0);
  if (p < siege_strength) {
    // The siege is successful, the knight takes the castle
    king = knight;
    fitness = knight_fitness;
    war_exhaustion = 0; // Reset war exhaustion
    return true;
  }

  // The siege fails, increase war exhaustion
  war_exhaustion++;
  return false;
}

bool Castle::can_ally(const Castle &other, const double epsilon) const {
  // Check if the castles can ally based on their kings
  return (king.is_near(other.king, epsilon));
}

Castle Castle::alliance(const Castle &other) const {
  // Create a new castle with the average fitness of both kings
  Knight new_king = king.cross(other.king);
  double new_fitness = new_king.fitness();
  return Castle(new_king, new_fitness,
                min(war_exhaustion, other.war_exhaustion));
}

#endif // __CASTLE_CPP
