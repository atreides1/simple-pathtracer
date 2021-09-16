#pragma once
#include "geometry.h"
#include "FreeImage.h"
#include <cstring>

class Image {
public:
	Image() : width(640), height(480), name("image.png") { init(); }
	Image(int w, int h, std::string s) : width(w), height(h), name(s) { init(); }

	void init() {
		// initialize pixel byte array
		pixels = FreeImage_Allocate(width, height, 24);
	}

	void writeToPixel(int i, int j, Vec3 color) {
		clamp(color, 0.0f, 1.0f);
		RGBQUAD col;
		col.rgbRed = static_cast<int>(color.x * 255.0f);
		col.rgbGreen = static_cast<int>(color.y * 255.0f);
		col.rgbBlue = static_cast<int>(color.z * 255.0f);

		FreeImage_SetPixelColor(pixels, i, j, &col);
	}

	void saveImage() {
		char* img_name = &name[0];
		FreeImage_Initialise();
		FreeImage_Save(FIF_PNG, pixels, img_name, 0);
		std::cout << FreeImage_GetCopyrightMessage() << '\n';
		FreeImage_DeInitialise();
	}

private:
	int width, height;
	std::string name;
	FIBITMAP* pixels; 
};