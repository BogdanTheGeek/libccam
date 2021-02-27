#include "libccam.h"

int main(){
	//start of program
	set_feed(100);
	set_rapid(1000);

	face(20,10,1,2);
	//end of program
	stop();
	return 0;
}
