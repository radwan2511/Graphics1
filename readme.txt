here we will document our changes/steps we did in the engine:

step 1:
initial push to git
plus we added fixes from tirgul 2: in Engine/Source Files/Mesh.cpp
we updated Vertex vertices[] from tirgul 2 to fix to coloring and show image in the right angle (it was flipped)

step 2:
changes and addition from tirgul 3 and first steps in the assignment
imlemented steps 1 and 2 in the assignment and set up base for the whole assignment
in files scene.cpp and scene.h added function CostumDraw (from tirgul 3) with function varialble for image location
1 - top left, 2- top right, 3- buttom left, 4- buttom right with the relevant glViewPort()
step 1 in assignment inside main.cpp in function main() read the lena256.jpg image to a one dimentional array using stbi_load
just as suggested from tirgul 3
step 2 in assignment inside main.cpp in function main() changed the value of DISPLAY_WIDTH and DISPLAY_HEIGHT to 512 

step 3:
implement gray scale and add documentation to here


step 4:
implement edge and add documentation to here


step 5: 
in main.cpp added function Halftone(original data) and changed the values in addTexture for halftone from hieght and width 256 to 512
because each pixel becomes 4 pixel in the new black and white image:
halftone applying:
1. initalize result one dimentional array
2. initialize result two dimentional array
3. loop through the original data and calculate average intensity 
4. update result two dimention array based on the average intensity same as in lecture 2 last slides
where: 1.0 >= l >= 0.8 --> all 4 pixels are white
where: 0.8 > l >= 0.6 --> 3 pixels are white and one is black
where: 0.6 > l >= 0.4 --> 2 pixels are white and 2 are black
where: 0.4 > l >= 0.2 --> 1 pixel is white and 3 are black
where: 0.2 > l >= 0 --> all 4 pixels are black
5. update the result one dimentional array after applying the halftone pattern each pixel is 4 pixel in the result and write to img5.txt file


step 6: 
implement floyd and add documentation to here







