#include <iostream>
#include "maze.hh"
#include <assert.h>

using namespace std;




    // Initialize a new maze of size rows x cols
Maze::Maze(int rows, int cols){
    //Check maze size is valid
    assert(rows >= 1);
    assert(cols >= 1);
    //Initialize the maze variables
    numRows = rows;
    numCols = cols;
    cells = new MazeCell[(2*numRows+1)*(2*numCols+1)];
}
    
    // Make a copy of an existing maze object
Maze::Maze(const Maze &m){
    numRows = m.getNumRows();
    numCols = m.getNumCols();
    // Generate new location instances for start and end
    start = Location(m.getStart().row, m.getStart().col);
    end = Location(m.getEnd().row, m.getEnd().col);
    // Copy the cells and walls
    cells = new MazeCell[(2*numRows+1)*(2*numCols+1)];
    for (int r = 0; r < numRows; r++){
        for (int c = 0; c < numCols; c++){
            cells[getArrayIndex(getCellArrayCoord(r,c))] = m.getCell(r, c);
            cells[getArrayIndex(getWallArrayCoord(r,c))] = m.ge
        }
    }

}
    
    // Maze destructor
Maze::~Maze(){
    delete [] cells;
}
    
    // Maze assignment operator
Maze::Maze & operator=(const Maze &m){

}


// Returns the number of rows in the maze
int Maze::getNumRows() const{
    return numRows;
}
    
// Returns the number of columns in the maze
int Maze::getNumCols() const{
    return numCols;
}


// Returns the starting point in the maze
Location Maze::getStart() const{
    return start;
}

// Sets the starting point in the maze    
void Maze::setStart(int row, int col){
    start = Location(row, col);
}
    

// Returns the ending point in the maze    
Location Maze::getEnd() const{
    return end;
}
    
// Sets the ending point in the maze
void Maze::setEnd(int row, int col){
    end = Location(row, col);
}
    

// Sets all cells and walls to be empty, so that the maze is
// completely cleared
void Maze::clear(){
    // Index from end of cells array to beginning, clearing all cells.
    for (int i = (2*numRows+1)*(2*numCols+1) - 1; i >= 0; i--){
        cells[i] = MazeCell::EMPTY;
    }
}
    
// Places a wall at every location that can be a wall in the maze
void Maze::setAllWalls(){
    for (int r = 0; r < numRows - 1; r++){
        for (int c = 0; c < numCols - 1; c++){
            cells[getArrayIndex(getWallArrayCoord(r, c, Direction::SOUTH))] = MazeCell::WALL;
            cells[getArrayIndex(getWallArrayCoord(r, c, Direction::EAST))] = MazeCell::WALL;
        }
    }
}


// Returns the value of the specified cell
MazeCell Maze::getCell(int cellRow, int cellCol) const{
    assert(cellRow >= 0 && cellRow < numRows);
    assert(cellCol >= 0 && cellCol < numCols);
    return cells[getArrayIndex(getCellArrayCoord(cellRow, cellCol))];
}

void Maze::setCell(int cellRow, int cellCol, MazeCell val){
    cells[getArrayIndex(getCellArrayCoord(cellRow, cellCol))] = val;
}

// Returns the cell-coordinates of the neighboring cell in the specified
// direction.  Trips an assertion if the given cell has no neighbor in the
// specified direction (e.g. the NORTH neighbor of cell (0,5)).
Location Maze::getNeighborCell(int cellRow, int cellCol,
                            Direction direction) const
{
    //Check coordinates in maze bounds
    assert(cellRow >= 0 && cellRow < numRows);
    assert(cellCol >= 0 && cellCol < numCols);
    switch direction{
        case Direction::NORTH:
            assert(cellRow >= 1);
            return Location(cellRow - 1, cellCol);
        case Direction::EAST:
            assert(cellCol < numCols - 1);
            return Location(cellRow, cellCol + 1);
        case Direction::SOUTH:
            assert(cellRow < numRows - 1);
            return Location(cellRow + 1, cellCol);
        case Direction::WEST:
            assert(cellCol >= 1);
            return Location(cellRow, cellCol - 1);
        default:
            //Should never reach here
            assert(false);
    }

}


// Returns true if there is a wall in the specified direction from the
// given cell, false otherwise
bool Maze::hasWall(int cellRow, int cellCol, Direction direction) const{
    return cells[getArrayIndex(getWallArrayCoord(cellRow, cellCol, direction))] == MazeCell::WALL;
}

// Puts a wall on the specified side of the given cell
void Maze::setWall(int cellRow, int cellCol, Direction direction){
    cells[getArrayIndex(getWallArrayCoord(cellRow, cellCol, direction))] == MazeCell::WALL;
}

// Removes a wall on the specified side of the given cell
void Maze::clearWall(int cellRow, int cellCol, Direction direction){
    cells[getArrayIndex(getWallArrayCoord(cellRow, cellCol, direction))] == MazeCell::EMPTY;
}


// Returns true if the specified maze cell has been visited.
bool Maze::isVisited(int cellRow, int cellCol) const{
    Cell *cell = cells[getArrayIndex(getWallArrayCoord(cellRow, cellCol, direction))];
    return cell == MazeCell::VISITED;
}

// Changes the cell's value to VISITED
void Maze::setVisited(int cellRow, int cellCol){
    Cell *cell = cells[getArrayIndex(getWallArrayCoord(cellRow, cellCol, direction))];
    cell = MazeCell::VISITED;
}


// Outputs the maze using simple ASCII-art to the specified output stream.
// The output format is as follows, using the example maze from the
// assignment write-up.  (The text to the right of the maze is purely
// explanatory, and you don't need to output it.)
//
// 3 4                 (# of rows and then # of columns)
// +---+---+---+---+   (each cell is 3 spaces wide, with a + at each corner)
// | S     |       |   (walls indicated by --- or |)
// +---+   +   +   +   (start indicated by S, end indicated by E)
// |   |   |   |   |
// +   +   +   +   +
// |           | E |
// +---+---+---+---+
void Maze::print(ostream &os) const{
    // Output number of rows and columns
    os << numRows << " " << numCols << endl;
    // Output the maze
    for(int r = 0; r < numRows; r++){
        //Output top walls
        for (int c = 0; c < numCols; c++){
            os << "+";
            if (hasWall(r, c, Direction::NORTH)){
                os << "---";
            }
            else{
                os << "   ";
            }
        }
        os << "+";
        //Output cells and walls in row
        for (int c = 0; c < numCols; c++){
            //Output left (west) wall
            if (hasWall(r, c, Direction::WEST)){
                os << "|";
            }
            else{
                os << " ";
            }
            //Output cell start or end, else, empty
            if (cell.getStart() == Location(r, c)){
                os << " S ";
            }
            else if (cell.getStart() == Location(r, c)){
                os << " E ";
            }
            else {
                os << "   ";
            }
        }
        if (hasWall(r, numCols-1, Direction::EAST)){
            os << "|";
        }
        else{
            os << " ";
        }
        os << endl;
    }
    //Output bottom wall
    for (int c = 0; c < numCols; c++){
        os << "+";
        if (hasWall(numRows-1, c, Direction::SOUTH)){
            os << "---";
        }
        else{
            os << "   ";
        }
    }
    os << "+";
}

// Take 2D expanded coordinates and compute the corresponding 1D array index
int Maze::getArrayIndex(const Location &loc) const{
    return (numCol*cellRow) + cellCol;
}

// Returns the expanded coordinates of the specified cell coordinates
Location Maze::getCellArrayCoord(int cellRow, int cellCol) const{
    return Location(cellRow*2 +1, cellCol*2+1);
}

// Returns the expanded coordinates of the wall on a specific side of
// a cell given in cell coordinates
Location Maze::getWallArrayCoord(int cellRow, int cellCol,
                            Direction direction) const{
    assert(cellRow >= 0 && cellRow < numRows);
    assert(cellCol >= 0 && cellCol < numCols);
    Location loc = getCellArrayCoord(cellRow, cellCol);
    switch direction{
        case Direction::NORTH:
            loc.row--;
            return loc;
        case Direction::EAST:
            loc.col++;
            return loc;
        case Direction::SOUTH:
            loc.row++;
            return loc;
        case Direction::WEST:
            loc.col--;
            return loc;
        default:
            //Should never reach here
            assert(false);
    }
}
