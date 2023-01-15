#pragma once
#include <fstream>
class Grid
{
public:
	Grid(std::string filename);
	~Grid();

	char** WhiteFilter(int* x, int* y);
	char** EdgeFilter(int* x, int* y);
	static void printGrid(char** grid, int width, int height);
	static void saveGrid(char** grid, int width, int height, std::string filename);
private:

	char* fileheader;
	const int fileheadersize = 14;
	char* ihdrportion;
	const int portionsize = 12;
	int filesize;
	int ihdrsize;
	int width;
	int height;
	

	char** Edgegrid;
	char** GreyGrid;
	char** rawdata = nullptr;
	int getGreyValue(char* block, int start);
	int getEdgevalue(char** grid, int x, int y);
	int getvalue(char* block, int size, int start);
	int getbytevalue(char** grid, int x, int y);
};