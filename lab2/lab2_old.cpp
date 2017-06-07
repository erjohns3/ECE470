//#include "rhino.h"
//#include <stdio.h>

#include <stdio.h>
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

void Move(char Src, char Dst){
	return;
}

void Solve(int N, char Src, char Aux, char Dst){
	if(N == 0)
		return;
	else{
		Solve(N-1, Src, Aux, Dst);
		Move(Src, Dst);
		Solve(N-1, Aux, Src, Dst);
	}
}


void lab_main()
{
	// Move the Rhino back home
	rhino_softhome();

	// Allocate a buffer for the user input
	char buffer[100];
	char src;
	char dest;
	char aux;

	// Read in some input - a simple user interface.

		buffer[0]='\0'; // clear the buffer

		printf("Enter a letter for source:");
		fgets(buffer, 100, stdin);
		src=sscanf_s(buffer, "%d", &src);

				printf("Enter a letter for destination:");
		fgets(buffer, 100, stdin);
		dest=sscanf_s(buffer, "%d", &dest);

				printf("Enter a letter for aux:");
		fgets(buffer, 100, stdin);
		aux=sscanf_s(buffer, "%d", &aux);
	//C4
	int posC4[] = {11,29,-428,600,76};
	//C3
	int posC3[] = {5,2,-467,673,75};
	//C3 drop
	int posC3drop[] = {1,-6,-427,653,77};
	//C2
	int posC2[] = {5,3,-518,728,75};
	//C2 drop
	int posC2drop[] = {0,8,-489,693,77};
	//C1
	int posC1[] = {5,3,-554,788,75};
	//C1 drop
	int posC1drop[] = {-1,3,-547,784,72};

	//B4
	int posB4[] = {12,37,-437,577,-27};
	//B3
	int posB3[] = {4,3,-500,638,-31};
	//B2
	int posB2[] = {3,1,-547,705,-31};
	//B2 drop
	int posB2drop[] = {1,9,-520,680,-34};
	//B1
	int posB1[] = {5,-7,-572,774,-32};

	//B4
	int posA4[] = {2,28,-420,613,-101};
	//A3
	int posA3[] = {1,27,-478,665,-97};
	//A2
	int posA2[] = {0,26,-523,720,-94};
	
	//A1
	int posA1[] = {1,25,-566,783,-94};

	/*
	int i;
	for(i=0; i<choice; i++)
	{
		// Move the waist back and forth "choice" times.
		rhino_move('F', 200);
		rhino_move('F', -200);
	}
	*/

	//src to dest
	rhino_ammove(posA4);
	rhino_ammove(posA3);
	rhino_grip();
	rhino_ammove(posA4);
	rhino_ammove(posC4);
	rhino_ammove(posC1drop);
	rhino_ungrip();
	rhino_ammove(posC4);
	//src to aux
	rhino_ammove(posA4);
	rhino_ammove(posA2);
	rhino_grip();
	rhino_ammove(posA4);
	rhino_ammove(posB4);
	rhino_ammove(posB1);
	rhino_ungrip();
	rhino_ammove(posB4);
	//dest to aux
	rhino_ammove(posC4);
	rhino_ammove(posC1);
	rhino_grip();
	rhino_ammove(posC4);
	rhino_ammove(posB4);
	rhino_ammove(posB2drop);
	rhino_ungrip();
	rhino_ammove(posB4);
	//src to dest
	rhino_ammove(posA4);
	rhino_ammove(posA1);
	rhino_grip();
	rhino_ammove(posA4);
	rhino_ammove(posC4);
	rhino_ammove(posC1drop);
	rhino_ungrip();
	rhino_ammove(posC4);
	//aux to src
	rhino_ammove(posB4);
	rhino_ammove(posB2);
	rhino_grip();
	rhino_ammove(posB4);
	rhino_ammove(posA4);
	rhino_ammove(posA1);
	rhino_ungrip();
	rhino_ammove(posA4);
	//aux to dest
	rhino_ammove(posB4);
	rhino_ammove(posB1);
	rhino_grip();
	rhino_ammove(posB4);
	rhino_ammove(posC4);
	rhino_ammove(posC2drop);
	rhino_ungrip();
	rhino_ammove(posC4);
	//src to dest
	rhino_ammove(posA4);
	rhino_ammove(posA1);
	rhino_grip();
	rhino_ammove(posA4);
	rhino_ammove(posC4);
	rhino_ammove(posC3drop);
	rhino_ungrip();
	rhino_ammove(posC4);



	// Move the Rhino back home
	rhino_softhome();

	// Go back to the main user interface
	return;
}

