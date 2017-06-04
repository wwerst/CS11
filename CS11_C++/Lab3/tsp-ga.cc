#include <vector>
#include <set>
#include <assert.h>
#include <algorithm>
#include <iostream>
#include "tsp-ga.hh"
#include "Point.hh"



// Traveling Salesman Problem
// Genome class for genetic algorithm


TSPGenome::TSPGenome(int numPoints){
  order = std::vector<int>();
  for (int i = 0; i < numPoints; i++){
    order.push_back(i);
  }
  random_shuffle(order.begin(), order.end());
}

TSPGenome::TSPGenome(const std::vector<int> &order){
  this->order = order;
}

// Destructor
TSPGenome::~TSPGenome(){
  // NOP
}

// Mutator methods

// Calculate circuit length
void TSPGenome::computeCircuitLength(const std::vector<Point> &points){
  circuitlength = 0;
  for (unsigned int i = 0; i < order.size(); i++){
    Point p1 = points.at(order.at(i));
    Point p2;
    if (i == order.size() - 1){
      p2 = points.at(order.at(0));
    } else {
      p2 = points.at(order.at(i+1));
    }
    circuitlength += p1.distanceTo(p2);    
  }
}


// Randomly swap two different points' order position.
void TSPGenome::mutate(){
  if (order.size() == 1){
    return;
  }
  int index1 = rand() % order.size();
  int index2 = rand() % order.size();
  while (index2 == index1){
    index2 = rand() % order.size();
  }
  int temp = order[index1];
  order[index1] = order[index2];
  order[index2] = temp;
}


// Accessor methods
std::vector<int> TSPGenome::getOrder() const{
  return order;
}
double TSPGenome::getCircuitLength() const {
  return circuitlength;
}


// Randomly crosslink two genomes and return a child genome
TSPGenome crosslink(const TSPGenome &g1, const TSPGenome &g2){
  std::vector<int> g1_order = g1.getOrder();
  std::vector<int> g2_order = g2.getOrder();
  assert (g1_order.size() == g2_order.size());
  unsigned int size = g1_order.size();
  int seq1_index = rand() % g1_order.size();
  std::vector<int> child_order = std::vector<int>();
  std::set<int> added_vals  = std::set<int>();
  // Take a random number of points from g1
  for (int g1_index = 0; g1_index < seq1_index; g1_index++){
    child_order.push_back(g1_order[g1_index]);
    added_vals.insert(g1_order[g1_index]);
  }
  // Take the remaining points from g2
  for (unsigned int g2_index = 0; g2_index < g2_order.size(); g2_index++){
    int g2_val = g2_order[g2_index];
    if (added_vals.count(g2_val) == 0){
      child_order.push_back(g2_val);
      added_vals.insert(g2_val);
    }
  }
  assert(size == child_order.size());
  return TSPGenome(child_order);
}

// Returns true if g1 is shorter path than g2
bool isShorterPath(const TSPGenome &g1, const TSPGenome &g2){
  return g1.getCircuitLength() < g2.getCircuitLength();
}

TSPGenome findAShortPath(const std::vector<Point> &points,
                            int populationSize,
                            int numGenerations,
                int keepPopulation,
                int numMutations){
  std::vector<TSPGenome> population = std::vector<TSPGenome>();
  for (int i = 0; i < populationSize; i++){
    population.push_back(TSPGenome(points.size()));
  }
  for (int gen = 0; gen <= numGenerations; gen++){
    // Recalculate circuit lengths of all in population
    for (int pop = 0; pop < populationSize; pop++){
      population[pop].computeCircuitLength(points);
    }
    // Sort population
    std::sort(population.begin(), population.end(), isShorterPath);
    // Print out the status
    if (gen % 10 == 0){
      std::cout << std::endl;
      std::cout << "Generation " << gen << ":  shortest path is "
           << population[0].getCircuitLength() << std::endl;
    }
    for (int i = keepPopulation; i < populationSize; i++){
      int parent1 = rand() % keepPopulation;
      int parent2 = rand() % keepPopulation;
      while (parent1 == parent2){
        parent2 = rand() % keepPopulation;
      }
      population[i] = crosslink(population[parent1],population[parent2]);
    }
    // Mutate all but population[0]
    for (int num_mut = 0; num_mut < numMutations; num_mut++){
      int random = rand() % populationSize;
      while (random == 0){
        random = rand() % populationSize;
      }
      population[random].mutate();
    }
    
  }
  for (int pop = 0; pop < populationSize; pop++){
    population[pop].computeCircuitLength(points);
  }
  return population[0];
}

