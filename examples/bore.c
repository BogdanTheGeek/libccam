#include "libccam.h"

int main(){
	//start of program
	set_feed(100);
	set_rapid(1000);

	travel(-10,-10,1, false);	//go to first bore's abolute location
	bore(4, 1, 3, true);	//bore a 4mm whole, 1mm spiral pitch, 3mm deep with a floor

	travel(10,-10,1, false);	//go to next bore's abolute location
	bore(4, 1, 3, true);	//bore a 4mm whole, 1mm spiral pitch, 3mm deep with a floor

	travel(10,10,1, false);	//go to next bore's abolute location
	bore(4, 1, 3, true);	//bore a 4mm whole, 1mm spiral pitch, 3mm deep with a floor

	travel(-10,10,1, false);	//go to last bore's abolute location
	bore(4, 1, 3, true);	//bore a 4mm whole, 1mm spiral pitch, 3mm deep with a floor

	//end of program
	return 0;
}
