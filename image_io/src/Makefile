TARGET = image_manip
OBJS = image_manip.o image_io.o #transforms.o
CXXFLAGS = -g -Wall
LDFLAGS = -lmingw32 -lSDLmain -lSDL -lSDL_image -lstdc++
.PHONY:clean

# CXX is already defined and depends on the object being made
${TARGET}:${OBJS}
	${CXX} -o ../${TARGET} ${OBJS} ${CXXFLAGS} ${LDFLAGS}

image_manip.o:image_manip.h
image_io.o:image_io.h
#transforms.o:transforms.h

# This rule must be called explicitly
clean:
	rm -f *.o ../${TARGET}.exe core*
