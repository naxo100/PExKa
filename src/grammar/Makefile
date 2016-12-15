all:
	flex -d -o KappaLexer.cpp kappaLexer.l
	bison --verbose --debug -d -o KappaParser.cpp kappaParser.y 
#	cc -c lex.yy.c -o lex.yy.o
#-std=c++0x 
#	g++ -std=c++0x -g ../main.cpp KappaLexer.cpp KappaParser.cpp KappaDriver.cpp ast/KappaAst.cpp -lboost_program_options ast/AlgebraicExpressions.cpp ast/Basics.cpp ast/Dynamics.cpp ast/Spatial.cpp ../state/Variable.cpp -lboost_program_options -o parser #&> compile_errors
	
clean:
	rm -rf KappaLexer.cpp
	rm -rf KappaParser.cpp KappaParser.hpp location.hh position.hh stack.hh
	rm -rf KappaParser.output
	rm -rf parser
