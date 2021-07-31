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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libccam.h"

const char header[] = "//This file was automatically generated from a .drl file\n"
											 "//Please verify results with a simulator before runnig on a machine\n"
											 "\n"
											 "//Include this file or copy arrays into you project\n";
 
const char footer[] = "//END OF FILE\n";

int main(int argc, char *argv[]){

	if(argc<2){
		printf("No file provided %d\n", argc);
		exit(-1);
	}

	char * drill_file_name = NULL;

	for(int i=1; i<argc; i++){
		if(strstr(argv[i], ".drl")) drill_file_name = argv[i];
	}
	
	if(NULL == drill_file_name){
		printf("No Exellon drill file provided(.drl)\n");
		exit(-1);
	}

	FILE *drill_file = fopen(drill_file_name, "r");
	if(NULL == drill_file){
		printf("Could not open file %s\n", drill_file);
		exit(-1);
	}

	char *array_file_name = (char *)malloc(strlen(drill_file_name));
	//change filetype
	sprintf(array_file_name, "%.*sh",strlen(drill_file_name) - 3, drill_file_name);
	FILE *array_file = fopen(array_file_name, "w");
	free(array_file_name);
	if(NULL == array_file){
		printf("Could not open file %s\n", array_file);
		exit(-1);
	}

	char line[256];
	bool found_first_tool = false;
	float x,y;
	char *pend;

	fprintf(array_file, "%s", header);

	while(NULL != fgets(line, sizeof(line), drill_file))
	{
		//remove comments
		char * comment = strstr(line, ";");
		if(NULL != comment) comment[0] = 0;
		
		//check if we have something on the remaining line
		if(strlen(line) < 2) continue;

		switch (line[0])
		{
			case 'T':	//tool change
				if(true == found_first_tool)
					//close previous tool
					fprintf(array_file, "};\n\n");
				else if('C' == line[2])
					//skip parameter
					break;
				else
					found_first_tool = true;

				//check for last tool
				if('0' == line[1]) break;

				//create new array
				fprintf(array_file, "const point tool_%c[] = {\n", line[1]);
				break;
			case 'X': //coordinate
				x = strtof(&line[1], &pend);
				y = strtof(&pend[1], &pend);
				//printf("X%f, Y%f\n", x, y);
				fprintf(array_file, "\t{%g, %g},\n", x, y);
				break;
		}

	}

	fprintf(array_file, "%s", footer);

	fclose(drill_file);
	fclose(array_file);

	printf("Done!\n");

	return 0;
}
