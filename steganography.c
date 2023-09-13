/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	//YOUR CODE HERE
    Color *color = (Color*) malloc(sizeof(Color));
    if (color == NULL) {
        printf("fail to alloc memory.\n");
        return NULL;
    }
    uint8_t channelBlue = image->image[row][col].B;
    uint8_t setWhite = (channelBlue & 1) ? 255 : 0; 
    color->R = setWhite;
    color->G = setWhite;
    color->B = setWhite;
    return color;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	//YOUR CODE HERE

    Image *steganoImage = (Image*) malloc(sizeof(Image));
    steganoImage->cols = image->cols;
    steganoImage->rows = image->rows;
    steganoImage->image = malloc(image->rows * sizeof(Color*)); 
    if (steganoImage->image == NULL) {
        printf("fail to alloc memory.\n");
        return NULL;
    }
    for (uint32_t i = 0; i < image->rows; i++) {
		steganoImage->image[i] = (Color*)malloc(image->cols * sizeof(Color));
        if (steganoImage->image[i] == NULL) {
            printf("fail to alloc memory.\n");
            return NULL;
        }
        for (uint32_t j = 0; j < image->cols; j++) {
            Color *undertune = evaluateOnePixel(image, i, j);
            steganoImage->image[i][j].R = undertune->R;
            steganoImage->image[i][j].G = undertune->G;
            steganoImage->image[i][j].B = undertune->B;
            free(undertune);
        }
    }
    return steganoImage;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
    Image *image = readData(argv[1]);
    Image *steganofile = steganography(image);
    writeData(steganofile);
    freeImage(steganofile);
    freeImage(image);
}
