#include "grid.hpp"

void
Grid::Draw ()
{
  for (int row = 0; row < rows; row++)
    {
      for (int column = 0; column < columns; column++)
        {
          
          DrawRectangle (column * cellSize, row * cellSize,
                         cellSize - edgesWidth, cellSize - edgesWidth, cellColor);
        }
    }
}

void
Grid::SetValue (int row, int column, int value)
{
  if (IsWithinBounds (row, column))
    {
      cells[row][column] = value;
    }
}

int
Grid::GetValue (int row, int column)
{
  if (IsWithinBounds (row, column))
    {
      return cells[row][column];
    }
  else
    {
      return 0;
    }
}

int
Grid::GetRows ()
{
  return rows;
}

int
Grid::GetColumns ()
{
  return columns;
}

void
Grid::Clear ()
{
  for (int row = 0; row < rows; row++)
    {
      for (int column = 0; column < columns; column++)
        {
          cells[row][column] = 0;
        }
    }
}

bool
Grid::IsClear ()
{
  int sum = 0;

  for (int row = 0; row < rows; row++)
    {
      for (int column = 0; column < columns; column++)
        {
          sum += cells[row][column];
        }
    }

  return (sum == 0) ? true : false;
}

void
Grid::SetEdges (int width)
{
  edgesWidth = width;
}

int
Grid::GetEdges ()
{
  return edgesWidth;
}

bool
Grid::IsWithinBounds (int row, int column)
{
  return (row >= 0 && row < rows && column >= 0 && column < columns) ? true
                                                                     : false;
}