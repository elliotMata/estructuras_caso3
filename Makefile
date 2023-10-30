all:
	g++ main.cpp NounReader.cpp JsonParser.cpp PhraseParser.cpp -o main

books:
	g++ main.cpp JsonParser.cpp -o main