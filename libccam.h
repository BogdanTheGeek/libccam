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

// the funniest way to implement bool in C
typedef enum
{
    false = ( 1 == 0 ),
    true = ( ! false )
} bool;

typedef struct point{
	double x;
	double y;
	double z;
}point;

//prototypes
void set_feed(double f);	//set feedrate for cutting
void set_rapid(double f);	//set feedrate for travel

void move(double x, double y, double z, bool relative);	//move with cutting feedrate
void travel(double x, double y, double z, bool relative);	//move with rapid feedrate

void bore(double diameter, double pitch, double depth, bool floor);	//bore a hole of the specified dimensions
void drill(point *p, int len, double depth);	//drill all points in array
void peck(point *p, int len, double depth, double pecks, double ratio);	//drill all points in array by pecking n times, the depth before a retract is controlled by ratio

void profile(point *p, int len, bool relative); //goes to all points in array with cutting feedrate
