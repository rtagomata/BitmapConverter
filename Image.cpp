#pragma once
#include "Image.h"
#include <iostream>
#include  <fstream>
#define BLACK 0
#define WHITE 1
#define RED 2
Color::Color() :r(0), g(0), b(0)
{
}

Color::Color(int r, int g, int b) : r(r), g(g), b(b)
{
}

Color::~Color()
{
}

Image::Image(int width, int height) :m_width(width), m_height(height), m_colors(std::vector<Color>(width* height))
{
}

Image::~Image()
{
}



void Image::setColors(char** grid)
{
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			
			switch (grid[m_height - y - 1][x])
			{
			case ' ':
				setBitColor(Color(255, 255, 255), x, y);
				break;
			default:
				setBitColor(Color(0, 0, 0), x, y);
			}
		}
	}
}


Color Image::getColor(int x, int y) const
{
	return m_colors[y * m_width + x];
}

void Image::setBitColor(const Color& color, int x, int y)
{
	m_colors[y * m_width + x].r = color.r;
	m_colors[y * m_width + x].g = color.g;
	m_colors[y * m_width + x].b = color.b;
}

void Image::Export(const char* path)
{
	std::ofstream f;
	f.open(path, std::ios::out | std::ios::binary);

	if (!f.is_open())
	{
		std::cout << "File could not be opened\n";
		return;
	}

	unsigned char bmPad[3] = { 0, 0, 0 };

	const int fileHeaderSize = 14;
	const int informationHeaderSize = 108;
	const int fileSize = fileHeaderSize + informationHeaderSize + m_width * m_height * 4;

	unsigned char fileheader[fileHeaderSize];

	fileheader[0] = 'B';
	fileheader[1] = 'M';

	fileheader[2] = fileSize;
	fileheader[3] = fileSize >> 8;
	fileheader[4] = fileSize >> 16;
	fileheader[5] = fileSize >> 24;

	fileheader[6] = 0;
	fileheader[7] = 0;
	fileheader[8] = 0;
	fileheader[9] = 0;

	fileheader[10] = fileHeaderSize + informationHeaderSize;
	fileheader[11] = 0;
	fileheader[12] = 0;
	fileheader[13] = 0;

	unsigned char informationHeader[informationHeaderSize];

	informationHeader[0] = informationHeaderSize;
	informationHeader[1] = 0;
	informationHeader[2] = 0;
	informationHeader[3] = 0;

	informationHeader[4] = m_width;
	informationHeader[5] = m_width >> 8;
	informationHeader[6] = m_width >> 16;
	informationHeader[7] = m_width >> 24;

	informationHeader[8] = m_height;
	informationHeader[9] = m_height >> 8;
	informationHeader[10] = m_height >> 16;
	informationHeader[11] = m_height >> 24;

	informationHeader[12] = 1;
	informationHeader[13] = 0;

	informationHeader[14] = 32;
	informationHeader[15] = 0;

	for (int i = 16; i < informationHeaderSize; i++)
	{
		informationHeader[i] = 0;
	}

	f.write(reinterpret_cast<char*>(fileheader), fileHeaderSize);
	f.write(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			unsigned char r = static_cast<unsigned char>(getColor(x, y).r);
			unsigned char g = static_cast<unsigned char>(getColor(x, y).g);
			unsigned char b = static_cast<unsigned char>(getColor(x, y).b);
			unsigned char a = static_cast<unsigned char>(255);
			unsigned char color[] = {b, g, r, a};

			f.write(reinterpret_cast<char*>(color), 4);
		}
	}
	f.close();

	std::cout << "File Created\n";
}
