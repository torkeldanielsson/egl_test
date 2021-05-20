CC=clang
OUTPUTNAME=egl_test
OBJS=egl_test.o
LIBS = -lEGL

default: all

all: $(OBJS)
	$(CC) -o $(OUTPUTNAME) $(OBJS) $(LIBS)

.PHONY: clean
clean:
	rm *.o
	rm $(OUTPUTNAME)