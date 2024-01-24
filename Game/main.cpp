#include "InputManager.h"
// #include "../DisplayGLFW/display.h"
#include "game.h"
#include "../res/includes/glm/glm.hpp"
#include <stb_image.h>
#include <fstream>

// fucntion for assignment
unsigned char* Halftone(unsigned char* original);
unsigned char* Floyd_Steinberg(unsigned char* original);
unsigned char* Canny(unsigned char* data);
std::vector<std::vector<unsigned char>>* Gaussine(std::vector<std::vector<unsigned char >>* pixels);
std::vector<std::vector<unsigned char>>* Filter(std::vector<std::vector<unsigned char>>* pixels, std::vector<std::vector<int>>* kernel, float div);
std::vector<std::vector<unsigned char>> Sobel(std::vector<std::vector<unsigned char>>* pixels);
std::vector<std::vector<unsigned char>> nonMaximumSupprition(std::vector<std::vector<unsigned char>> pixels);


int main(int argc,char *argv[])
{
	const int DISPLAY_WIDTH = 512; // changed here to 512 i.e.step 2 in the assignment
	const int DISPLAY_HEIGHT = 512; // changed here to 512
	const float CAMERA_ANGLE = 0.0f;
	const float NEAR = 1.0f;
	const float FAR = 100.0f;

	Game *scn = new Game(CAMERA_ANGLE,(float)DISPLAY_WIDTH/DISPLAY_HEIGHT,NEAR,FAR);
	
	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");
	
	Init(display);
	
	scn->Init();

	display.SetScene(scn);

	// from tirgul 3 i.e step 1 in the assignment
	int width, height, numComponents; //this code was copied from Texture constructor, and based from tergul 3
	const std::string fileName = "../res/textures/lena256.jpg";
	unsigned char* data = stbi_load((fileName).c_str(), &width, &height, &numComponents, 4);

	// step 3 in the assignment
	unsigned char* gray_scale = data;
	// step 4 in the assignment
	unsigned char* edges = Canny(data);
	// step 5 in the assignment
	unsigned char* halftone = Halftone(data);
	// step 6 in the assignment
	unsigned char* floyd_steinberg = Floyd_Steinberg(data); // TODO should implement a fucntion to change image using floyd steinberg algorithm



	// up - left image
	scn->AddTexture(256,256, gray_scale);
	scn->SetShapeTex(0, 0); // plane number, texture number
	scn->CustonDraw(1, 0, scn->BACK, true, false, 0);

	//// up - right image
	scn->AddTexture(256, 256, edges);
	scn->SetShapeTex(0, 1);
	scn->CustonDraw(1, 0, scn->BACK, false, false, 1);

	//// buttom - left image
	scn->AddTexture(512, 512, halftone); // need to create image with twice the resolution
	scn->SetShapeTex(0, 2);
	scn->CustonDraw(1, 0, scn->BACK, false, false, 2);

	//// buttom - right image
	scn->AddTexture(256, 256, floyd_steinberg);
	scn->SetShapeTex(0, 3);
	scn->CustonDraw(1, 0, scn->BACK, false, false, 3);
	

	scn->Motion();
	display.SwapBuffers();

	while(!display.CloseWindow())
	{
		/*scn->Draw(1,0,scn->BACK,true,false);
		scn->Motion();
		display.SwapBuffers();*/
		display.PollEvents();	
			
	}
	delete scn;
	return 0;
}




unsigned char* Canny(unsigned char* data) {
	std::vector<std::vector<unsigned char>>* original = new std::vector<std::vector<unsigned char>>();
	for (int i = 0; i < 256; i++) {
		std::vector<unsigned char> row;
		for (int j = 0; j < 256; j++) {
			int r = 4 * (i * 256 + j);
			int g = 4 * (i * 256 + j) + 1;
			int b = 4 * (i * 256 + j) + 2;
			unsigned char gray_pixle = (unsigned char)(0.3 * data[r] + 0.58 * data[g] + 0.11 * data[b]);
			row.push_back(gray_pixle);
		}
		original->push_back(row);
	}
	std::vector<std::vector<unsigned char>>* gauss = Gaussine(original);
	std::vector<std::vector<unsigned char>> sobel = Sobel(gauss);
	std::vector<std::vector<unsigned char>> suppression = nonMaximumSupprition(sobel);
	unsigned char* result = (unsigned char*)malloc(512 * 512);
	for (int i = 0, counter = 0; i < 256; i++) {
		for (int j = 0; j < 256; j++) {
			int r = 4 * (i * 256 + j);
			int g = 4 * (i * 256 + j) + 1;
			int b = 4 * (i * 256 + j) + 2;
			int alpha = 4 * (i * 256 + j) + 3;
			result[r] = (suppression)[i][j];
			result[g] = (suppression)[i][j];
			result[b] = (suppression)[i][j];
			result[alpha] = 255;
		}
	}
	// write to img4.txt
	std::ofstream img4("img4.txt");
	if (img4.is_open())
	{
		for (int i = 0; i < 256; i++) {
			img4 << "\n";
			for (int j = 0; j < 256; j++) {
				
				img4 << (result[4 * 256 * i + 4*j] > 0 ? "0":"1") << ",";
			}
		}
		img4.close();
	}
	return result;
}


std::vector<std::vector<unsigned char>>* Gaussine(std::vector<std::vector<unsigned char >>* pixels) {
	std::vector<std::vector<int>>* gk = new std::vector<std::vector<int>>();
	gk->push_back(std::vector<int>{1, 2, 1});
	gk->push_back(std::vector<int>{2, 4, 2});
	gk->push_back(std::vector<int>{1, 2, 1});
	return Filter(pixels, gk, 16.0);
}

std::vector<std::vector<unsigned char>>* Filter(std::vector<std::vector<unsigned char>>* pixels, std::vector<std::vector<int>>* kernel, float div) {
	std::vector<std::vector<unsigned char>>* filtered = new std::vector<std::vector<unsigned char>>();
	for (int i = 1; i < 255; i++) {
		std::vector<unsigned char> row;
		for (int j = 1; j < 255; j++) {
			float sum = 0;
			for (int x = 0; x < 3; x++) {
				for (int y = 0; y < 3; y++) {
					sum += (*kernel)[x][y] * (*pixels)[i + x - 1][j + y - 1];
				}
			}
			sum /= div;
			sum = sum < 0 ? 0 : sum>255 ? 255 : sum;
			unsigned char new_pixel = (char)sum;
			row.push_back(new_pixel);
		}
		filtered->push_back(row);
	}

	for (auto& row : *filtered) {
		row.insert(row.begin(), 0);
		row.push_back(0);
	}
	std::vector<unsigned char> brow(256, 0);
	filtered->insert(filtered->begin(), brow);
	filtered->push_back(brow);
	return filtered;
}


std::vector<std::vector<unsigned char>> Sobel(std::vector<std::vector<unsigned char>>* pixels) {
	std::vector<std::vector<int>>* k_x = new std::vector<std::vector<int>>();
	k_x->push_back(std::vector<int>{-1, 0, 1});
	k_x->push_back(std::vector<int>{-2, 0, 2});
	k_x->push_back(std::vector<int>{-1, 0, 1});

	std::vector<std::vector<int>>* k_y = new std::vector<std::vector<int>>();
	k_y->push_back(std::vector<int>{1, 2, 1});
	k_y->push_back(std::vector<int>{0, 0, 0});
	k_y->push_back(std::vector<int>{-1, -2, -1});

	std::vector<std::vector<unsigned char>>* x = Filter(pixels, k_x, 1.0);
	std::vector<std::vector<unsigned char>>* y = Filter(pixels, k_y, 1.0);
	std::vector<std::vector<unsigned char>> sobel(256, std::vector<unsigned char>(256, 0));

	for (int i = 1; i < 256; i++)
	{
		for (int j = 1; j < 256; j++)
		{
			unsigned char gx = (*x)[i][j];
			unsigned char gy = (*y)[i][j];
			unsigned char val = abs(gx) + abs(gy);
			(sobel)[i][j] = val;
		}
	}
	return sobel;
}

std::vector<std::vector<unsigned char>> nonMaximumSupprition(std::vector<std::vector<unsigned char>> pixels) {
	std::vector<std::vector<float>>* angles = new std::vector<std::vector<float>>();
	for (int i = 0; i < 256; i++) {
		std::vector<float> row;
		for (int j = 0; j < 256; j++) {
			float angle = (float)(pixels)[i][j] * 180.0 / 3.14;
			angle = angle < 0 ? angle + 180 : angle;
			row.push_back(angle);
		}
		angles->push_back(row);
	}
	std::vector<std::vector<unsigned char>> suppression(256, std::vector<unsigned char>(256, 0));

	for (int i = 1; i < 255; i++) {
		for (int j = 1; j < 255; j++) {
			int q = 255;
			int r = 255;
			float angle = (*angles)[i][j];
			if (0 <= angle < 22.5 | 157.5 <= angle <= 180) {
				q = (pixels)[i][j + 1];
				r = (pixels)[i][j - 1];
			}
			else if (22.5 <= angle < 67.5) {
				q = (pixels)[i + 1][j - 1];
				r = (pixels)[i - 1][j + 1];
			}
			else if (67.5 <= angle < 112.5) {
				q = (pixels)[i + 1][j];
				r = (pixels)[i - 1][j];
			}
			else if (112.5 <= angle < 157.5) {
				q = (pixels)[i - 1][j - 1];
				r = (pixels)[i + 1][j + 1];
			}
			if (((pixels)[i][j] >= q) & ((pixels)[i][j] >= r))
				(suppression)[i][j] = (pixels)[i][j];
		}
	}
	return suppression;
}



unsigned char* Halftone(unsigned char* original)
{
	unsigned char* result = new unsigned char[256 * 2 * 256 * 2 * 16];
	int pixelClusterSize = 4; //each pixel becomes 4 pixel
	int white = 255;
	int black = 0;
	// first step initial result 2D array
	unsigned char** result_2D = (unsigned char**)malloc(pixelClusterSize/2 * 256 * sizeof(unsigned char*));
	for (int i = 0; i < 256 * pixelClusterSize/2; i++)
		result_2D[i] = (unsigned char*)malloc(pixelClusterSize / 2 * 256 * sizeof(unsigned char));
	for (int i = 0; i < 256; i++)
		for (int j = 0; j < 256; j++) 
		{
			// calculate average intensity
			float intensity = (original[pixelClusterSize * 256 * i + pixelClusterSize * j] + original[pixelClusterSize * 256 * i + pixelClusterSize * j + 1] + original[pixelClusterSize * 256 * i + pixelClusterSize * j + 2]) / 3;
			int pixelYPos = 2 * i;
			int pixelXPos = 2 * j;			
			if (1.0 * white >= intensity >= 0.8 * white) {
				result_2D[pixelYPos][pixelXPos] = white;
				result_2D[pixelYPos + 1][pixelXPos] = white;
				result_2D[pixelYPos][pixelXPos + 1] = white;
				result_2D[pixelYPos + 1][pixelXPos + 1] = white;
			}
			else if (intensity >= 0.6 * white) {
				result_2D[pixelYPos][pixelXPos] = white;
				result_2D[pixelYPos + 1][pixelXPos] = white;
				result_2D[pixelYPos][pixelXPos + 1] = white;
				result_2D[pixelYPos + 1][pixelXPos + 1] = black;
			}
			else if (intensity >= 0.4 * white) {
				result_2D[pixelYPos][pixelXPos] = white;
				result_2D[pixelYPos + 1][pixelXPos] = white;
				result_2D[pixelYPos][pixelXPos + 1] = black;
				result_2D[pixelYPos + 1][pixelXPos + 1] = black;

			}
			else if (intensity >= 0.2 * white) {
				result_2D[pixelYPos][pixelXPos] = white;
				result_2D[pixelYPos + 1][pixelXPos] = black;
				result_2D[pixelYPos][pixelXPos + 1] = black;
				result_2D[pixelYPos + 1][pixelXPos + 1] = black;
			}
			else if (intensity >= 0)
			{
				result_2D[pixelYPos][pixelXPos] = black;
				result_2D[pixelYPos + 1][pixelXPos] = black;
				result_2D[pixelYPos][pixelXPos + 1] = black;
				result_2D[pixelYPos + 1][pixelXPos + 1] = black;
			}
		}
	// each pixel became 4 pixels in the new black and white picture also writing to text file
	int pixilPos = 0;
	std::ofstream img5("img5.txt");
	if (img5.is_open())
	{
		for (int i = 0; i < 256 * pixelClusterSize/2; i++) {
			img5 << "\n";
			for (int j = 0; j < 256 * pixelClusterSize/2; j++, pixilPos += 4) {
				result[pixilPos] = result_2D[i][j];     result[pixilPos + 1] = result_2D[i][j];
				result[pixilPos + 2] = result_2D[i][j]; result[pixilPos + 3] = result_2D[i][j];
				img5 << result_2D[i][j]/255 << ",";
			}
		}
		img5.close();
	}
	return result;
}




unsigned char* Floyd_Steinberg(unsigned char* original)
{
	unsigned char* result = new unsigned char[256 * 2 * 256 * 2 * 8];
	int pixelClusterSize = 4;
	int pixelPos = 0;
	// second step initial result 2D array
	unsigned char result_2D[256][256];
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++,pixelPos++)
		{
			result_2D[i][j] = (original[pixelClusterSize * 256 * i + pixelClusterSize * j] + original[pixelClusterSize * 256 * i + pixelClusterSize * j + 1] + original[pixelClusterSize * 256 * i + pixelClusterSize * j + 2]) / 3;
		}
	}

	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 256; j++) {

			if (j - 1 >= 0 && i + 1 < 256 && j + 1 < 256)
			{
				// Error Diffusion Dither just as explained in lecture 2 page 120
				int l = result_2D[i][j];
				int P = ((l - 1) /16)*16 + 1; // trunc(l(x,y) + 0.5)
				float e = l - P;
				float a = 7 / 16.0;
				float b = 3 / 16.0;
				float g = 5 / 16.0;
				float d = 1 / 16.0;
				result_2D[i][j+1] += a * e;
				result_2D[i + 1][j - 1] += b * e ;
				result_2D[i + 1][j] += g * e ;
				result_2D[i + 1][j+1] += d * e ;
			}
		}
	}

	pixelPos = 0;
	//int pixilPos = 0;
	std::ofstream img6("img6.txt");
	if (img6.is_open())
	{
		for (int i = 0; i < 256 ; i++) {
			img6 << "\n";
			for (int j = 0; j < 256; j++, pixelPos += 4) {
				result[pixelPos] = result_2D[i][j];     result[pixelPos + 1] = result_2D[i][j];
				result[pixelPos + 2] = result_2D[i][j]; result[pixelPos + 3] = result_2D[i][j];
				img6 << result_2D[i][j]/16 << ",";
			}
		}
		img6.close();
	}
	return result;
}