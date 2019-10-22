CC = g++
CFLAGS = -Wall 
#LIBS = -lglew32 -lglfw3 -lopengl32 #-lglew32 -lfreeglut -lopengl32 -lglu32 
LIBS = -lglew32 -lglfw3 -Lc:/qtproject/opengl/mesa-18.0.0/build/windows-x86_64-debug/gallium/targets/libgl-gdi/ -lopengl32
default:glTest
glTest: opengl.cpp
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

.PHONY: clean
clean:
	rm -f glTest
