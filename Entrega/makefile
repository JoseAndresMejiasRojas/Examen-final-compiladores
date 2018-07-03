default:
	bison -d gramaticas.cpp -ogramaticas.tab.c
	flex expresiones.l
	g++ -g -o parser.exe lex.yy.c gramaticas.tab.c
