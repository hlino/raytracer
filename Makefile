CC = g++
CFLAGS = -Wall -c
INCFLAGS = -I./Include/
LIBFLAGS = -lfreeimage -L./Lib/
OBJS = main.o Camera.o Vect.o Ray.o Light.o Color.o Sphere.o Object.o Plane.o


all: Tracer

Tracer: $(OBJS)
	$(CC) $(OBJS) -o Tracer.exe $(LIBFLAGS)
	rm -f $(OBJS)

camera.o: camera.h Camera.cpp vect.h
	$(CC) $(CFLAGS) Camera.cpp

vect.o: vect.h Vect.cpp
	$(CC) $(CFLAGS) Vect.cpp

ray.o: ray.h Ray.cpp vect.h
	$(CC) $(CFLAGS) Ray.cpp

color.o: color.h Color.cpp
	$(CC) $(CFLAGS) Color.cpp

light.o: light.h Light.cpp color.h
	$(CC) $(CFLAGS) Light.cpp

sphere.o: sphere.h Sphere.cpp color.h vect.h object.h
	$(CC) $(CFLAGS) Sphere.cpp

object.o: object.h Object.cpp color.h ray.h vect.h
	$(CC) $(CFLAGS) Object.cpp

plane.o: plane.h Plane.cpp vect.h color.h object.h
	$(CC) $(CFLAGS) Plane.cpp

main.o: main.cpp vect.h ray.h camera.h color.h light.h sphere.h object.h plane.h
	$(CC) $(CFLAGS) $(INCFLAGS) main.cpp

clean:
	rm -f main.o *.png Tracer
