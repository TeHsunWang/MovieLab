/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Winter 2024                   */
/* DIPs.c: source file for DIP operations                            */
/*********************************************************************/

#include <stdlib.h>
#include <assert.h>
#include "DIPs.h"
#include "Constants.h"
#include "string.h"
#include "math.h"

/* Black and White */
Image *BlackNWhite(Image *image)
{
	assert(image);
	int x, y, tmp;

	for (y = 0; y < ImageHeight(image); y++) {
		for (x = 0; x < ImageWidth(image); x++) {
			tmp = (GetPixelR(image, x, y) + GetPixelG(image, x, y) + GetPixelB(image, x, y)) / 3;
			SetPixelR(image, x, y, tmp);
			SetPixelG(image, x, y, tmp);
			SetPixelB(image, x, y, tmp);
		}
	}
	return image;
}

/* Mirror image horizontal  */
Image *HMirror(Image *image) {
	assert(image);
	int x, y;
	for (y = 0; y < ImageHeight(image); y++) {
		for (x = 0; x < ImageWidth(image)/2; x++) {
			SetPixelG(image, ImageWidth(image) - 1 - x, y, GetPixelG(image, x, y));
			SetPixelB(image, ImageWidth(image) - 1 - x, y, GetPixelB(image, x, y));
			SetPixelR(image, ImageWidth(image) - 1 - x, y, GetPixelR(image, x, y));
		}
	}
	return image;
}

/* Edge detection */
Image *Edge(Image *image) {
	assert(image);
	Image *tmpImage = CreateImage(ImageWidth(image), ImageHeight(image));
	assert(tmpImage);

	int x, y, m, n;
	memcpy(tmpImage->R, image->R, ImageWidth(image) * ImageHeight(image) * sizeof(unsigned char));
	memcpy(tmpImage->G, image->G, ImageWidth(image) * ImageHeight(image) * sizeof(unsigned char));
	memcpy(tmpImage->B, image->B, ImageWidth(image) * ImageHeight(image) * sizeof(unsigned char));
	for (y = 1; y < ImageHeight(image) - 1; y++) {
		for (x = 1; x < ImageWidth(image) - 1; x++) {
			int tmpR = 0;
			int tmpG = 0;
			int tmpB = 0;
			for (m = -1; m <= 1; m++) {
				for (n = -1; n <= 1; n++) {
					tmpR += (GetPixelR(tmpImage, x, y) - GetPixelR(tmpImage, x + n, y + m));
					tmpG += (GetPixelG(tmpImage, x, y) - GetPixelG(tmpImage, x + n, y + m));
					tmpB += (GetPixelB(tmpImage, x, y) - GetPixelB(tmpImage, x + n, y + m));
				}
			}
			SetPixelR(image, x, y, (tmpR > 255) ? 255 : (tmpR < 0) ? 0 : tmpR);
			SetPixelG(image, x, y, (tmpG > 255) ? 255 : (tmpG < 0) ? 0 : tmpG);
			SetPixelB(image, x, y, (tmpB > 255) ? 255 : (tmpB < 0) ? 0 : tmpB);
		}
	}
	for (y = 0; y < ImageHeight(image); y++) {
		x = 0;
		SetPixelR(image, x, y, 0);
		SetPixelG(image, x, y, 0);
		SetPixelB(image, x, y, 0);
		x = ImageWidth(image) - 1;
		SetPixelR(image, x, y, 0);
		SetPixelG(image, x, y, 0);
		SetPixelB(image, x, y, 0);
	}
	for (x = 0; x < ImageWidth(image); x++) {
		y = 0;
		SetPixelR(image, x, y, 0);
		SetPixelG(image, x, y, 0);
		SetPixelB(image, x, y, 0);
		y = ImageHeight(image) - 1;
		SetPixelR(image, x, y, 0);
		SetPixelG(image, x, y, 0);
		SetPixelB(image, x, y, 0);
	}
	DeleteImage(tmpImage);
	tmpImage = NULL;
	return image;
}

/* Add a watermark to an image */
Image *Watermark(Image *image, const Image *watermark, unsigned int topLeftX, unsigned int topLeftY)
{
    assert(image);
    assert(watermark);
    double newColorR, newColorG, newColorB;

    // Loop over the watermark image dimensions
    for (unsigned int y = 0; y < ImageHeight(watermark); y++) {
        for (unsigned int x = 0; x < ImageWidth(watermark); x++) {
            // Calculate corresponding position on the main image
            unsigned int imgX = x + topLeftX;
            unsigned int imgY = y + topLeftY;

            // Check if it's within the bounds of the main image
            if (imgX < ImageWidth(image) && imgY < ImageHeight(image)) {
                // Check if watermark pixel is black
                if ((GetPixelR(watermark, x, y) == 0) &&
                    (GetPixelG(watermark, x, y) == 0) &&
                    (GetPixelB(watermark, x, y) == 0)) 
                {
                    newColorR = GetPixelR(image, imgX, imgY) * 1.45;
                    SetPixelR(image, imgX, imgY, (unsigned char)(newColorR > 255 ? 255 : newColorR));
                    
                    newColorG = GetPixelG(image, imgX, imgY) * 1.45;
                    SetPixelG(image, imgX, imgY, (unsigned char)(newColorG > 255 ? 255 : newColorG));
                    
                    newColorB = GetPixelB(image, imgX, imgY) * 1.45;
                    SetPixelB(image, imgX, imgY, (unsigned char)(newColorB > 255 ? 255 : newColorB));
                }
            }
        }
    }
    return image;
}

/* Spotlight */
Image *Spotlight(Image *image, int centerX, int centerY, unsigned int radius)
{
	int width = image->W;
	int height = image->H;
	for(int i = 0; i<width; i++)
	{
		for(int j = 0; j< height; j++)
		{
			int xd = fabs(i - centerX);
			int yd = fabs(j - centerY);
			double distance = sqrt((double)(xd * xd + yd * yd));
			if(distance > radius)
			{
				SetPixelR(image, i, j, 0);
				SetPixelG(image, i, j, 0);
				SetPixelB(image, i, j, 0);
			}
		}
	}
	return image;
}

/* Rotate and zoom an image */
Image *Rotate(Image *image, double Angle, double ScaleFactor)
{
	assert(image);
	int WIDTH = ImageWidth(image);
	int HEIGHT = ImageHeight(image);
	unsigned char RotatedR[WIDTH][HEIGHT];
    unsigned char RotatedG[WIDTH][HEIGHT];
    unsigned char RotatedB[WIDTH][HEIGHT];
    int x, y;
    int x2, y2;
    const double pi = 3.141592653589793238462024;
    double Radian;
    double ScaleX, ScaleY;
    double M[2][2];
	int CenterX = WIDTH/2;
	int CenterY = HEIGHT/2;

    Radian = -Angle/360.0 * 2*pi;
    ScaleX = ScaleFactor;
    ScaleY = ScaleFactor;
    M[0][0] = cos(Radian)/ScaleX;
    M[0][1] = -sin(Radian)/ScaleX;
    M[1][0] = sin(Radian)/ScaleY;
    M[1][1] = cos(Radian)/ScaleY;
    for(y=0; y<HEIGHT; y++)
    {   for(x=0; x<WIDTH; x++)
        {
            x2 = (x-CenterX)*M[0][0] + (y-CenterY)*M[0][1] + CenterX;
            y2 = (x-CenterX)*M[1][0] + (y-CenterY)*M[1][1] + CenterY;
            if (  (x2<0) || (x2>WIDTH-1)
                ||(y2<0) || (y2>HEIGHT-1))
            {
                RotatedR[x][y] = 0;
                RotatedG[x][y] = 0;
                RotatedB[x][y] = 0;
            }
            else
            {
                RotatedR[x][y] = GetPixelR(image, x2, y2);
                RotatedG[x][y] = GetPixelG(image, x2, y2);
                RotatedB[x][y] = GetPixelB(image, x2, y2);
            }
        }
    }
    for(y=0; y<HEIGHT; y++)
    {   for(x=0; x<WIDTH; x++)
        {
            SetPixelR(image, x, y, RotatedR[x][y]);
            SetPixelG(image, x, y, RotatedG[x][y]);
            SetPixelB(image, x, y, RotatedB[x][y]);
        }
    }
	return image;
}
/* EOF */
