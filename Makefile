all:
	g++ main.cpp NounReader.cpp JsonParser.cpp PhraseParser.cpp -o main

server:
	g++ Server.cpp PhraseParser.cpp JsonParser.cpp NounReader.cpp -o server

# docker run --network host -it --rm -v "F:\2023\Semestre 2\Estructuras de Datos\caso3:/home" gcc bash
# http://localhost:65000/match?req_phrase=love%20flower%20relationship%20dream