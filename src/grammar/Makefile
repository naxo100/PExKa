all:
	flex -d -+ -o KappaLexer.cpp kappaLexer.l
	bison --verbose --debug -d -o KappaParser.cpp kappaParser.y 
#	cc -c lex.yy.c -o lex.yy.o
#-std=c++0x 
	g++ -g KappaLexer.cpp KappaParser.cpp ast/KappaAst.cpp ../main.cpp -o parser
	
clean:
	rm -rf KappaLexer.cpp
	rm -rf KappaParser.cpp KappaParser.hpp location.hh position.hh stack.hh
	rm -rf parser