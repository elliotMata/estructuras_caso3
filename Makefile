all:
	g++ main.cpp NounReader.cpp JsonParser.cpp PhraseParser.cpp FileReader.cpp -o main

server:
	g++ Server.cpp PhraseParser.cpp JsonParser.cpp NounReader.cpp FileReader.cpp -o server

temp:
	g++ temp.cpp NounReader.cpp FileReader.cpp -o temp

json:
	g++ JsonCreator.cpp NounReader.cpp FileReader.cpp -o json
	
# docker run -it --rm -v "F:\2023\Semestre 2\Estructuras de Datos\caso3:/home" -p 65000:65000 gcc bash
# http://localhost:65000/match?req_phrase=horror%20monster%20dark%20terror