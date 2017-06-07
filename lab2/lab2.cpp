//#include "rhino.h"
//#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <array>
#include <windows.h>
#include <wchar.h>
#include "rhino.h"
#include "labs.h"

/**
 * This starter file should help you do lab 2.
 *
 * Compile instructions:
 * click the green run button
 * 
 */

/**
 * This function just prints a sample banner when you start the program.
 * If you don't use this function, please have your program print a banner like it.
 */
void lab_banner()
{
	// Sample banner
	printf("Robotics Lab 2: The Tower of Hanoi\n");
	printf("Lab partners: Barney Rubble, Fred Flintstone\n");
}

/**
 * If you want, put a description of what your program does and how to use it in here.
 * (purely optional)
 */
void lab_help()
{
	printf("No instructions available.\n");
}

/**
 * Place your lab demo in this function.
 * When you run the program and type "lab", it will call this function.
 */

void Move(int Src, int Dst, int* size){

	int pos [] = {
				30,0,-348,943,5,		//A1
				30,0,-319,897,5,		//A2
				30,0,-283,854,5,		//A3
				30,0,-145,780,5,		//resetA

				30,0,-511,818,5,		//B1
				30,0,-472,762,5,		//B2
				30,0,-425,713,5,		//B3
				30,0,-300,615,5,		//resetB

				30,0,-656,745,5,		//C1
				30,0,-608,676,5,		//C2
				30,0,-549,610,5,		//C3
				30,0,-340,460,5			//resetC
				};		

	/*


				-35,0,-576,781,-88,		//pickA1
				-35,0,-576,781,-88,		//dropA1
				-35,0,-536,718,-88,		//pickA2
				-35,0,-518,700,-88,		//dropA2
				-35,0,-484,662,-88,		//pickA3
				-35,0,-467,640,-88,		//dropA3
				-35,0,-430,609,-88,		//resetA

				5,-7,-572,774,-32,		//pickB1
				5,-7,-572,774,-32,		//dropB1
				3,1,-547,705,-31,		//pickB2
				1,9,-520,680,-34,		//dropB2
				4,3,-500,638,-31,		//pickB3
				4,3,-500,638,-31,		//dropB3
				12,37,-437,577,-27,		//resetB

				5,3,-554,788,75,		//pickC1
				-1,3,-547,784,72,		//dropC1
				5,3,-518,728,75,		//pickC2
				0,8,-489,693,77,		//dropC2
				5,2,-467,673,75,		//pickC3
				1,-6,-427,653,77,		//dropC3
				11,29,-428,600,76		//resetC

	/////////////////////////////////////////////////////

				1,25,-566,783,-94,		//pickA1
				1,25,-566,783,-94,		//dropA1
				0,26,-523,720,-94,		//pickA2
				0,26,-523,720,-94,		//dropA2
				1,27,-478,665,-97,		//pickA3
				1,27,-478,665,-97,		//dropA3
				2,28,-420,613,-101,		//resetA

				5,-7,-572,774,-32,		//pickB1
				5,-7,-572,774,-32,		//dropB1
				3,1,-547,705,-31,		//pickB2
				1,9,-520,680,-34,		//dropB2
				4,3,-500,638,-31,		//pickB3
				4,3,-500,638,-31,		//dropB3
				12,37,-437,577,-27,		//resetB

				5,3,-554,788,75,		//pickC1
				-1,3,-547,784,72,		//dropC1
				5,3,-518,728,75,		//pickC2
				0,8,-489,693,77,		//dropC2
				5,2,-467,673,75,		//pickC3
				1,-6,-427,653,77,		//dropC3
				11,29,-428,600,76		//resetC
				};		
		*/
	
	int posSrc[5];
	int resetSrc[5];
	int posDst[5];
	int resetDst[5];

	for(int i=0; i<5; ++i){
		posSrc[i] = pos[Src*20+(size[Src]-1)*5 + i];
		resetSrc[i] = pos[Src*20+15 + i];
		posDst[i] = pos[Dst*20+size[Dst]*5 + i];
		resetDst[i] = pos[Dst*20+15 + i];
	}

	/*
	for(int i=0; i<5; ++i){
		posSrc[i] = pos[Src*35+(size[Src]-1)*10 + i];
		resetSrc[i] = pos[Src*35+30 + i];
		posDst[i] = pos[Dst*35+size[Dst]*10+5 + i];
		resetDst[i] = pos[Dst*35+30 + i];
	}
	*/

	rhino_ammove(resetSrc);
	rhino_ammove(posSrc);
	rhino_grip();
	rhino_ammove(resetSrc);
	rhino_ammove(resetDst);
	rhino_ammove(posDst);
	rhino_ungrip();
	rhino_ammove(resetDst);

	size[Src]--;
	size[Dst]++;

	printf(" %d , %d , %d\n", size[0], size[1], size[2]);

	return;
}

void Solve(int N, int* size, int Src, int Aux, int Dst){
	//printf("N %d, Src %d, Aux %d, Dst %d\n", N, Src, Aux, Dst);
	if(N == 0)
		return;
	else{
		Solve(N-1, size, Src, Dst, Aux);
		Move(Src, Dst, size);
		Solve(N-1, size, Aux, Src, Dst);
	}
}


void lab_main()
{
	// Move the Rhino back home
	rhino_softhome();

	// Allocate a buffer for the user input
	char buffer[100];
	char src;
	char dst;

	int Src;
	int Dst;
	int Aux;

	// Read in some input - a simple user interface.

	buffer[0]='\0'; // clear the buffer

	printf("Enter a letter for source:");
	fgets(buffer, 100, stdin);
	sscanf_s(buffer, "%c", &src);

	printf("Enter a letter for destination:");
	fgets(buffer, 100, stdin);
	sscanf_s(buffer, "%c", &dst);

	Src = ((int)src) - 97;
	Dst = ((int)dst) - 97;
	Aux = 3-Src-Dst;

	int* size = new int[3];
	
	size[Src] = 3; //src
	size[Dst] = 0; //dst
	size[Aux] = 0; //aux

	printf(" %d , %d , %d\n", size[0], size[1], size[2]);

	Solve(3, size, Src, Aux, Dst);

	/*
	int resetC[] = {11,29,-428,600,76};		//resetC
	int dropC3[] = {1,-6,-427,653,77};		//dropC3
	int pickC3[] = {5,2,-467,673,75};		//pickC3
	int dropC2[] = {0,8,-489,693,77};		//dropC2
	int pickC2[] = {5,3,-518,728,75};		//pickC2
	int dropC1[] = {-1,3,-547,784,72};		//dropC1
	int pickC1[] = {5,3,-554,788,75};		//pickC1

	int resetB[] = {12,37,-437,577,-27};	//resetB
	int dropB3[] = {4,3,-500,638,-31};		//dropB3
	int pickB3[] = {4,3,-500,638,-31};		//pickB3
	int dropB2[] = {1,9,-520,680,-34};		//dropB2
	int pickB2[] = {3,1,-547,705,-31};		//pickB2
	int dropB1[] = {5,-7,-572,774,-32};		//dropB1
	int pickB1[] = {5,-7,-572,774,-32};		//pickB1

	int resetA[] = {2,28,-420,613,-101};	//resetA
	int dropA3[] = {1,27,-478,665,-97};		//dropA3
	int pickA3[] = {1,27,-478,665,-97};		//pickA3
	int dropA2[] = {0,26,-523,720,-94};		//dropA2
	int pickA2[] = {0,26,-523,720,-94};		//pickA2
	int dropA1[] = {1,25,-566,783,-94};		//dropA1
	int pickA1[] = {1,25,-566,783,-94};		//dropA1
	

	//src to dest
	rhino_ammove(resetA);
	rhino_ammove(pickA3);
	rhino_grip();
	rhino_ammove(resetA);
	rhino_ammove(resetC);
	rhino_ammove(dropC1);
	rhino_ungrip();
	rhino_ammove(resetC);
	//src to aux
	rhino_ammove(resetA);
	rhino_ammove(pickA2);
	rhino_grip();
	rhino_ammove(resetA);
	rhino_ammove(resetB);
	rhino_ammove(dropB1);
	rhino_ungrip();
	rhino_ammove(resetB);
	//dest to aux
	rhino_ammove(resetC);
	rhino_ammove(pickC1);
	rhino_grip();
	rhino_ammove(resetC);
	rhino_ammove(resetB);
	rhino_ammove(dropB2);
	rhino_ungrip();
	rhino_ammove(resetB);
	//src to dest
	rhino_ammove(resetA);
	rhino_ammove(pickA1);
	rhino_grip();
	rhino_ammove(resetA);
	rhino_ammove(resetC);
	rhino_ammove(dropC1);
	rhino_ungrip();
	rhino_ammove(resetC);
	//aux to src
	rhino_ammove(resetB);
	rhino_ammove(pickB2);
	rhino_grip();
	rhino_ammove(resetB);
	rhino_ammove(resetA);
	rhino_ammove(dropA1);
	rhino_ungrip();
	rhino_ammove(resetA);
	//aux to dest
	rhino_ammove(resetB);
	rhino_ammove(pickB1);
	rhino_grip();
	rhino_ammove(resetB);
	rhino_ammove(resetC);
	rhino_ammove(dropC2);
	rhino_ungrip();
	rhino_ammove(resetC);
	//src to dest
	rhino_ammove(resetA);
	rhino_ammove(pickA1);
	rhino_grip();
	rhino_ammove(resetA);
	rhino_ammove(resetC);
	rhino_ammove(dropC3);
	rhino_ungrip();
	rhino_ammove(resetC);
	*/
	// Move the Rhino back home
	rhino_softhome();

	delete [] size;

	// Go back to the main user interface
	return;
}

