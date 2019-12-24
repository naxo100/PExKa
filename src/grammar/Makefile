all:
	flex -d -o kappa3/Lexer.cpp kappa3/Lexer.l
	bison --verbose --debug -d -o kappa3/Parser.cpp kappa3/Parser.y 
	flex -d -o kappa4/Lexer.cpp kappa4/Lexer.l
	bison --verbose --debug -d -o kappa4/Parser.cpp kappa4/Parser.y 
#-std=c++0x 
#	g++ -std=c++0x -g ../main.cpp KappaLexer.cpp KappaParser.cpp KappaDriver.cpp ast/KappaAst.cpp -lboost_program_options ast/AlgebraicExpressions.cpp ast/Basics.cpp ast/Dynamics.cpp ast/Spatial.cpp ../state/Variable.cpp -lboost_program_options -o parser #&> compile_errors
	
clean:
	rm -rf kappa3/Lexer.cpp kappa3/Parser.cpp kappa3/Parser.hpp kappa3/stack.hh
	rm -rf kappa4/Lexer.cpp kappa4/Parser.cpp kappa4/Parser.hpp kappa4/stack.hh
#	rm -rf kappa3/location.hh kappa3/position.hh kappa3/stack.hh
	rm -rf kappa3/Parser.output kappa4/Parser.output
#	rm -rf parser
