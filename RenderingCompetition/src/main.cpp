/*
* *********************************************
*
* Rendering Competition - CG1
*
* (c) 2014 by Alexander Knueppel & Sven Frank        
*
* *********************************************
*/

#include "../include/Image.h"
#include "../include/Random.h"
#include <iostream>

const int iWidth = 640;
const int iHeight = 480;

int main(int argc, char* argv[]) {
	//Image
	Image img(iWidth, iHeight); 
	//Random number generator (numbers are in the range [0,1])
	RandomFloat rnd;

	//Write random colors to ppm
	for (int y = 0; y < iHeight; y++) {
		for (int x = 0; x < iWidth; x++) {
			//three random colors for each channel
			img[y][x] = glm::vec3(rnd(),rnd(),rnd()); 
		}
		printf("Progress: %6.2f %% rendered... \r", y*iWidth*100.f / (iWidth*iHeight));
	}
	img.writePPM("results/result.ppm"); 
	
	return 0;
}