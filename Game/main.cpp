#include "InputManager.h"
// #include "../DisplayGLFW/display.h"
#include "game.h"
#include "../res/includes/glm/glm.hpp"
#include <stb_image.h>
#include <fstream>

// fucntion for assignment
unsigned char* Halftone(unsigned char* original);

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
	unsigned char* gray_scale = data; // TODO should implement a fucntion to change image to gray
	// step 4 in the assignment
	unsigned char* edges = data; // TODO should implement a fucntion to change image using sobel operators
	// step 5 in the assignment
	unsigned char* halftone = Halftone(data);
	// step 6 in the assignment
	unsigned char* floyd_steinberg = data; // TODO should implement a fucntion to change image using floyd steinberg algorithm



	// up - left image
	scn->AddTexture(256,256, gray_scale);
	scn->SetShapeTex(0, 0); // plane number, texture number
	scn->CustonDraw(1, 0, scn->BACK, true, false, 0);

	//// up - right image
	scn->AddTexture(256, 256, edges);
	scn->SetShapeTex(0, 1);
	scn->CustonDraw(1, 0, scn->BACK, false, false, 1);

	//// buttom - left image
	scn->AddTexture(512, 512, halftone);
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
			float intensity = (original[pixelClusterSize * (pixelClusterSize * 256 + j)] + original[pixelClusterSize * (i * 256 + j) + 1] + original[pixelClusterSize * (i * 256 + j) + 2]) / 3;
			int pixelYPos = 2 * i;
			int pixelXPos = 2 * j;			
			if (intensity > 0.8 * white) {
				result_2D[pixelYPos][pixelXPos] = white;
				result_2D[pixelYPos + 1][pixelXPos] = white;
				result_2D[pixelYPos][2 * pixelXPos] = white;
				result_2D[pixelYPos + 1][pixelXPos + 1] = white;
			}
			else if (intensity > 0.6 * white) {
				result_2D[pixelYPos][pixelXPos] = black;
				result_2D[pixelYPos + 1][pixelXPos] = white;
				result_2D[pixelYPos][pixelXPos + 1] = white;
				result_2D[pixelYPos + 1][pixelXPos + 1] = white;
			}
			else if (intensity > 0.4 * white) {
				result_2D[pixelYPos][pixelXPos] = black;
				result_2D[pixelYPos + 1][pixelXPos] = white;
				result_2D[pixelYPos][pixelXPos + 1] = white;
				result_2D[pixelYPos + 1][pixelXPos + 1] = black;

			}
			else if (intensity > 0.2 * white) {
				result_2D[pixelYPos][pixelXPos] = black;
				result_2D[pixelYPos + 1][pixelXPos] = white;
				result_2D[pixelYPos][pixelXPos + 1] = black;
				result_2D[pixelYPos + 1][pixelXPos + 1] = black;
			}
			else
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
