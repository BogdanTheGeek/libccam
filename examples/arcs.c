#include "libccam.h"
point arc_origin = {3,0,0};	//center is 3 units to the right
int main(){
	//start of program
	set_feed(100);
	set_rapid(1000);
	
	move(0,0,0, ABS);

	move(0,10,0, ABS);
	arc(arc_origin, -180);	//can use predefined points

	move(0,-10,0, REL);
	arc( (point){2,0,0} , 90);	//or define them inside the function

	move(10,0,0, REL);
	//end of program
	stop();
	return 0;
}
