all:
	g++ main.cpp NounReader.cpp JsonParser.cpp PhraseParser.cpp -o main

books:
	g++ main.cpp JsonParser.cpp -o main

server:
	g++ Server.cpp -o server

# docker run --network host -it --rm -v "F:\2023\Semestre 2\Estructuras de Datos\caso3:/home" gcc bash