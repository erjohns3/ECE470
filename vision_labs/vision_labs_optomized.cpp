#include "labs.h"
#include "console.h"
#include <stdio.h>

// -----------------------------------------------------------------------------------------
// You will edit the first two functions in this file (thresholdImage and associateObjects)
// for lab 5.  The functions at the bottom of this file are not to be edited until lab 6.
// -----------------------------------------------------------------------------------------

int DEBUG_LABELS=false; // if set to "true" will generate a text file in your directory displaying the contents of the pixel label array 

void thresholdImage(QImage *gray, QImage *threshed)
// For use with Lab 5
// Take a grayscale bitmap and threshold it.
// The image pointer has dimension width by height pixels.
{
	int   totalpixels;
	uchar graylevel;							// think of the uchar datatype as an integer that occupies only one byte in memory.
	uchar *pfirstgraybyte, *pgraybyte; 
	QRgb  *pfirstthreshedrgb;
	int H[256];

	for(int i=0; i<256; i++)
		H[i] = 0;

	totalpixels	  = gray->numBytes()/4;			// total number of pixels in image
	pfirstgraybyte	  = gray->bits();			// address of first byte of pixel data in image gray
	pfirstthreshedrgb = (QRgb*)threshed->bits();// address of first RGBA triplet in image threshed

	int zt; // threshold grayscale value 

	// 1. build a histogram for the gray image

	for(int i=0; i<totalpixels; i++){
		graylevel = *(pfirstgraybyte + i*4);
		H[graylevel]++;
	}

	// 2. select value of zt that minimizes the within-group variance

	float maxVar = -1;

	for(int i=0; i<256; i++){
		float N1 = 0;
		float u1 = 0;
		float q1 = 0;
		float N2 = 0;
		float u2 = 0;

		for(int j=0; j<=i; j++)
			N1 += H[j];

		N2 = (float)totalpixels - N1;

		for(int j=0; j<=i; j++)
			u1 += j*H[j]/N1; 
		
		q1 = N1/(float)totalpixels;
		
		for(int j=i+1; j<256; j++)
			u2 += j*H[j]/N2;
		
		float curVar = q1*(1-q1)*pow((u1-u2),2);

		if(curVar > maxVar){
			maxVar = curVar;
			zt = i;
		}
	}
	
	// threshold the image
	for(int i=0; i<totalpixels; i++)
	{
		pgraybyte = pfirstgraybyte+i*4;
		graylevel = *(pgraybyte);		
		if(graylevel>zt) *(pfirstthreshedrgb+i)=0xffffffff; // set rgb to 255 (white)
		else             *(pfirstthreshedrgb+i)=0x00000000; // set rgb to 0   (black)
	}
}

void associateObjects(QImage *threshed, QImage *associated)
// For use with Lab 5
// Assign a unique color to each object in the thresholded image,
// and change all pixels in each object to the color.
{
	int height,width;
	int red, green, blue;
	QRgb *pfirstthreshedrgb, *pfirstassociatedrgb, *rgbaddress;
	QRgb pixel;

	height = threshed->height();
	width = threshed->width();
	pfirstthreshedrgb=(QRgb*)threshed->bits();
	pfirstassociatedrgb=(QRgb*)associated->bits();
	
	// initialize an array of labels, assigning a label number to each pixel in the image
	int ** pixellabel = new int*[height];
	for (int i=0;i<height;i++) {
		pixellabel[i] = new int[width];
	}
	for(int row=0; row<height; row++)
	{
		for(int col=0; col<width; col++)
		{
			// read red, green, blue values of pixel in threshed image
			rgbaddress=pfirstthreshedrgb+row*width+col;
			pixel = *(rgbaddress);
			red   = qRed(pixel);			// note: r=g=b= {255 or 0}
			
			if (red==0x00)		pixellabel[row][col]=0;		// object/foreground
			else				pixellabel[row][col]=-1;	// background
		}
	}
	
	// prints the array of pixel labels to a text file RELEASE/DEBUGFILE.TXT for you to view
	if (DEBUG_LABELS)
	{
		console_printf("generating label debugging text file RELEASE/DEBUGFILE.TXT");
		FILE *debugfile;
		fopen_s(&debugfile,"debugfile.txt","w");
		for(int row=0; row<height; row++)
			{
			for(int col=0; col<width; col++)
				fprintf(debugfile,"%3d",pixellabel[row][col]);
			fprintf(debugfile,"\n");
			}		
		fclose(debugfile);
	}
	
	// assign the same label to all pixels in each object, a unique label for each object

	
	const int labelSize = 4096;
	const int blockAmount = 6;
	const int labelStart = blockAmount+1;
	int label = labelStart;
	int labelHist[labelSize];
	int equiv[labelSize];
	for(int i=0; i<labelSize; i++){}
		labelHist[i] = 0;
		equiv[i] = -1;
	}
	int blocks[blockAmount];
	for(int i=0; i<blockAmount; i++)
		blocks[i] = 0;
	int prevRow = -1;
	int prevCol = -1;
	int min = 150;										//------------------need to change-------------------
	int max = 250;										//------------------need to change-------------------
	int num = 0;
	int rootFinder;

	for(int row=0; row<height; row++){
		for(int col=0; col<width; col++){
			if(pixellabel[row][col]==0){
				if(row>0)
					prevRow = pixellabel[row-1][col];
				if(col>0)
					prevCol = pixellabel[row][col-1];
				if(prevRow>0)
					pixellabel[row][col] = prevRow;
				else if(prevCol>0)
					pixellabel[row][col] = prevCol;
				else{
					pixellabel[row][col] = label;
					label++;
				}
				labelHist[pixellabel[row][col]]++;
				if(prevRow>0 && prevCol>0 && prevRow!=prevCol){
					if(prevRow < prevCol){
						equiv[prevCol] = prevRow;
					}else if(prevCol < prevRow){
						equiv[prevRow] = prevCol;
					}
				}
			}
		}
	}

	for(int row=0; row<height; row++){
		for(int col=0; col<width; col++){
			if(pixellabel[row][col]>0){
				rootFinder = equiv[pixellabel[row][col]];
				while(rootFinder > -1){
					labelHist[pixellabel[row][col]]--;
					pixellabel[row][col] = rootFinder;
					labelHist[pixellabel[row][col]]++;
					rootFinder = equiv[rootFinder];
				}
			}
		}
	}

	// determine number of objects in the image
	// specify a unique color for each object
	// assign color to all pixels in each objet

	for(int i=labelStart; i<labelSize; i++){
		if(labelHist[i] > min && labelHist[i] < max){
			blocks[num] = i;
			num++;
		}
	}

	console_printf("number of blocks: %i", num);

	for(int row=0; row<height; row++){
		for(int col=0; col<width; col++){
			for(int i=0; i<blockAmount; i++){
				if(pixellabel[row][col]==blocks[i]){
					pixellabel[row][col] = i+1;
					break;
				}
			}
		}
	}
	
	// assign UNIQUE color to each object
	for(int row=0; row<height; row++){
		for(int col=0; col<width; col++){
			switch (pixellabel[row][col]){
				case 1:
					red   = 255;
					green = 0;
					blue  = 0;
					break;
				case 2:
					red   = 0;
					green = 255;
					blue  = 0;
					break;
				case 3:
					red   = 0;
					green = 0;
					blue  = 255;
					break;
				case 4:
					red   = 255;
					green = 255;
					blue  = 0;
					break;
				case 5:
					red   = 255;
					green = 0;
					blue  = 255;
					break;
				case 6:
					red   = 0;
					green = 255;
					blue  = 255;
					break;
				default:
					red   = 255;
					green = 255;
					blue  = 255;
					break;					
			}
			rgbaddress=pfirstassociatedrgb+row*width+col;
			*(rgbaddress)=qRgb(red,green,blue);
		}
	}

	// clean up memory
	for (int i=0;i<height;i++)
		delete pixellabel[i];
    delete pixellabel;
}

// -----------------------------------------------
// The functions below will be used with lab 6
// You may ignore them while working on lab 5
// -----------------------------------------------


void lab_pick(int row, int column, QRgb pix)
// For use with Lab 6.
// Pick up a block by clicking on the object in the processed image.
{
	console_printf("Picking up the block at (%d,%d); color is (%d,%d,%d)",
		row, column, qRed(pix), qGreen(pix), qBlue(pix));
}


void lab_place(int row, int column, QRgb pix)
// For use with Lab 6
// If the robot is holding a block, place it at the designated row and column.
{
	console_printf("Placing the block at (%d,%d); color is (%d,%d,%d)",
		row, column, qRed(pix), qGreen(pix), qBlue(pix));
}


void lab_main()
{
	// nothing here; leave it blank!
}


void lab_help()
{
	console_printf("find   get color information for a pixel in any of the four images\n");
	console_printf("pick   click on an object to direct the Rhino to pick it up\n");
	console_printf("place  click on a location for the Rhino to place the object in its gripper\n");
}
