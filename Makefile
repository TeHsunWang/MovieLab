# Variables
CC = gcc
DEBUG = -g -DDEBUG
CFLAGS = -std=c11 -Wall -c
LFLAGS = -Wall -lm
DEPS = Image.h FileIO.h DIPs.h ImageList.h Movie.h

# Object files for MovieLab
OBJS = MovieLab.o FileIO.o DIPs.o Image.o ImageList.o Movie.o
DEBUG_OBJS = MovieLab_DEBUG.o FileIO_DEBUG.o DIPs_DEBUG.o Image_DEBUG.o ImageList_DEBUG.o Movie_DEBUG.o

# Default target
all: MovieLab

# Generate object files
MovieLab.o: MovieLab.c $(DEPS)
	$(CC) $(CFLAGS) MovieLab.c -o MovieLab.o

FileIO.o: FileIO.c FileIO.h
	$(CC) $(CFLAGS) FileIO.c -o FileIO.o

DIPs.o: DIPs.c DIPs.h
	$(CC) $(CFLAGS) DIPs.c -o DIPs.o

Image.o: Image.c Image.h
	$(CC) $(CFLAGS) Image.c -o Image.o

ImageList.o: ImageList.c ImageList.h
	$(CC) $(CFLAGS) ImageList.c -o ImageList.o

Movie.o: Movie.c Movie.h
	$(CC) $(CFLAGS) Movie.c -o Movie.o

# Generate the executable
MovieLab: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o MovieLab

# Debug versions of object files
MovieLab_DEBUG.o: MovieLab.c $(DEPS)
	$(CC) $(CFLAGS) $(DEBUG) MovieLab.c -o MovieLab_DEBUG.o

# (Include similar DEBUG targets for the other object files...)

# Clean up
clean:
	rm -f *.o *.a MovieLab

# Additional targets as needed...
