#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "Grid.h"
#include "Image.h"

int main()
{

	Grid img("sample2.bmp");
	int* width;
	width = new int;
	int* height;
	height = new int;

	//Two functions for filtering, EdgeFilter and WhiteFilter
	char** filteredGrid = img.WhiteFilter(width, height);

	//Function for printing grid
	Grid::printGrid(filteredGrid, *width, *height);
	//Save grid into a text file
	Grid::saveGrid(filteredGrid, *width, *height, "sample.txt");

	//Converting a grid into an image
	Image imggrid(*width, *height);
	imggrid.setColors(filteredGrid);
	imggrid.Export("sampleout.bmp");



}

