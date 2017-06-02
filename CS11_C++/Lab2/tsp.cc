#include <cmath>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Point.hh"
#include <string>
using namespace std;




// Function to find length of path to traverse a set of points in a given order
double circuitLength(const vector<Point> &points, const vector<int> &order){

  // Initialize variable to store total length
  double length = 0;
  for (unsigned int i = 0; i < order.size(); i++){
    
    // Get length from p1 to p2, and add it to length
    Point p1 = points.at(order.at(i));
    Point p2;
    // Check if p1 is last point, if so p2 is the first point in list
    if (i == order.size() - 1){ 
      p2 = points.at(order.at(0));
    } else {
      p2 = points.at(order.at(i+1));
    }
    length += p1.distanceTo(p2);    
  }
  return length;

}

// Function to find minimum length that covers all points
vector<int> findShortestPath(const vector<Point> &points){

  // Initialize vector for order of points
  vector<int> order = vector<int>();
  for (unsigned int i = 0; i < points.size(); i++){
    order.push_back(i);
  }
  
  // initialize shortestOrder to original order
  vector<int> shortestOrder = vector<int>(order);
  double shortestLength = circuitLength(points, order);
  // Go through all permutations of order and find shortest path
  while (next_permutation(order.begin(), order.end())){
    double length = circuitLength(points, order);
    if (length < shortestLength){
      shortestOrder = vector<int>(order);
      shortestLength = length;
    }
  }
  return shortestOrder;
}
  


// Main function for the program
int main(){

  // Read points
  int numpoints;
  
  // Get the number of points 
  cout << "How Many Points?  ";
  cin >> numpoints;
  
  // Get the x, y, z coordinates for the points
  double x, y, z;
  vector<Point> points = vector<Point>();
  for (int i = 0; i < numpoints; i++){
    printf("Point %d: ", i);
    cin >> x >> y >> z;
    
    // Add Point to list of points 
    points.push_back(Point(x,y,z));
  }
  
  // Find minimum length path
  vector<int> shortestpath = findShortestPath(points);
  
  // Concatenate the shortestpath vector into a string to display
  string path_str = "[";
  for (int i = 0; i < (int)shortestpath.size(); i++){
    //cout << path_str << endl;
    path_str = path_str + to_string(shortestpath[i]) + " ";
  }
  // Change the last character, a space, into a ]
  path_str[path_str.length()-1] = ']';
  
  // Display the best order and the shortest distance
  printf("Best order:  %s\n", path_str.c_str());
  printf("shortest distance:  %.4f \n",circuitLength(points, shortestpath));
}




