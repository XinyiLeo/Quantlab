app: app.o
	g++ app.o -o app

app.o: app.cpp
	g++ -c app.cpp

clean:
	rm *.o
