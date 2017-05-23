#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "maze.hh"


// Helper function to add a direction to v if the cell in that direction hasn't been visited
void addDirectionOption(const Maze &maze, const Location &current,
                        Direction dir, vector<Direction> &v){
  Location nextCell = maze.getNeighborCell(current.row, current.col, dir);
  if (!maze.isVisited(nextCell.row, nextCell.col)){
    v.push_back(dir);
  }
}




// Main function. It takes 2 arguments (plus program name so argc = 3)
// The argmuments are in usage statement below
// The program generates a maze of the specified size with the start in the
// top-left corner and the end in the bottom-right corner
int main(int argc, char *argv[]){
  
  // Initialize random number generator
  srand(time(NULL));

  // Parse arguments
  if (argc != 2+1){
    std::cout << "usage: ./genmaze numRows numCols" <<
        std::endl << "numRows is the number of rows in the maze" <<
        std::endl << "numCols is the number of columns in the maze" << std::endl;
    return 1;
  }
  int numRows = atoi(argv[1]);
  int numCols = atoi(argv[2]);

  // Create maze
  Maze maze = Maze(numRows, numCols);
  
  // Create path vector for generating maze layout
  vector<Location> path = vector<Location>();

  // Initialize maze
  maze.clear();
  maze.setAllWalls();
  maze.setStart(0, 0); // Set start top-left cell
  maze.setEnd(numRows-1, numCols-1); // Set end bottom-right cell
  maze.setVisited(maze.getStart().row, maze.getStart().col);

  // Set start of path to start.
  path.push_back(maze.getStart());
  // Generate maze by walking along paths until there are no paths with new directions to go.
  while (!path.empty()){
    // Push the current location
    Location current = path.back();
    
    // If we reached end, back up and keep going in other directions
    if (current == maze.getEnd()){
      path.pop_back();
      continue;
    }

    // Generate a list of all the directions we could go
    vector<Direction> options = vector<Direction>();
    

    if (current.row > 0){  // I can move up a row
      addDirectionOption(maze, current, Direction::NORTH, options);
    }
    if (current.row < maze.getNumRows() - 1){ // I can move down a row
      addDirectionOption(maze, current, Direction::SOUTH, options);
    }
    if (current.col > 0){  // I can move left a column
      addDirectionOption(maze, current, Direction::WEST, options);
    }
    if (current.col < maze.getNumCols() - 1){ // I can move right a column
      addDirectionOption(maze, current, Direction::EAST, options);
    }
    
    // If no directions to go, back up on path
    if (options.empty()){
      path.pop_back();
      continue;
    }
    
    // Select random direction, and clear wall in that direction and add that move to path.
    int index = rand() % options.size();
    Direction dir = options[index];
    maze.clearWall(current.row, current.col, dir);
    Location next = maze.getNeighborCell(current.row, current.col, dir);
    maze.setVisited(next.row, next.col);
    path.push_back(next);
    
  }
  // print the maze
  maze.print(std::cout);
    
}
