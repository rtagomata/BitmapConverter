#include "Grid.h"
#include <iostream>

Grid::Grid(std::string filename)
{

	std::ifstream f;
	f.open(filename, std::ios::binary);
	if (!f)
	{
		std::cout << "Error reading file" << std::endl;
		return;
	}
	//First 14 bytes of a BMP is the fileheader
	fileheader = new char[fileheadersize];
	//Ihdr size is at byte 10-14 of fileheader
	//Skipping most of ihdr assuming theres no compression and bmp size is width * height * 4 + ihdrsize + fileheadersize
	f.close();
	f.open(filename, std::ios::binary);
	f.seekg(fileheadersize);
	ihdrportion = new char[portionsize];
	f.read(ihdrportion, portionsize);
	ihdrsize = getvalue(ihdrportion, 4, 0);
	width = getvalue(ihdrportion, 4, 4);
	height = getvalue(ihdrportion, 4, 8);
	rawdata = new char* [height];
	for (int i = 0; i < height; i++)
	{
		char* rawrowdata;
		rawrowdata = new char[width * 4];
		f.clear();
		f.seekg(14 + ihdrsize + (i + 1) * width * 4);
		f.read(rawrowdata, 4 * width);
		rawdata[height - i - 1] = rawrowdata;
	}

}

Grid::~Grid()
{

}



int Grid::getvalue(char* block, int size, int start)
{

	int num = 0;
	int temp;
	//Iterate through the block and each iteration the number is multiplied by a power of 16
	for (int i = start; i < start + size; i++)
	{
		//Converting char to int sometimes give a negative result so adding 256 if that is the case
		temp = ((int)block[i] * pow(256, i - start) < 0) ? (int)block[i] * pow(256, i - start) + 256 : (int)block[i] * pow(256, i - start);
		num += temp;
	}
	num = (num < 0) ? (num + 256) : num;
	return num;
}


char** Grid::WhiteFilter(int* x, int* y)
{
	if (rawdata == nullptr)
	{
		return nullptr;
	}
	GreyGrid = new char* [height];
	for (int y = 0; y < height; y++)
	{
		char* row;
		row = new char[width];
		for (int x = 0; x < width; x++)
		{
			int GreyValue = getGreyValue(rawdata[y], x * 4);
			row[x] = (GreyValue > 240) ? ' ' : '#';
		}
		GreyGrid[y] = row;
	}
	*x = width;
	*y = height;
	return GreyGrid;

}

char** Grid::EdgeFilter(int* x, int* y)
{
	if (rawdata == nullptr)
	{
		return nullptr;
	}

	Edgegrid = new char* [height];
	for (int y = 0; y < height; y++)
	{
		char* row;
		row = new char[width];
		for (int x = 0; x < width; x++)
		{
			row[x] = (getEdgevalue(rawdata, x, y) == 255) ? '#' : ' ';
		}
		Edgegrid[y] = row;
	}
	*x = width;
	*y = height;
	return Edgegrid;
}

int Grid::getGreyValue(char* block, int start)
{
	int sum = 0;
	int temp;
	for (int i = 0; i < 3; i++)
	{
		temp = ((int)block[i + start] < 0) ? (int)block[i + start] + 256 : (int)block[i + start];
		sum += temp;
	}
	sum = sum / 3;
	return sum;
}
void Grid::printGrid(char** grid, int width, int height)
{
	for (int y = 0; y < height; y++) 
	{
		for (int x = 0; x < width; x++) 
		{
			std::cout << grid[y][x];
		}
		std::cout << "\n";
	}
}



void Grid::saveGrid(char** grid, int width, int height, std::string filename)
{
	std::ofstream f;
	f.open(filename);
	if (!f)
	{
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				f << grid[y][x];
			}
			f << "\n";
		}
		f.close();
		std::cout << "Successfully saved\n";
	}
	
	f.close();
	std::cout << "Error creating file\n";
}

int Grid::getbytevalue(char** grid, int x, int y)
{
	int temp;
	temp = (grid[y][x] < 0) ? grid[y][x] + 256 : grid[y][x];
	return temp;
}
int Grid::getEdgevalue(char** grid, int x, int y)
{
	int bxsum = 0;
	int gxsum = 0;
	int rxsum = 0;

	int bysum = 0;
	int gysum = 0;
	int rysum = 0;
	//Using sobels algorithm to find the edge, outside of bounds are treated as zeroes
	for (int i = -1; i < 2; i++)
	{
		if (i + y < 0 || i + y >= height) continue;
		for (int j = -1; j < 2; j++)
		{
			if (j + x < 0 || j + x >= width) continue;

			bxsum += getbytevalue(grid, (j + x) * 4, i + y) * j * (2 - abs(i));
			gxsum += getbytevalue(grid, (j + x) * 4 + 1, i + y) * j * (2 - abs(i));
			rxsum += getbytevalue(grid, (j + x) * 4 + 2, i + y) * j * (2 - abs(i));

			bysum += getbytevalue(grid, (j + x) * 4, i + y) * i * (2 - abs(j));
			gysum += getbytevalue(grid, (j + x) * 4 + 1, i + y) * i * (2 - abs(j));
			rysum += getbytevalue(grid, (j + x) * 4 + 2, i + y) * i * (2 - abs(j));
		}
	}
	int b = (bxsum * bxsum + bysum * bysum < 0) ? 0 : bxsum * bxsum + bysum * bysum;
	b = (b > 255) ? 255 : b;
	int g = (gxsum * gxsum + gysum * gysum < 0) ? 0 : gxsum * gxsum + gysum * gysum;
	g = (g > 255) ? 255 : g;
	int r = (rxsum * rxsum + rysum * rysum < 0) ? 0 : rxsum * rxsum + rysum * rysum;
	r = (r > 255) ? 255 : r;
	int res = (b + g + r) / 3;
	return res;
}