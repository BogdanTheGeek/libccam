#include "libccam.h"

point square[4] = {{1,0},{1,1},{0,1},{0,0}};
point relative_square[4] = {{1,0},{0,1},{-1,0},{0,-1}};

point contour[4] = {{40,-10,-3},{40,20,-3},{-10,20,-3},{-10,-10,-3}};

int main(){
	//start of program
	set_feed(100);
	set_rapid(1000);
	
	move(0,0,-1, REL);
	profile(square, 4, ABS);	
	move(0,0,1, REL);
	
	travel(10,10,0, false);
	
	move(0,0,-1, REL);
	profile(relative_square, 4, REL);	
	move(0,0,1, REL);
	
	travel(20,10,0, false);
	
	move(0,0,-1, REL);
	profile(relative_square, 4, REL);	
	move(0,0,1, REL);
	
	travel(30,10,0, false);

	move(0,0,-1, REL);
	profile(relative_square, 4, REL);	
	move(0,0,1, REL);

	travel(-10,-10,0, ABS);

	move(0,0,-3, REL);
	profile(contour, 4, ABS);	
	move(0,0,3, REL);
	

	//end of program
	stop();
	return 0;
}
