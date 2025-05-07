CC = g++
CFLAGS  = -g -Wall
STD_VERSION = -std="c++11"

all: lexer_json_dot.o parser_json.o parser_dot.o output_file_generator.o element.o circuit.o main.o
	$(CC) $(FLAGS) lexer_json_dot.o parser_json.o parser_dot.o output_file_generator.o element.o circuit.o main.o -o simulator $(STD_VERSION)

lexer_json_dot.o: lexer_json_dot.cpp
	$(CC) $(FLAGS) -c lexer_json_dot.cpp $(STD_VERSION)

parser_json.o: parser_json.cpp
	$(CC) $(FLAGS) -c parser_json.cpp $(STD_VERSION)

parser_dot.o: parser_dot.cpp
	$(CC) $(FLAGS) -c parser_dot.cpp $(STD_VERSION)

output_file_generator.o: output_file_generator.cpp
	$(CC) $(FLAGS) -c output_file_generator.cpp $(STD_VERSION)

element.o: element.cpp
	$(CC) $(FLAGS) -c element.cpp $(STD_VERSION)

circuit.o: circuit.cpp
	$(CC) $(FLAGS) -c circuit.cpp $(STD_VERSION)

main.o: main.cpp
	$(CC) $(FLAGS) -c main.cpp $(STD_VERSION)

clean:
	$(RM) all *.o *~
