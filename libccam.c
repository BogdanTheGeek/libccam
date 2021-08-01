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

#include "types.h"
#include "libccam-svg.h"
#include "libccam.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI M_PI

//variables
static int decimal_places = 3;
static unsigned int segments = 30;

//private functions
double mag(point p){
	return sqrt(p.x*p.x + p.y*p.y);
}

//public functions
void set_curve_segments(unsigned int s){
	segments = s;
}

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

void dwell(int millis){
	printf("G4 P%d;\n", millis);
}

void change_tool(int tool){
	printf("T %d;\n", tool);
	printf("M6;\n");
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
	printf("G91 ");
	if(deg<0){
		printf("G2 ");
		//deg = -deg;
	}else{
		printf("G3 ");
	}
	double x, y, ux, uy;

	deg = (deg * PI) / 180;	//convert to rads
	
	double p_len = mag(p);	

	ux = p.x/p_len;
	uy = p.y/p_len;
	deg += atan2(-uy, -ux); //rotate the circle to match center vector 

	x = p.x + cos(deg)*p_len;
	y = p.y + sin(deg)*p_len;

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
		switch (p[i].type){
		case LINE:
			move(p[i].x, p[i].y, p[i].z, relative);
			break;
		case ARC:
			arc(p[i], *(double*)p[i].opt);
			break;
		case Q_BZ:
			;	//cannot have decalration first
			point *qseg = (point *)malloc(sizeof(point)*segments);

			q_seg(qseg, segments, (point *)p[i].opt);

			if(relative == REL)make_relative(qseg, segments);
			profile(qseg, segments, relative); //using recursion to convert line segments to gcode
			free(qseg);
			break;
		case C_BZ:
			;	//cannot have decalration first
			point *cseg = (point *)malloc(sizeof(point)*segments);
			c_seg(cseg, segments, (point *)p[i].opt);
			if(relative == REL)make_relative(cseg, segments);
			profile(cseg, segments, relative);	//using recursion to convert line segments to gcode
			free(cseg);
			break;
		}
	}
}

void face(double x, double y, double pitch, double angle){	//face the stock at current height with angle relative to positive y
	if(angle == 0){	//simple horizontal pattern
		for(int i=0; i<=y/pitch; i++){
			if(i%2 == 0)move(x, 0, 0, REL);
			else move(-x, 0, 0, REL);
			if(i<=(x/pitch-1))move(0, pitch, 0, REL);
		}
	}
	else if(angle == 90){	//simple vertical pattern
		for(int i=0; i<=x/pitch; i++){
			if(i%2 == 0) move(0, y, 0, REL);
			else move(0, -y, 0, REL);
			if(i<=(y/pitch-1))move(pitch, 0, 0, REL);
		}
	}else if(angle>0 && angle<90){	//protect delicate trig functions from negative numbers :P
		double x0, y0, x1, y1;		// 2 points needed to make a line function
		angle = (angle * PI) / 180;	//convert to rads
		double m = -tan(angle);		//the gradient of the line function is the tan of the angle, the '-' is there to make the angle relative to x not y

		double h = pitch / sin(angle);	//horizontal pitch
		double v = pitch / cos(angle);	//vertical pitch

		//standard linear algebra to get points on the line
		if(h>x) x0 = x;
		else x0 = h;
		y0 = 0;
		x1 = 0;
		y1 = -m*x0 + y0;
		double c = y1;
		
		move(x0, 0, 0, REL);

		while(x1 < x && y0 < y){	//while inside defined area

			if(y1 > y){			// if y outside area, find a new point on the perimeter
				x1 = (y-c)/m;
				y1 = y;
			}

			move(x1-x0, y1-y0, 0, REL);
			
			if(y1 < y && x1 == 0){	//if on left wall
				move(0, v, 0, REL);	//move up
				y1 += v;
			}
			else if(x1+h < x){		//if on top wall and have room
				move(h, 0, 0, REL);	//move right
				x1 += h;
			}else if(x1 < x){		//if no room left, move to right wall
				move(x-x1, 0, 0, REL);
				x1 = x;
			}
			
			c = y1 - m*x1;	//find new intersection to y-axis
			x0 = (y0-c)/m;

			if(x0 > x){		//if outside of defined area
				y0 = m*x + c;	//move to edge
				x0 = x;			//find point on edge
			}
				
			move(x0-x1, y0-y1, 0, REL);

			if(x0 < x){		//if within width
				move(h, 0, 0, REL);	//move right
				x0 += h;
			}
			else if(y0+v < y){	//if we have room
				move(0, v, 0, REL);	//move up
				y0 += v;
			}else if(y0 < y){	//if we don't have room
				move(0, y-y1, 0, REL);
				y0 = y;
			}
			
			//get next point
			c = y0 - m*x0;
			y1 = -m*x0 + y0;
			x1 = (y1-c)/m;

		}
	}	
}

