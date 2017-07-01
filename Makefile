4inarow: main.cpp
	g++ -o 4inarow main.cpp

run: 4inarow
	./4inarow

clean:
	rm -f 4inarow
