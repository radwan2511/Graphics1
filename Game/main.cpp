#include "InputManager.h"
// #include "../DisplayGLFW/display.h"
#include "game.h"
#include "../res/includes/glm/glm.hpp"
#include <stb_image.h>

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
	unsigned char* halftone = data; // TODO should implement a fucntion to change image using halftone according to last slide in lecture2
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
	scn->AddTexture(256, 256, halftone);
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
