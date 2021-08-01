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
#ifndef TYPES_H
#define TYPES_H

// the funniest way to implement bool in C
typedef enum
{
    false = ( 1 == 0 ),
    true = ( ! false )
} bool;

typedef enum {
	 LINE = 0,
	 ARC = 1,
	 C_BZ = 2,
	 Q_BZ = 4
} point_type;

enum{
	ABS = 0, //absolute coordinates
	REL = 1	//relative coordinates
};

typedef struct point{
	double x;
	double y;
	double z;
	point_type type;
	void * opt;
}point;

#endif //TYPES_H