#include <cmath>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "Point.hh"
#include "tsp-ga.hh"

// Main function. It takes 4 arguments (plus program name so argc = 5)
// The argmuments are in usage statement below
// The program find a short distance between a listed set of points.
int main(int argc, char *argv[]){

  // Parse arguments
  if (argc != 4+1){
    std::cout << "Usage: ./tsp-ga population generations keep_prop mutate" <<
        std::endl << "population - Positive integer specifying population size" <<
        std::endl << "generations - positive integer specifying how many generations"
        << std::endl << "keep_prop - floating-point value in range 0-1" <<
        std::endl << "mutate - nonnegative floating-point for average number of 	   mutations";
    return 1;
  }
  int population = atoi(argv[1]);
  int generations = atoi(argv[2]);
  int keep_num = (int)(atof(argv[3])*population);
  double mutate = (int)(atof(argv[4])*population);
  // Read points
  int numpoints;
  std::cout << "How Many Points?  ";
  std::cin >> numpoints;
  double x, y, z;
  std::vector<Point> points = std::vector<Point>();
  for (int i = 0; i < numpoints; i++){
    printf("Point %d: ", i);
    std::cin >> x >> y >> z;
    points.push_back(Point(x,y,z));
  }
  // Find a short length
  TSPGenome shortpathgenome = findAShortPath(points, population,
					   	generations,
						keep_num,
						mutate);
  // Print the distance
  
  std::vector<int> shortpathorder = shortpathgenome.getOrder();
  // Concatenate the shortpathorder vector into a string to display
  std::string path_str = "[";
  for (int i = 0; i < (int)shortpathorder.size(); i++){
    //cout << path_str << endl;
    path_str = path_str + std::to_string(shortpathorder[i]) + " ";
  }
  // Change the last character, a space, into a ]
  path_str[path_str.length()-1] = ']';
  
  // Display the best order and the shortest distance
  printf("Best order:  %s\n", path_str.c_str());
  
  double shortpathdist = shortpathgenome.getCircuitLength();
  printf("shortest distance:  %.4f \n",shortpathdist);
}
