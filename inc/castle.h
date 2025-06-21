#ifndef __CASTLE_H
#define __CASTLE_H

#include "knight.h"
#include <vector>

using namespace std;

/**
 * @brief Class to represent a castle
 */
class Castle {
private:
  Knight king;                 // The owner of the castle
  double fitness;              // Fitness value of the castle
  unsigned int war_exhaustion; // War exhaustion level

  /** @brief Sigmoid funcion
   *
   * @param x The input value
   * @return The sigmoid value of x
   */
  double sigmoid(double x) const { return 1.0 / (1.0 + exp(-x)); }

public:
  /** @brief Constructor to initialize the castle with a knight
   *
   * @param knight The knight that owns the castle
   * @param fitness The fitness value of the knight
   * @param war_exhaustion The war exhaustion level of the castle
   */
  Castle(const Knight &knight, const double fitness,
         const unsigned int war_exhaustion = 0)
      : king(knight), fitness(fitness), war_exhaustion(war_exhaustion) {}

  /** @brief Copy Constructor
   *
   * @param other The castle to copy from
   */
  Castle(const Castle &other)
      : king(other.king), fitness(other.fitness),
        war_exhaustion(other.war_exhaustion) {}

  /** @brief Assignment operator
   *
   * @param other The castle to assign from
   * @return Reference to this castle
   */
  Castle &operator=(const Castle &other) {
    if (this != &other) {
      king = other.king;
      fitness = other.fitness;
      war_exhaustion = other.war_exhaustion;
    }
    return *this;
  }

  /** @brief Equality operator
   *
   * @param other The castle to compare with
   * @return True if both castles are equal, false otherwise
   */
  bool operator==(const Castle &other) const {
    return (king == other.king && fitness == other.fitness &&
            war_exhaustion == other.war_exhaustion);
  }

  /** @brief Less than operator
   *
   * @param other The castle to compare with
   * @return True if this castle is less than the other, false otherwise
   */
  bool operator<(const Castle &other) const {
    return (fitness < other.fitness || (fitness == other.fitness &&
                                        war_exhaustion < other.war_exhaustion));
  }

  /** @brief Get the king of the castle
   *
   * @return The knight that owns the castle
   */
  const Knight &get_king() const { return king; }

  /** @brief Get the fitness value of the castle
   *
   * @return The fitness value of the castle
   */
  double get_fitness() const { return fitness; }

  /** @brief Crossover operation
   *
   * @param other The castle to crossover with
   * @return A new knight resulting from the crossover of the two kings
   */
  Knight crossover(const Castle &other) const { return king.cross(other.king); }

  /** @brief Siege operation
   *
   * @param knight The knight that is attacking the castle
   * @param knight_fitness The fitness value of the knight
   * @return True if the siege is successful, false otherwise
   */
  bool siege(const Knight &knight, const double knight_fitness);

  /** @brief Check if can make an alliance with another castle. An alliance can
   * be made if both castles have an similar king.
   *
   * @param other The castle to check alliance with
   * @param epsilon The tolerance for comparing the kings' chromosomes
   * @return True if an alliance can be made, false otherwise
   */
  bool can_ally(const Castle &other,
                double epsilon = numeric_limits<double>::epsilon()) const;

  /** @brief Make alliance with another castle.
   *
   * @param other The castle to ally with
   * @return the castle that results from the alliance
   */
  Castle alliance(const Castle &other) const;
};

#endif // __CASTLE_H
