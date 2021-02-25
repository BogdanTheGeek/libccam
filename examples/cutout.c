#include "libccam.h"

point square[4] = {{1,0},{1,1},{0,1},{0,0}};
point relative_square[4] = {{1,0},{0,1},{-1,0},{0,-1}};

int main(){
	//start of program
	set_feed(100);
	set_rapid(1000);
	
	profile(square, 4, false);	
	
	travel(10,10,0, false);
	profile(relative_square, 4, true);	
	travel(20,10,0, false);
	profile(relative_square, 4, true);	
	travel(30,10,0, false);
	profile(relative_square, 4, true);	

	//end of program
	return 0;
}
