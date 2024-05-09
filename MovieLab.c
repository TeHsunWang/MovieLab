/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Winter 2024                   */
/* MovieLab.c: source file for the main function                     */
/* Please use this template for your HW5.                            */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <math.h>

#include "FileIO.h"
#include "DIPs.h"
#include "Movie.h"
#include "Constants.h"
#include "Image.h"
#include "ImageList.h"

/* Load one movie frame from the input file */
YUVImage* LoadOneFrame(const char* fname, int n,
	unsigned int width, unsigned height);

/* Load the movie frames from the input file */
Movie *LoadMovie(const char *fname, int frameNum,
	unsigned int width, unsigned height);

/* Saves one movie frame to the output file */
void SaveOneFrame(YUVImage *image, const char *fname, FILE *file);

/* Save the movie frames to the output file */
int SaveMovie(const char *fname, Movie *movie);

/* Print the command-line arguments usage of the program */
void PrintUsage();

int main(int argc, char *argv[])
{
	int x = 0;
	char *fin = NULL;
	char *fout = NULL;
	
	unsigned int width = 512;
	unsigned int height = 288;

	int framenum = 0;
	Movie* movie = NULL;
	Image* watermarkImage = NULL;
	
	/* the while loop to check options in the command line */
	while (x < argc) {
		/* the input file name */
		if (strcmp(argv[x], "-i") == 0) {
			if (x < argc - 1) {
				fin = argv[x + 1];
			} /*fi*/
			else {
				printf("Missing argument for the input file name!\n");
				return 5;
			} /*esle*/
			x += 2;
			continue;
		} /*fi*/

		/* the output file name */
		if (strcmp(argv[x], "-o") == 0) {
			if (x < argc - 1) {
				fout = argv[x + 1];
			} /*fi*/
			else {
				printf("Missing argument for the output file name!\n");
				return 5;
			} /*esle*/
			x += 2;
			continue;
		} /*fi*/

		/* the help option */
		if (strcmp(argv[x], "-h") == 0) {
			PrintUsage();
			return 0;
		} /*fi*/

		if (strcmp(argv[x], "-s") == 0) {
			if (x < argc - 1) {
				if (sscanf(argv[x + 1], "%ux%u", &width, &height) != 2) {
					printf("Error inputting the height and width\n");
					return 5;
				}
			} else {
				printf("Missing argument for resolution -s\n");
				return 5;
			}
			x += 2;
			continue;
		}

		if (strcmp(argv[x], "-f") == 0)
		{
			if(x < argc -1)
			{
				framenum = atoi(argv[x+1]);
			}
			else
			{
				printf("Missing argument for framenum\n");
				return 5;
			}
			x += 2;
			continue;
		}

		if (strcmp(argv[x], "-bw") == 0) {
			printf("bw in progress...\n");
            if (movie == NULL) {
                // Load the movie only if it hasn't been loaded yet
                movie = LoadMovie(fin, framenum, width, height);
                if (movie == NULL) {
                    printf("Error loading movie from file %s\n", fin);
                    return 1;  // Indicate error
                }
                printf("The movie file %s has been read successfully!\n", fin);
                YUV2RGBMovie(movie); // Convert YUV to RGB for processing
            }
            // Apply Black and White filter to each frame
            IENTRY *current = movie->Frames->First;
			if(!current->image) printf("fail to load frame image\n");
            while (current != NULL) {
                Image *frameImage = (Image *)current->image;  // Cast to Image* if needed
                Image *bwImage = BlackNWhite(frameImage);
                current->image = bwImage;  // Update frame with processed image
                current = current->Next;
            }

            printf("Black and White filter applied.\n");
            x++;  // Move to next argument
            continue;
		}

		if(strcmp(argv[x], "-hmirror") == 0)
		{
			printf("hmirror in process...\n");
			if(movie == NULL)
			{
				movie = LoadMovie(fin, framenum, width, height);
				if (movie == NULL)
				{
					printf("Fail to Load movie\n");
					return 1;
				}
				printf("The movie file %s has been read successfully!\n", fin);
				YUV2RGBMovie(movie);
			}
			IENTRY* current = movie->Frames->First;
			if(!current->image) printf("Fail to load image\n");
			while(current != NULL)
			{
				Image* frameImg = (Image*)current->image;
				Image* hmirrorImg = HMirror(frameImg);
				current->image = hmirrorImg;
				current = current->Next;
			}
			printf("Hmirror filter applied.\n");
            x++;  // Move to next argument
            continue;
		}

		if(strcmp(argv[x], "-edge") == 0)
		{
			printf("edge in process...\n");
			if(movie == NULL)
			{
				movie = LoadMovie(fin, framenum, width, height);
				if(movie == NULL)
				{
					printf("Fail to load movie\n");
					return 1;
				}
				printf("The movie file %s has been read successfully!\n", fin);
				YUV2RGBMovie(movie);
			}
			IENTRY* current = movie->Frames->First;
			if(!current->image) printf("Fail to load image\n");
			while(current != NULL)
			{
				Image* frameImg = (Image*)current->image;
				Image* edgeImg = Edge(frameImg);
				current->image = edgeImg;
				current = current->Next;
			}
			printf("Edge filter applied. \n");
			x++;
			continue;
		}
		if(strcmp(argv[x], "-crop") == 0)
		{
			if(movie == NULL)
			{
				movie = LoadMovie(fin, framenum, width, height);
				if (movie == NULL)
				{
					printf("Fail to load movie\n");
					return 1;
				}
				printf("The movie file %s has been read successfully!\n", fin);
				YUV2RGBMovie(movie);
			}

			unsigned int start, end;
			if (sscanf(argv[x+1], "%u-%u", &start, &end) == 2) 
			{
				CropImageList(movie->Frames, start, end);
			} 
			else 
			{
				printf("no correct format for crop\n");
			}
			x+=2;
			continue;
		}
		if(strcmp(argv[x], "-fast") == 0)
		{
			printf("fast in process...\n");
			if(movie == NULL)
			{
				movie = LoadMovie(fin, framenum, width, height);
				if (movie == NULL)
				{
					printf("Failed to load movie");
				}
				printf("The movie file %s has been read successfully!\n", fin);
				YUV2RGBMovie(movie);
			}
			unsigned int factor; 
			if (sscanf(argv[x+1], "%u", &factor) == 1) 
			{
				FastImageList(movie->Frames, factor);
			} 
			else 
			{
				printf("no correct format for fast\n");
			}
			x += 2;
			continue;
		}

		if(strcmp(argv[x], "-reverse") == 0)
		{
			if(movie == NULL)
			{
				movie = LoadMovie(fin, framenum, width, height);
				if(movie == NULL)
				{
					printf("Fail to Load Movie\n");
					return 1;
				}
				printf("The movie file %s has been read successfully!\n", fin);
				YUV2RGBMovie(movie);
			}
			ReverseImageList(movie->Frames);
			x++;
			continue;
		}

		if(strcmp(argv[x], "-watermark") == 0)
		{
			int topleftX = 0;
			int topleftY = 0;
			if(movie == NULL)
			{
				movie = LoadMovie(fin, framenum, width, height);
				if(movie == NULL)
				{
					printf("Fail to load movie\n");
					return 1;
				}
				printf("The movie file %s has been read successfully!\n", fin);
				YUV2RGBMovie(movie);
			}
			IENTRY* current = movie->Frames->First;
			if(!current->image) printf("Fail to load image\n");
			char * watermarkName = NULL;
			if (x < argc-1) 
			{
				watermarkName = argv[x+1];
				watermarkImage = LoadImage(watermarkName);
			} 
			else 
			{
				printf("no correct format for watermark\n");
			}
			while(current != NULL)
			{
				if(framenum % WATERMARK_PER_FRAMES != 0)
				{
					topleftX = rand() % (width - ImageWidth(watermarkImage));
					topleftY = rand() % (height - ImageHeight(watermarkImage)); 
				}
				framenum ++;
				Image* frameImg = current->image;
				Image* watermarkImg = Watermark(frameImg, watermarkImage, topleftX, topleftY);
				current->image = watermarkImg;
				current = current->Next;
			}
			printf("Watermark filter has been applied\n");
			DeleteImage(watermarkImage);
			watermarkImage = NULL;
			x += 2;
			continue;
		}

		if(strcmp(argv[x], "-spotlight") == 0)
		{
			if(movie == NULL)
			{
				movie = LoadMovie(fin, framenum, width, height);
				printf("The movie file %s has been read successfully!\n", fin);
				YUV2RGBMovie(movie);
			}
			int numFrames = 20;
			Image * firstImage = movie->Frames->First->image;
			Image * lastImage = movie->Frames->Last->image;
			double maxRadius = sqrt( (0.5*width)*(0.5*width) + (0.5*height)*(0.5*height) );
			int increment = (int)(maxRadius / (numFrames-1));
			int r[numFrames];
			int centerX = width/2;
			int centerY = height/2;
			for(int i = 0; i<numFrames; i++)
			{
				r[i] = i*increment;
			}
			for(int i = 0; i<numFrames; i++)
			{
				Image* newImage = CopyRGBImage(firstImage);
				newImage = Spotlight(newImage, centerX, centerY, r[numFrames-1-i]);
				PrependRGBImage(movie->Frames, newImage);

				Image * newImage2 = CopyRGBImage(lastImage);
				newImage2 = Spotlight(newImage2, centerX, centerY, r[numFrames-1-i]);
				AppendRGBImage(movie->Frames, newImage2);
			}
			printf("spotlight done\n");
			x+=1;
			continue;
		}

		if (strcmp(argv[x], "-rotate") == 0) 
		{
			if (movie == NULL) {
				movie = LoadMovie(fin, framenum, width, height);
				if (movie == NULL) {
					printf("Fail to load movie\n");
					return 1;
				}
				printf("The movie file %s has been read successfully!\n", fin);
				YUV2RGBMovie(movie);
			}
			Image* firstImg = movie->Frames->First->image;
			Image* lastImg = movie->Frames->Last->image;
			Image* tempFrames[20];

			// Generate and store new frames
			for(int i = 0; i < 20; i++) {
				Image* newImg = CopyRGBImage(firstImg);
				double scaleFactor = 0.2 + i * 0.04;
				newImg = Rotate(newImg, -20.0 + i * 1.0, scaleFactor);
				tempFrames[i] = newImg; // Store the frame
			}

			// Prepend frames to the movie in reverse order
			for(int i = 20 - 1; i >= 0; i--) {
				PrependRGBImage(movie->Frames, tempFrames[i]);
			}
			for(int i = 0; i < 20; i++)
			{
				Image* newImg = CopyRGBImage(lastImg);
				newImg = Rotate(newImg, -i, 1+0.04*i);
				AppendRGBImage(movie->Frames, newImg);
			}
			printf("Rotate effect has been applied\n");
			x++;
			continue;
		}
		x++;
	} /*elihw*/

	if (fin == NULL) {
		printf("Missing argument for the input file name!\n");
		PrintUsage();
		return 5;
	}

	if (fout == NULL) {
		printf("Missing argument for the output file name!\n");
		PrintUsage();
		return 5;
	}
	
	/* to be implemented */
	RGB2YUVMovie(movie);
	SaveMovie(fout, movie);
	printf("The movie file %s has been written successfully!\n", fout);
	printf("%d frames are written to the file out.yuv in total.\n", framenum);
	DeleteMovie(movie);
	fin = NULL;
	fout = NULL;

	return 0;
}

void PrintUsage()
{
	printf("\nUsage: MovieLab -i <file> -o <file> -f <framenum> [options]\n"
	       "Options:\n"
	       "-s <WidthxHeight>     To set the resolution of the input stream (WidthxHeight)\n"
	       "-bw                   Activate the black and white filter on every movie frame\n"
	       "-hmirror              Activate horizontal mirror on every movie frame\n"
	       "-edge                 Activate the edge filter on every movie frame\n"
	       "-crop <start-end>     Crop the movie frames from <start> to <end>\n"
	       "-fast <factor>        Fast forward the movie by <factor>\n"
	       "-reverse              Reverse the frame order of the input movie\n"
	       "-watermark <file>     Add a watermark from <file> to every movie frame\n"
	       "-spotlight            Spotlight the fade in and fade out\n"
	       "-rotate               Rotate and zoom in/out the input movie\n"
	       "-h                    Display this usage information\n"
	      );
}

/* Load one movie frame from the input file */
YUVImage* LoadOneFrame(const char* fname, int n,
	unsigned int width, unsigned height)
{
	FILE *file;
	unsigned int x, y;
	unsigned char c;
	YUVImage* YUVimage;

	/* Check errors */
	assert(fname);
	assert(n >= 0);

	YUVimage = CreateYUVImage(width, height);
	if (YUVimage == NULL) {
		return NULL;
	}

	/* Open the input file */
	file = fopen(fname, "r");
	if (file == NULL) {
		DeleteYUVImage(YUVimage);
		return NULL;
	}

	/* Find the desired frame */
	fseek(file, 1.5 * n * width * height, SEEK_SET);

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			c = fgetc(file);
			SetPixelY(YUVimage, x, y, c);
		} /*rof*/
	}

	for (y = 0; y < height; y += 2) {
		for (x = 0; x < width; x += 2) {
			c = fgetc(file);
			SetPixelU(YUVimage, x, y, c);
			SetPixelU(YUVimage, x + 1, y, c);
			SetPixelU(YUVimage, x, y + 1, c);
			SetPixelU(YUVimage, x + 1, y + 1, c);
		}
	}

	for (y = 0; y < height; y += 2) {
		for (x = 0; x < width; x += 2) {
			c = fgetc(file);
			SetPixelV(YUVimage, x, y, c);
			SetPixelV(YUVimage, x + 1, y, c);
			SetPixelV(YUVimage, x, y + 1, c);
			SetPixelV(YUVimage, x + 1, y + 1, c);
		}
	}

	/* Check errors */
	assert(ferror(file) == 0);

	/* Close the input file and return */
	fclose(file);
	file = NULL;
	return YUVimage;
}

/* Load the movie frames from the input file */
Movie *LoadMovie(const char *fname, int frameNum,
	unsigned int width, unsigned height)
{
	Movie *newmovie = CreateMovie();
    if (newmovie == NULL) {
		printf("Load Movie failed at LoadMovie");
        return NULL;
    }

    for (int i = 0; i < frameNum; i++) {
        YUVImage *frame = LoadOneFrame(fname, i, width, height);
        if (frame != NULL) {
            AppendYUVImage(newmovie->Frames, frame);
        } else {
            // Handle error: could not load frame
            fprintf(stderr, "Error loading frame %d from file %s\n", i, fname);
            // Decide how to handle this error. Maybe continue or free and return NULL
            DeleteMovie(newmovie);
            return NULL;
        }
    }
    return newmovie;
}

/* Save the movie frames to the output file */
int SaveMovie(const char *fname, Movie *movie)
{
	int count;
	FILE *file;
	IENTRY *curr;

	/* Open the output file */
	file = fopen(fname, "w");
	if (file == NULL) {
		return 1;
	}

	count = 0;
	curr = movie->Frames->First;

	while (curr != NULL) {
		//here
		SaveOneFrame(curr->image, fname, file);
		curr = curr->Next;
		count++;
	}

	fclose(file);
	file = NULL;

	printf("The movie file %s has been written successfully!\n", fname);
	printf("%d frames are written to the file %s in total.\n", count, fname);
	return 0;
}

/* Saves one movie frame to the output file */
void SaveOneFrame(YUVImage *image, const char *fname, FILE *file)
{
	int x, y;
	for (y = 0; y < image->H; y++) {
		for (x = 0; x < image->W; x++) {
			fputc(GetPixelY(image, x, y), file);
		}
	}

	for (y = 0; y < image->H; y += 2) {
		for (x = 0; x < image->W; x += 2) {
			fputc(GetPixelU(image, x, y), file);
		}
	}

	for (y = 0; y < image->H; y += 2) {
		for (x = 0; x < image->W; x += 2) {
			fputc(GetPixelV(image, x, y), file);
		}
	}
}


/* EOF */
