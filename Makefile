EXEC = image_manip

SRCDIR = src
INCDIR = inc
OBJDIR = obj
LIBDIR = lib
BLDDIR = bld

LIBS = `sdl-config --cflags --libs` -lSDL_image -lstdc++

CXX = g++
CPPFLAGS = -I${INCDIR} -std=c++11 -O3 -g -Wall -Wextra


_DEPS = ${EXEC}.h \
		image_io.h \
		transforms.h
DEPS = ${patsubst %,${INCDIR}/%,${_DEPS}}

_OBJ = ${EXEC}.o \
	   image_io.o \
	   transforms.o
OBJ = ${patsubst %,${OBJDIR}/%,${_OBJ}}


${OBJDIR}/%.o: ${SRCDIR}/%.cpp ${DEPS}
		${CXX} -c -o $@ $< ${CPPFLAGS}

${BLDDIR}/${EXEC}: ${OBJ}
		${CXX} -o $@ $^ ${CPPFLAGS} ${LIBS}


.PHONY: clean

clean:
		rm -f ${BLDDIR}/${EXEC} ${OBJ} *~ core ${INCDIR}/*~
