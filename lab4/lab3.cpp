#include "rhino.h"
#include <stdio.h>

/**
 * This starter file should help you do lab 3.
 */

#include "rhino.h"
#include <stdio.h>

/**
 * This function just prints a sample banner when you start the program.
 * If you don't use this function, please have your program print a banner like it.
 */



/** 
 * place your comments here
 */
int lab_angles(float theta1, float theta2, float theta3, float theta4, float theta5)
{
	float enc_B, enc_C, enc_D, enc_E, enc_F;      // desired encoder values

	/* c programming note:
     * If the first argument in a division is an INTEGER, c will perform
	 * INTEGER division, eleminating any fractional part of the division.
	 *
	 *      float a;
	 *      a = 5/3;  // assigns the integer 1 to a
	 *
	 * To avoid this problem, type cast the first integer as a float in
	 * one of the two following equivalent ways.
	 *
	 *      float a;
	 *      a = 5.0/3;  	// assigns the value 1.66666666 to a
	 *	a = (float)5/3	// also assigns the value 1.6666666 to a
	 *
	 */

	//Implement joint angle to encoder expressions here.

	enc_B = 3.22*(theta5+9.32);
	enc_C = 8.76*((theta4)+theta3+theta2);
	enc_D = 8.76*((theta3-2.75)+theta2);
	enc_E = -8.76*(theta2-124.2);
	enc_F = 4.38*theta1;

	printf("Computed encoder values:\nB:%f \nC:%f \nD:%f \nE:%f \nF:%f\n\n",enc_B,enc_C,enc_D,enc_E,enc_F);
	
	return rhino_mmove((int)enc_B, (int)enc_C, (int)enc_D, (int)enc_E, (int)enc_F);
}