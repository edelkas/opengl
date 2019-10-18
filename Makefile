# Nota: Es preciso instalar GLFW (libglfw3 y libglfw3-dev).
# Con ello, viene GLUT (ligGL). Ambas hay que linkarlas.

CPPFLAGS = -Iinclude
LDFLAGS = -Llib -lglfw -lGL

App:
	g++ src/App.cpp $(CPPFLAGS) $(LDFLAGS) -o App
