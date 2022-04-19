all: jderobot2 clean

jderobot2: jderobot2.o
	g++ -o jderobot2 jderobot2.o

jderobot2.o: jderobot2.cpp
	g++ -c jderobot2.cpp
	
clean:
	rm *.o jderobot2
