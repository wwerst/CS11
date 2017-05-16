#ifndef TSP_GA_H
#define TSP_GA_H

#include "Point.hh"

// Traveling Salesman Problem
// Genome class for genetic algorithm
class TSPGenome {

private:
  double circuitlength;
  std::vector<int> order;

public:
  // Constructors
  TSPGenome(int numPoints);
  TSPGenome(const std::vector<int> &order);  // three-argument constructor

  // Destructor
  ~TSPGenome();

  // Mutator methods
  void computeCircuitLength(const std::vector<Point> &points);
  void mutate();

  // Accessor methods
  std::vector<int> getOrder() const;
  double getCircuitLength() const;
};

TSPGenome crosslink(const TSPGenome &g1, const TSPGenome &g2);

bool isShorterPath(const TSPGenome &g1, const TSPGenome &g2);

TSPGenome findAShortPath(const std::vector<Point> &points,
                         	int populationSize,
                         	int numGenerations,
				int keepPopulation,
				int numMutations);
#endif
