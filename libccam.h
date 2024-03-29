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
#ifndef LIBCCAM_H
#define LIBCCAM_H

#include "types.h"


//prototypes
void set_curve_segments(unsigned int s);

void set_feed(double f);	//set feedrate for cutting
void set_rapid(double f);	//set feedrate for travel

void pause();	//pause the program until manual resume
void stop();	//end the program
void dwell(int millis);	//wait for specified millis
void change_tool(int tool);

void make_relative(point *p, int len);	//convert absolute shape into relative shape
void make_absolute(point *p, int len);	//convert relative shape into absolute shape

void move(double x, double y, double z, bool relative);	//move with cutting feedrate
void travel(double x, double y, double z, bool relative);	//move with rapid feedrate

void arc(point p, double deg);	//move in an arc around point p for deg degrees

void bore(double diameter, double pitch, double depth, bool floor);	//bore a hole of the specified dimensions
void drill(point *p, int len, double depth);	//drill all points in array
void peck(point *p, int len, double depth, double pecks);	//drill all points in array by pecking n times
void advanced_peck(point *p, int len, double depth, double dwell, double first_peck_depth, double pecks_depth, double pecks_reduction, double min_peck);	//drill all points in array by pecking n times

void profile(point *p, int len, bool relative); //goes to all points in array with cutting feedrate
void face(double x, double y, double pitch, double angle);	//face the stock at current height with angle relative to positive y


#endif //LIBCCAM_H
