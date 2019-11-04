# Nota: Es preciso instalar GLFW (libglfw3 y libglfw3-dev) y GLEW.
# Con ello, viene GLUT (ligGL). Ambas hay que linkarlas.

CPPFLAGS = -Iinclude
LDFLAGS = -Llib -lGLEW -lglfw -lGL

build:
	rm -f App;
	g++ src/*.cpp $(CPPFLAGS) $(LDFLAGS) -o App
