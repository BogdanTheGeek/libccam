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

#include "libccam-svg.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


point line_bz(point a, point b, double pos){

	point result;
	result.x = (1 - pos)*a.x + pos*b.x;
	result.y = (1 - pos)*a.y + pos*b.y;

	return result;
}

point quad_bz(point a, point b, point c, double pos){

	point result;

	point a1 = line_bz(a, b, pos);
	point b1 = line_bz(b, c, pos);

	result = line_bz(a1, b1, pos);

	return result;
}

point cube_bz(point a, point b, point c, point d, double pos){
	
	point result;

	point a1 = quad_bz(a, b, c, pos);
	point b1 = quad_bz(b, c, d, pos);


	result = line_bz(a1, b1, pos);

	return result;
}

void q_seg(point *seg, int len, point *p_abc){	//convert quad bezier points to array of segments
	//len determines the "resolution" of the line
	for(int i = 0; i<len; i++){
		
		seg[i] = quad_bz(p_abc[0], p_abc[1], p_abc[2], (double)(i+1) / (double)len);
		seg[i].z = p_abc[0].z;
		seg[i].type = LINE;
	}
}

void c_seg(point *seg, int len, point *p_abcd){	//convert cube bezier points to array of segments
	//len determines the "resolution" of the line
	for(int i = 0; i<len; i++){
		
		seg[i] = cube_bz(p_abcd[0], p_abcd[1], p_abcd[2], p_abcd[3], (double)i / (double)len);
		seg[i].z = p_abcd[0].z;
		seg[i].type = LINE;
	}
}

void print_points(point * p, int len){

	putchar('\n');

	for(int i=0; i<len; i++){
		printf("X:%0.3f Y:%0.3f Z:%0.3f TYPE:%d\n", p[i].x, p[i].y, p[i].z, p[i].type);
		if(p[i].type == C_BZ){
			printf("--Cubic Bezier--");
			point *cbz = p[i].bz_point;
			print_points(cbz, 4);
			printf("--End--\n");
		}
	}
}

int svg_to_array(char *svg_path, point *path){
	
	int path_i = 0;
	int last_path_i = 0;

	char * cursor = svg_path;
	char last_cmd = *cursor;

	while(cursor < svg_path + strlen(svg_path)){
		//putchar(*cursor);
		switch (*cursor){
		case 'm':	//REL move
			last_cmd = *cursor;
			double mx = strtof(cursor+2, &cursor);
			double my = strtof(cursor+1, &cursor);
			path[path_i].x = mx;
			path[path_i].y = my;
			path[path_i].type = LINE;
			path_i++;
			break;
		case 'M':	//ABS move
			last_cmd = *cursor;
			double Mx = strtof(cursor+2, &cursor);
			double My = strtof(cursor+1, &cursor);
			path[path_i].x = Mx;
			path[path_i].y = My;
			path[path_i].type = LINE;
			path_i++;
			break;
		case 'l':	//REL line
			last_cmd = *cursor;
			double lx = strtof(cursor+2, &cursor);
			double ly = strtof(cursor+1, &cursor);
			path[path_i].x = lx;
			path[path_i].y = ly;
			path[path_i].type = LINE;
			path_i++;
			break;
		case 'L':	//ABS line
			last_cmd = *cursor;
			double Lx = strtof(cursor+2, &cursor);
			double Ly = strtof(cursor+1, &cursor);
			path[path_i].x = Lx;
			path[path_i].y = Ly;
			path[path_i].type = LINE;
			path_i++;
			break;
		case 'h':	//REL horizontal line
			last_cmd = *cursor;
			double hx = strtof(cursor+2, &cursor);
			path[path_i].x = hx;
			path[path_i].y = 0;
			path[path_i].type = LINE;
			path_i++;
			break;
		case 'H':	//ABS horizontal line
			last_cmd = *cursor;
			double Hx = strtof(cursor+2, &cursor);
			path[path_i].x = Hx;
			path[path_i].y = path[path_i-1].y;
			path[path_i].type = LINE;
			path_i++;
			break;
		case 'v':	//REL vertical line
			last_cmd = *cursor;
			double vy = strtof(cursor+2, &cursor);
			path[path_i].x = 0;
			path[path_i].y = vy;
			path[path_i].type = LINE;
			path_i++;
			break;
		case 'V':	//ABS vertical line
			last_cmd = *cursor;
			double Vy = strtof(cursor+2, &cursor);
			path[path_i].x = path[path_i-1].x;
			path[path_i].y = Vy;
			path[path_i].type = LINE;
			path_i++;
			break;
		case 'c':	//REL cubic bezier curve
			last_cmd = *cursor;
			path[path_i].type = C_BZ;
			point *cbz = (point *)malloc(sizeof(point)*4);
			//first point is 0,0 as we are in the correct position
			cbz[0].x = 0;
			cbz[0].y = 0;
			cbz[0].z = 0;

			cursor++;	//skip cmd char
			double cx,cy;
			for(int i=1; i<4; i++){
				cx = strtof(cursor+1, &cursor);
				cy = strtof(cursor+1, &cursor);
				cbz[i].x = cx;
				cbz[i].y = cy;
			}
			path[path_i].bz_point = (void *)cbz;
			path_i++;
			break;
		case 'C':	//ABS cubic bezier curve
			last_cmd = *cursor;

			break;
		case 's':	//REL continued cubic bz curve
			last_cmd = *cursor;

			break;
		case 'S':	//ABS continued cubic bz curve
			last_cmd = *cursor;

			break;
		case 'q':	//REL quad bz curve
			last_cmd = *cursor;
			path[path_i].type = Q_BZ;
			point *qbz = (point *)malloc(sizeof(point)*3);
			//first point is 0,0 as we are in the correct position
			qbz[0].x = 0;
			qbz[0].y = 0;
			qbz[0].z = 0;

			cursor++;	//skip cmd char
			double qx,qy;
			for(int i=1; i<3; i++){
				qx = strtof(cursor+1, &cursor);
				qy = strtof(cursor+1, &cursor);
				qbz[i].x = qx;
				qbz[i].y = qy;
			}
			path[path_i].bz_point = (void *)qbz;
			path_i++;

			break;
		case 'Q':	//ABS quad bz curve
			last_cmd = *cursor;

			break;
		case 't':	//REL continued quad bz curve
			last_cmd = *cursor;

			break;
		case 'T':	//ABS continued quad bz curve
			last_cmd = *cursor;

			break;
		case 'a':	//REL arc
			last_cmd = *cursor;

			break;
		case 'A':	//ABS arc
			last_cmd = *cursor;

			break;
		case 'Z':	//close path
		case 'z':
			//cursor++;
			//break;
			path[path_i].x = path[last_path_i+1].x;
			path[path_i].y = path[last_path_i+1].y;
			path[path_i].type = LINE;
			path_i++;
			last_path_i = path_i;
			cursor++;
			break;
		}

		//check if there is a cmd
		if(*cursor == ' ' && *(cursor+2) != ' '){
			//printf("\nUsing last command: %c\n", last_cmd);
			cursor--;	//move back
			*cursor = last_cmd;
			//printf("%s", svg_path);
			continue;	//go back to begining of loop
		}

		cursor++;
	}
	//print_points(path, path_i);

	return path_i;
}
