#ifndef LEXER_JSON_DOT
#define LEXER_JSON_DOT

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <cctype>

// Lexing functions
void get_lexed(std::string &name_file, std::vector<std::string> &my_parsed_file, std::string &type_file, bool debug);
// Checkers functions
void check_syntaxe_dot(std::vector<std::string> &my_parsed_file, std::vector<std::string> &error_report);
void check_syntaxe_json(std::vector<std::string> &my_parsed_file, std::vector<std::string> &error_report);

#endif