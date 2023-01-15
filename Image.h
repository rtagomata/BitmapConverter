#pragma once

#include <vector>

struct Color {
	float r, g, b;

	Color();
	Color(int r, int g, int b);
	~Color();
};

class Image
{
public:
	Image(int width, int height);
	~Image();



	void setColors(char** grid);
	void Export(const char* path);
	
private:
	Color getColor(int x, int y) const;
	void setBitColor(const Color& color, int x, int y);
	int m_width;
	int m_height;
	std::vector<Color> m_colors;
};