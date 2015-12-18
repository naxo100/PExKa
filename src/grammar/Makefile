all:
	lex kappaLexer.l
	yacc --verbose --debug -d kappaParser.y -o kappaParser.cc
	cc -c lex.yy.c -o lex.yy.o
	c++ lex.yy.o kappaParser.cc -o parser
	
clean:
	rm -f -r *.hh
	rm -f -r *.c
	rm -f -r *.cc
	rm -f -r *.output
	rm -f -r *.o