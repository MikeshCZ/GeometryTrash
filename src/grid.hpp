#pragma once
#include <algorithm>
#include <raylib.h>
#include <vector>

using namespace std;

class Grid
{
public:
  Grid (int width, int height, int cellSize, Color cellColor)
      : rows (height / cellSize), columns (width / cellSize),
        cellSize (cellSize), cells (rows, vector<int> (columns, 0)),
        edgesWidth (1), cellColor(cellColor) {};
  // Draw complete grid
  void Draw ();
  // Set the value of the cell
  void SetValue (int row, int column, int value);
  // Get the value of the cell
  int GetValue (int row, int column);
  // Return number of rows
  int GetRows ();
  // Return number of columns
  int GetColumns ();
  // Clear the grid
  void Clear ();
  // Is the grid is full of dead cells?
  bool IsClear ();
  // Set the width of the edges of the cells
  void SetEdges (int width);
  // Return back the current width of the edges of the cells
  int GetEdges ();
  // Color of the cell
  Color cellColor;

private:
  // Number of the rows of the grid
  int rows{};
  // Number of the columns of the grid
  int columns{};
  // Size of the cell
  int cellSize{};
  // Vector of pair of the cells in grid
  vector<vector<int> > cells{};
  int edgesWidth{};
  // Is the row and column within the bound of the grid?
  bool IsWithinBounds (int row, int column);
};