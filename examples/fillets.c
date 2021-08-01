#include "libccam.h"

double angle1 = 90.0;
double angle2 = 180.0;
double angle3 = -90.0;

point contour[8] = {
	{45,0,0},			//bottom side
	{0,6,0, ARC, &angle1},	//bottom right corner
	{0,15,0},			//right side
	{-2,0,0, ARC, &angle2},	//turn around
	{0,-13,0},		
	{-4,0,0, ARC, &angle3},	//inside corner
	{-43,0,0},
	{0,-4,0}
};

int len = sizeof(contour)/sizeof(point);

int main(){
	//start of program
	set_feed(100);
	set_rapid(1000);
	
	travel(-10,-10,0, ABS);

	move(0,0,-3, REL);
	profile(contour, len, REL);	
	move(0,0,3, REL);
	
	//end of program
	stop();
	return 0;
}
