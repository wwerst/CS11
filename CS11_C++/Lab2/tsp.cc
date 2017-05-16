#include <cmath>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Point.hh"
using namespace std;

// Function to find minimum length that covers all points
double circuitLength(const vector<Point> &points, const vector<int> &order);
vector<int> findShortestPath(const vector<Point> &points);


int main(){
  // Read points
  int numpoints;
  cout << "How Many Points?  ";
  cin >> numpoints;
  double x, y, z;
  vector<Point> points = vector<Point>();
  for (int i = 0; i < numpoints; i++){
    printf("Point %d: ", i);
    cin >> x >> y >> z;
    points.push_back(Point(x,y,z));
  }
  // Find minimum length
  vector<int> shortestpath = findShortestPath(points);
  printf("shortest distance:  %.4f \n",circuitLength(points, shortestpath));
}

vector<int> findShortestPath(const vector<Point> &points){
  vector<int> order = vector<int>();
  for (unsigned int i = 0; i < points.size(); i++){
    order.push_back(i);
  }
  vector<int> shortestOrder = vector<int>(order);
  double shortestLength = circuitLength(points, order);
  while (next_permutation(order.begin(), order.end())){
    double length = circuitLength(points, order);
    if (length < shortestLength){
      shortestOrder = vector<int>(order);
      shortestLength = length;
    }
  }
  return shortestOrder;
}
  

double circuitLength(const vector<Point> &points, const vector<int> &order){
  double length = 0;
  for (unsigned int i = 0; i < order.size(); i++){
    Point p1 = points.at(order.at(i));
    Point p2;
    if (i == order.size() - 1){
      p2 = points.at(order.at(0));
    } else {
      p2 = points.at(order.at(i+1));
    }
    length += p1.distanceTo(p2);    
  }
  return length;

}
