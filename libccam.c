/*
MIT License

Copyright (c) 2021 Bogdan Ionescu

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "libccam.h"

#include <stdio.h>
#include <math.h>

#define PI 3.141592654

//variables
static int decimal_places = 3;

//private functions
double mag(point p){
	return sqrt(p.x*p.x + p.y*p.y);
}

//public functions
void set_feed(double f){	//set feedrate for cutting
	printf("G1 F%0.*f;\n", decimal_places, f);
}

void set_rapid(double f){	//set feedrate for travel
	printf("G0 F%0.*f;\n", decimal_places, f);
}

void pause(){
	printf("M0;\n");
}

void stop(){
	printf("M30;\n");

}

void wait(int millis){
	printf("G4 P%d;\n", millis);
}

void make_relative(point *p, int len){	//convert absolute shape into relative shape
	
	for(int i=len; i>0; i--){
		p[i].x = p[i].x - p[i-1].x;
		p[i].y = p[i].y - p[i-1].y;
		p[i].z = p[i].z - p[i-1].z;
	}
}

void make_absolute(point *p, int len){	//convert relative shape into absolute shape

	point absolute_p = {0,0,0};
	for(int i=0; i<len; i++){
		p[i].x = absolute_p.x + p[i].x;
		p[i].y = absolute_p.y + p[i].y;
		p[i].z = absolute_p.z + p[i].z;
		
		absolute_p.x = p[i].x;
        absolute_p.y = p[i].y;
        absolute_p.z = p[i].z;
	}
}

void move(double x, double y, double z, bool relative){	//move with cutting feedrate
	if(relative)printf("G91 ");
	else printf("G90 ");
	printf("G1 X%0.*f Y%0.*f Z%0.*f;\n", decimal_places, x, decimal_places, y, decimal_places, z);
}

void travel(double x, double y, double z, bool relative){	//move with rapid feedrate
	if(relative)printf("G91 ");
	else printf("G90 ");
	printf("G0 X%0.*f Y%0.*f Z%0.*f;\n", decimal_places, x, decimal_places, y, decimal_places, z);
}

void arc(point p, double deg){	//move in an arc around point p for deg degrees
	if(deg<0){
		printf("G2 ");
		//deg = -deg;
	}else{
		printf("G3 ");
	}
	double x, y, ux, uy;
	//deg=deg - x*90 - y*90;
	deg = (deg * PI) / 180;	//convert to rads
	ux = p.x/mag(p);
	uy = p.y/mag(p);
	//deg += acos(-ux) + acos(-uy); //rotate the circle to match center vector 
	deg += atan2(-uy, -ux);
	x = p.x + cos(deg);
	y = p.y + sin(deg);

	printf("X%0.*f Y%0.*f Z%0.*f I%0.*f J%0.*f;\n", decimal_places, x, decimal_places, y, decimal_places, p.z*2, decimal_places, p.x, decimal_places, p.y);
}

void bore(double diameter, double pitch, double depth, bool floor){	//bore a hole of the specified dimensions
	travel(-diameter/2,0,0, REL);

	double remaining = depth;
	while(remaining-pitch > 0){
		printf("G3 Z%0.*f I%0.*f;\n", decimal_places, -pitch, decimal_places, diameter/2);
		remaining-=pitch;
	}
	printf("G3 Z%0.*f I%0.*f;\n", decimal_places, -remaining, decimal_places, diameter/2);
	if(floor)printf("G3 I%0.*f;\n", decimal_places, diameter/2);
	travel(0,0,depth, REL);
}

void drill(point *p, int len, double depth){	//drill all points in array
	for(int i=0; i<len; i++){
		travel(p[i].x, p[i].y, p[i].z, ABS);
		move(0, 0, -depth, REL);
		travel(p[i].x, p[i].y, p[i].z, ABS);
	}
}

void peck(point *p, int len, double depth, double pecks){	//drill all points in array by pecking n times
	for(int i=0; i<len; i++){
		travel(p[i].x, p[i].y, p[i].z, ABS);
		printf("G83 Z%0.*f R%0.*f Q%0.*f;\n", decimal_places, p[i].z - depth, decimal_places, p[i].z, decimal_places, depth/pecks);
		printf("G80;\n");
	}
}

void advanced_peck(point *p, int len, double depth, double dwell, double first_peck_depth, double pecks_depth, double pecks_reduction, double min_peck){	//drill all points in array by pecking n times
	for(int i=0; i<len; i++){
		travel(p[i].x, p[i].y, p[i].z, ABS);
		printf("G83 Z%0.*f R%0.*f P%0.*f I%0.*f Q%0.*f J%0.*f K%0.*f;\n", decimal_places, p[i].z - depth, decimal_places, p[i].z, decimal_places, dwell, decimal_places, first_peck_depth, decimal_places, pecks_depth, decimal_places, pecks_reduction, decimal_places, min_peck);
		printf("G80;\n");
	}

}

void profile(point *p, int len, bool relative){ //goes to all points in array with cutting feedrate
	for(int i=0; i<len; i++){
		move(p[i].x, p[i].y, p[i].z, relative);
	}
}

void face(double x, double y, double pitch, double angle){	//face the stock at current height with angle relative to positive y
	if(angle == 90){
		for(int i=0; i<=y/pitch; i++){
			if(i%2 == 0)move(x, 0, 0, REL);
			else move(-x, 0, 0, REL);
			if(i<=(x/pitch-1))move(0, pitch, 0, REL);
		}
	}
	else if(angle == 0){
		for(int i=0; i<=x/pitch; i++){
			if(i%2 == 0) move(0, y, 0, REL);
			else move(0, -y, 0, REL);
			if(i<=(y/pitch-1))move(pitch, 0, 0, REL);
		}
	}else if(angle>0 && angle<90){
		printf(";Angled facing not supported yet\n");
	}
}
