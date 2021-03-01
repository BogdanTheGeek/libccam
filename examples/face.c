#include "libccam.h"

int main(){
	//start of program
	set_feed(100);
	set_rapid(1000);

	travel(10,20,0,ABS);
	move(0,0,-1, REL);
	face(20,10,1,60);	// all values must be positive
	//end of program
	stop();
	return 0;
}
