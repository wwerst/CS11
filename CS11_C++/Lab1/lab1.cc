#include <cmath>
#include <stdio.h>
#include <iostream>
#include "Point.hh"

// Function to compute area of triangle formed by 3 points
double computeArea(Point &a, Point &b, Point &c);

using namespace std;
int main(){
  // Read point 1
  double x, y, z;
  Point p1, p2, p3;
  cout << "Point 1: ";
  cin >> x >> y >> z;
  // Read in point 2
  p1 = Point(x,y,z);
  cout << "Point 2: ";
  cin >> x >> y >> z;
  p2 = Point(x,y,z);
  // Read in point 3
  cout << "Point 3: ";
  cin >> x >> y >> z;
  p3 = Point(x,y,z);
  // Print out area of point
  printf("Area is: %4.8f \n",computeArea(p1,p2,p3));

}

double computeArea(Point &a, Point &b, Point &c){
  double s1, s2, s3, s;
  // Use Heron's formula
  s1 = a.distanceTo(b);
  s2 = b.distanceTo(c);
  s3 = c.distanceTo(a);
  s = (s1+s2+s3)/2;
  return sqrt(s*(s-s1)*(s-s2)*(s-s3));
}
