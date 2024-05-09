/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Winter 2024                   */
/* Movie.c: source file for basic movie manipulations                */
/*********************************************************************/

#include <stdlib.h>
#include <assert.h>
#include "Movie.h"

/* Allocate the memory space for the movie and the memory space */
/* for the frame list. Return the pointer to the movie. */
Movie *CreateMovie(void)
{
	Movie *movie = NULL;
	movie = (Movie*) malloc(sizeof(Movie));
	if(movie == NULL)
	{
		printf("Fail to create movie\n");
		return NULL;
	}
	movie->Frames = CreateImageList();
	if (movie->Frames == NULL)
	{
		printf("Fail to create movie frame\n");
		return NULL;
	}
	return movie;
}

/* Release the memory space for the frame list. */
/* Release the memory space for the movie. */
void DeleteMovie(Movie *movie)
{
	assert(movie);
    IENTRY *current, *next;
    
    current = movie->Frames->First;
    while (current != NULL) {
        next = current->Next;
        DeleteImage(current->image); 
        free(current);
        current = next;
    }

    free(movie->Frames);
    free(movie);
}

int clip(int x)
{
	if(x > 255) return 255;
	if(x < 0) return 0;
	return x;
}

/* Convert a YUV movie to a RGB movie */
void YUV2RGBMovie(Movie *movie)
{
	assert(movie);
	assert(movie->Frames);
	IENTRY *current = NULL;
	int C, D, E, R, G, B, Y, U, V;
	int width, height;
	int x, y;

	Image *img = NULL;
	YUVImage *yuvimage = NULL;

	current = movie->Frames->First;
	while(current != NULL)
	{
		yuvimage = current->image;
		width = yuvimage->W;
		height = yuvimage->H;
		img = CreateImage(width, height);
		assert(img); 
		for(x = 0; x < width; x++)
		{
			for(y = 0; y < height; y++)
			{
				Y = GetPixelY(yuvimage, x, y);
				U = GetPixelU(yuvimage, x, y);
				V = GetPixelV(yuvimage, x, y);
				C = Y - 16;
				D = U - 128;
				E = V - 128;
				R = clip(( 298 * C + 409 * E + 128) >> 8);
				G = clip(( 298 * C - 100 * D - 208 * E + 128) >> 8);
				B = clip(( 298 * C + 516 * D + 128) >> 8);
				SetPixelR(img, x, y, R);
				SetPixelG(img, x, y, G);
				SetPixelB(img, x, y, B);
			}
		}
		DeleteYUVImage(yuvimage);
		current->image = img;
		current = current->Next;
	}
}

/* Convert a RGB movie to a YUV movie */
void RGB2YUVMovie(Movie *movie)
{
	assert(movie);
	assert(movie->Frames);
	IENTRY* cur = NULL;
	unsigned char Y, U, V;
	unsigned char R, G, B;
	int x, y;
	int width, height;
	Image* img = NULL;
	YUVImage* yuvimage = NULL;
	cur = movie->Frames->First;
	
	while(cur != NULL)
	{
		img = cur->image;
		width = img->W;
		height = img->H;
		yuvimage = CreateYUVImage(width, height);
		assert(yuvimage);
		for(x = 0; x < width; x++)
		{
			for(y = 0; y < height; y++)
			{
				R = GetPixelR(img, x, y);
				G = GetPixelG(img, x, y);
				B = GetPixelB(img, x, y);
				Y = clip((( 66 * R + 129 * G + 25 * B + 128) >> 8) + 16 );
				U = clip((( -38 * R - 74 * G + 112 * B + 128) >> 8) + 128 );
				V = clip((( 112 * R - 94 * G - 18 * B + 128) >> 8) + 128 );
				SetPixelY(yuvimage, x, y, Y);
				SetPixelU(yuvimage, x, y, U);
				SetPixelV(yuvimage, x, y, V);
			}
		}
		DeleteImage(img);
		cur->image = yuvimage;
		cur = cur->Next;
	}
}

/* EOF */
