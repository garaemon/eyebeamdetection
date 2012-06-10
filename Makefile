all: eyebeamdetection


eyebeamdetection: eyebeamdetection.cpp
	g++ $^ -o $@ `pkg-config --libs --cflags opencv` -g

clean:
	rm -f eyebeamdetection
