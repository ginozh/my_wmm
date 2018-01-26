CC = g++
CFLAGS = -Wall 
LIBS = -lglew32 -lglfw3 -lopengl32 #-lglew32 -lfreeglut -lopengl32 -lglu32 
default:glTest
glTest: opengl.cpp
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

.PHONY: clean
clean:
	rm -f glTest
