CC = gcc 
CFLAGS = -Wall 
LIBS = -lglew32 -lfreeglut -lopengl32 -lglu32 
default:glTest glHello
glTest: main.cpp
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

glHello: opengl_helloworld.cpp
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

.PHONY: clean
clean:
	rm -f glTest glHello
