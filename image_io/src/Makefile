TARGET = image_manip
OBJS = image_manip.o image_io.o transforms.o
CPPFLAGS = -g -Wall -O3
LDFLAGS = `sdl-config --cflags --libs` -lSDL_image -lstdc++
.PHONY:clean

# CXX is already defined and depends on the object being made
${TARGET}:${OBJS}
	${CXX} -o ../${TARGET} ${OBJS} ${CPPFLAGS} ${LDFLAGS}

image_manip.o:image_manip.h image_io.h transforms.h
image_io.o:image_io.h
transforms.o:transforms.h image_io.h

# This rule must be called explicitly
clean:
	rm -f *.o ../${TARGET}.exe core*
