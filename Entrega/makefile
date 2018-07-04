default:
	bison -d gramaticas.cpp -o gramaticas.tab.c
	flex expresiones.l
	g++ -g -o parser lex.yy.c gramaticas.tab.c -std=c++11
