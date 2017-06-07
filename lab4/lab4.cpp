#include <math.h>
#include <stdio.h>
#include "rhino.h"
#include "labs.h"
//#include "lab3.cpp"

void lab_banner()
{
	printf("Robotics Lab 4: Inverse Kinematics\n");
	printf("Lab partners: Barney Rubble, Fred Flintstone\n");
}

/* Use care when working with angles on this lab.
 * The angles coming in are in DEGREES.
 * The angles used by the trig functions are in RADIANS.
 * The angles going out are in DEGREES.
 */

int lab_movex(float xworld, float yworld, float zworld, float pitch, float roll)
// Computes of the the elbow-up inverse kinematics solutions
{
	
	double x,y,z,theta5,theta4,theta3,theta2,theta1,wx,wy,wz; 
	
	// First, convert xworld, yworld, zworld to coordinates in the base frame
	x= xworld+9.5;
	y= yworld-14.5;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         ;
	z= zworld;

	// Now the wrist roll
	theta5 = roll;
	
	// the waist angle
	theta1 = 180/M_PI*atan2(y,x);
	
	// Now the wrist center
	wx= x-(0.9*cos(pitch*M_PI/180)+15.5*sin(pitch*M_PI/180))*cos(theta1*M_PI/180);
	wy= y-(0.9*cos(pitch*M_PI/180)+15.5*sin(pitch*M_PI/180))*sin(theta1*M_PI/180);
	wz= z+0.9*sin(pitch*M_PI/180)+15.5*cos(pitch*M_PI/180);

	double r = sqrt(pow(wx,2)+pow(wy,2));
	double R = sqrt(pow(wx,2)+pow(wy,2)+pow((wz-27.7),2));

	// The arm angles
	theta2= 180/M_PI*(acos(R/45.6) + atan2(wz-27.7,r));
	theta3= 180/M_PI*acos((pow(R,2)-1039.7)/(-1039.7)) - 180;

	// Finally, the wrist angle
	theta4= pitch-theta2-theta3;

	// View values
	printf("\n");
	printf("theta1 = %f degrees\n",theta1);
	printf("theta2 = %f degrees\n",theta2);
	printf("theta3 = %f degrees\n",theta3);
	printf("theta4 = %f degrees\n",theta4);
	printf("theta5 = %f degrees\n",theta5);

	float enc_B, enc_C, enc_D, enc_E, enc_F;      // desired encoder values

	enc_B = 3.22*(theta5+9.32);
	enc_C = 8.76*((theta4)+theta3+theta2);
	enc_D = 8.76*((theta3-2.75)+theta2);
	enc_E = -8.76*(theta2-124.2);
	enc_F = 4.38*theta1;

	printf("Computed encoder values:\nB:%f \nC:%f \nD:%f \nE:%f \nF:%f\n\n",enc_B,enc_C,enc_D,enc_E,enc_F);
	
	return rhino_mmove((int)enc_B, (int)enc_C, (int)enc_D, (int)enc_E, (int)enc_F);
	
	// check that your values are good BEFORE uncommenting the following line.
	//return lab_angles((float)theta1,(float)theta2,(float)theta3,(float)theta4,(float)theta5);
	
}