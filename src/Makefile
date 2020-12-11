# Makefile for DIP programming environment
#
#

# GNU compilers, flags
CPP = g++ -O3
CFLAGS = -Wall -pedantic -Wno-deprecated
LIBS = -lcfitsio -lm -lcurl 

# Paths for includes and libraries
INCP = -I/usr/local/include -I../module 
LIBP = -L/usr/local/lib 

# Locally compiled modules
OBJS_DIR = ../module
OBJS_FILE = spline3.o sstring.o iastring.o options.o fitsimage.o
OBJS = $(addprefix $(OBJS_DIR)/, $(OBJS_FILE))

# All required modules
MODS = $(INCP) $(LIBP) $(OBJS) $(LIBS)



ReferencePixelCorrect : ReferencePixelCorrect.cpp $(OBJS)
	${CPP} $(CFLAGS) -o ReferencePixelCorrect ReferencePixelCorrect.cpp $(MODS)

Up-the-Ramp_test : Up-the-Ramp_test.cpp $(OBJS)
	${CPP} $(CFLAGS) -o Up-the-Ramp_test Up-the-Ramp_test.cpp $(MODS)

Up-the-Ramp_test3 : Up-the-Ramp_test3.cpp $(OBJS)
	${CPP} $(CFLAGS) -o Up-the-Ramp_test3 Up-the-Ramp_test3.cpp $(MODS)

SampleProgram1_20201209 : SampleProgram1_20201209.cpp $(OBJS)
	${CPP} $(CFLAGS) -o SampleProgram1_20201209 SampleProgram1_20201209.cpp $(MODS)

SampleProgram2_20201209 : SampleProgram2_20201209.cpp $(OBJS)
	${CPP} $(CFLAGS) -o SampleProgram2_20201209 SampleProgram2_20201209.cpp $(MODS)

SampleProgram3_20201210 : SampleProgram3_20201210.cpp $(OBJS)
	${CPP} $(CFLAGS) -o SampleProgram3_20201210 SampleProgram3_20201210.cpp $(MODS)

# Modules compiled and linked separately
sstring.o : sstring.cpp sstring.h
	${CPP} $(CFLAGS) -c sstring.cpp $(INCP)

iastring.o : iastring.cpp iastring.h
	${CPP} $(CFLAGS) -c iastring.cpp $(INCP)

options.o : options.cpp options.h
	${CPP} $(CFLAGS) -c options.cpp $(INCP)

fitsimage.o : fitsimage.cpp fitsimage.h
	${CPP} $(CFLAGS) -c fitsimage.cpp $(INCP)

