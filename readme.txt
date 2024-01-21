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
implement halftone and add documentation to here


step 6: 
implement floyd and add documentation to here







