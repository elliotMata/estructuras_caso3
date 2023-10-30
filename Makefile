all:
	g++ NounReader.cpp JsonParser.cpp -o main

books:
	g++ main.cpp JsonParser.cpp -o main