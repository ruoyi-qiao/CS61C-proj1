/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	//YOUR CODE HERE
    Color *color = (Color*) malloc(sizeof(Color));
    if (color == NULL) {
        printf("fail to alloc memory.\n");
        return NULL;
    }
    color->B = 0;
    color->R = 0;
    color->G = 0;

    for(int i = 0; i < 8; i++) {
        uint32_t stateRed = (image->image[row][col].R >> i) & 1;
        uint32_t stateBlue = (image->image[row][col].B >> i) & 1;
        uint32_t stateGreen = (image->image[row][col].G >> i) & 1;
        int neighborAlive = 0;
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue;
                int curRow = (row + dx + image->rows) % image->rows;
                int curCol = (col + dy + image->cols) % image->cols;
                if (((image->image[curRow][curCol].R) >> i) & 1) {
                    neighborAlive++;
                }
            }
        }
        if (stateRed)
            color->R |= ((1) & (rule >> (neighborAlive + 9))) << i;
        else 
            color->R |= ((1) & (rule >>  (neighborAlive))) << i;
        neighborAlive = 0;
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue;
                int curRow = (row + dx + image->rows) % image->rows;
                int curCol = (col + dy + image->cols) % image->cols;
                if (((image->image[curRow][curCol].B) >> i) & 1) {
                    neighborAlive++;
                }
            }
        }
        if (stateBlue)
            color->B |= ((1) & (rule >> (neighborAlive + 9))) << i;
        else 
            color->B |= ((1) & (rule >>  (neighborAlive))) << i;
        neighborAlive = 0;
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue;
                int curRow = (row + dx + image->rows) % image->rows;
                int curCol = (col + dy + image->cols) % image->cols;
                if (((image->image[curRow][curCol].G) >> i) & 1) {
                    neighborAlive++;
                }
            }
        }
        if (stateGreen)
            color->G |= ((1) & (rule >> (neighborAlive + 9))) << i;
        else 
            color->G |= ((1) & (rule >>  (neighborAlive))) << i;
    }
    return color;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
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
            Color *undertune = evaluateOneCell(image, i, j, rule);
            assert(undertune != NULL);
            steganoImage->image[i][j].R = undertune->R;
            steganoImage->image[i][j].G = undertune->G;
            steganoImage->image[i][j].B = undertune->B;
            free(undertune);
        }
    }
    return steganoImage;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
    Image *image = readData(argv[1]);
    if (!image) {
        return -1;
    }
    uint32_t rule = strtol(argv[2], NULL, 16);
    Image *steganofile = life(image, rule);
    writeData(steganofile);
    freeImage(steganofile);
    freeImage(image);
}
