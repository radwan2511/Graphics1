radwan ganem 322509951
moslem asaad 315646802
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
no need to implement because picture is already in gray scale as stated in the assignment pdf


step 4:
in main.cpp added function Canny(original data) which includes the whole steps for the edge detection algorithm, 
canny function includes: 
1. graying the original pixels and creating a 2d vector of grayed chars. 
2. applying the Gaussine which is calling afunction called Gaussine(grayed pixels vector). 
  2.1 filtering the pixels with the 3x3 gauss kernel which is {[1,2,1],[2,4,2],[1,2,1]} ==> blurred image result.
3. applying sobel operator for the blurred image: 
  3.1 creating filtered image with the {[-1,0,1],[-2,0,2],[-1,0,1]} kernel for x direction.
  3.2 creating filtered image with the {[1,2,1],[0,0,0],[-1,-2,-1]} kernel for y direction.
  3.3 creating the gradient image 
  explanation in "https://homepages.inf.ed.ac.uk/rbf/HIPR2/sobel.htm"
4. applying Non-maximum Suppression to the gradiented pixles: 
  4.1 computing and creating angles matrix. 
  4.2 based in the angle decide which pixles should be in the comparation: 
      for example, if the angle is 15, deciding the color would be compared with the left and the right pixles of the gradiented image, and if the current pixle value is     
      bigger than both which means the edge strength is bigger than the neigbors, we keep it otherwise it turned to black. 
 in another words, comparing the edge strength of a pixle with its corresponding neighbors, and based on there edge strength the color (pixle value) is determind. 

step 5: 
in main.cpp added function Halftone(original data) and changed the values in addTexture for halftone from hieght and width 256 to 512 also stated in tips for assignment in practical lecture 2
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
in main.cpp added function Floyd_Steinberg(original data) which changes the
image from 256 intensity grayscale values, to 16 intensity grayscale values
floyd-steinberg applying:
1. initalize result one dimentional array
2. initialize result two dimentional array
3. loop through the original data and calculate average intensity i.e l(x,y) and save it in original two dimentional array
4. loop through the new data and update pixels in result two dimentional array just as studied and described in lecture 2 page 120
5. P = trunc(l(x,y) + 0.5), e = l(x,y) - P, alpha = 7/16, beta = 3/16, gamma = 5/16, delta = 1/16
6. apply this through the nested for loop
                                            column j
[                                   ,    P(x,y) = l(x,y)               ,    l(x,y+1) += alpha*e    ]
[ l(x+1,y-1) = beta*e     ,   l(x+1,y) += gamma*e   ,    l(x+1,y+1) += delta*e ] 
7. update result two dimention array to one dimentionary array and write to img6.txt file







