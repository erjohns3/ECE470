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
	const int maxBlocks = 6;
	const int labelStart = maxBlocks;
	int label = labelStart;
	int labelHist[labelSize];
	int equiv[labelSize];
	for(int i=0; i<labelSize; i++){
		labelHist[i] = 0;
		equiv[i] = -1;
	}
	int blocks[maxBlocks];
	int centerR[maxBlocks];
	int centerC[maxBlocks];
	int sumR[maxBlocks];
	int sumC[maxBlocks];
	for(int i=0; i<maxBlocks; i++){
		blocks[i] = 0;
		centerR[i] = 0;
		centerC[i] = 0;
		sumR[i] = 0;
		sumC[i] = 0;
	}
	int prevRow = -1;
	int prevCol = -1;
	int min = 180;										//------------------need to change-------------------
	int max = 280;										//------------------need to change-------------------
	int numBlocks = 0;
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
				else
					pixellabel[row][col] = label;
				labelHist[pixellabel[row][col]]++;
				if(prevRow>0 && prevCol>0 && prevRow!=prevCol){
					if(prevRow < prevCol){
						for(int jrow=0; jrow<height; jrow++){
							for(int jcol=0; jcol<width; jcol++){
								if (pixellabel[jrow][jcol]==prevCol){
									pixellabel[jrow][jcol] = prevRow;
								}
							}
						}
						labelHist[prevRow] += labelHist[prevCol];
						labelHist[prevCol] = 0;
					}
					else if(prevCol < prevRow){
						for(int jrow=0; jrow<height; jrow++){
							for(int jcol=0; jcol<width; jcol++){
								if (pixellabel[jrow][jcol]==prevRow){
									pixellabel[jrow][jcol] = prevCol;
								}
							}
						}
						labelHist[prevCol] += labelHist[prevRow];
						labelHist[prevRow] = 0;
					}
				}
				for(int i=labelStart; i<labelSize; i++){
					if(labelHist[i]==0){
						label = i;
						break;
					}
				}
			}
		}
	}

	// determine number of objects in the image
	// specify a unique color for each object
	// assign color to all pixels in each objet

	for(int i=labelStart; i<labelSize; i++){
		if(labelHist[i] > min && labelHist[i] < max){
			blocks[numBlocks] = i;
			numBlocks++;
			if(numBlocks >= maxBlocks){
				break;
			}
		}
	}

	for(int row=0; row<height; row++){
		for(int col=0; col<width; col++){
			for(int i=0; i<maxBlocks; i++){
				if(pixellabel[row][col]==blocks[i]){
					pixellabel[row][col] = i;
					labelHist[i]++;
					break;
				}
			}
		}
	}
	
	// assign UNIQUE color to each object
	for(int row=0; row<height; row++)
	{
		for(int col=0; col<width; col++)
		{
			switch (pixellabel[row][col]+1)
			{
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
	
	for(int row=0; row<height; row++){
		for(int col=0; col<width; col++){
			for(int i=0; i<numBlocks; i++){
				if(pixellabel[row][col]==i){
					sumR[i]+=row;
					sumC[i]+=col;
				}
			}
		}
	}

	for(int i=0; i<numBlocks; i++){
		centerR[i] = sumR[i]/labelHist[i];
		centerC[i] = sumC[i]/labelHist[i];
	}

	for(int row=0; row<height; row++){
		for(int col=0; col<width; col++){
			for(int i=0; i<numBlocks; i++){
				if((row==centerR[i] && col>centerC[i]-20 && col<centerC[i]+20) || (col==centerC[i] && row>centerR[i]-20 && row<centerR[i]+20)){
					rgbaddress=pfirstassociatedrgb+row*width+col;
					*(rgbaddress)=qRgb(0,0,0);
				}
			}
		}
	}
	
	console_printf("number of blocks: %i, c0: %d,%d, c1: %d,%d, c2: %d,%d, c3: %d,%d, c5: %d,%d, c5: %d,%d, ", numBlocks, centerR[0], centerC[0], centerR[1], centerC[1], centerR[2], centerC[2], centerR[3], centerC[3], centerR[4], centerC[4], centerR[5], centerC[5]);

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
