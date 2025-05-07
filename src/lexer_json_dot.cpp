#include "lexer_json_dot.h"

/* get_lexed()
    -> looking for each character in input file
    -> 5 possible cases :
        -> escape, line feed, tab, carrier return are skipped
        -> special dot character such as [ { ; ...
        -> special json character such : ...
        -> "?" possibility with files, for practical use we decided to forgive '?' possibility
        -> word are incrementing until we meet anything mentionned before

    -> Caution ! "->" isn't consider as dot syntaxe so it have to be used with escape.
    This will be automatically detected in case of mistake
    -> comments are skipped until next line feed
*/
void get_lexed(std::string &name_file, std::vector<std::string> &my_parsed_file, std::string &type_file, bool debug = 0){

    std::vector<std::string> error_report;
    std::ifstream file_lex(name_file);
    // Try to open given file
    if(! file_lex.is_open()){
        std::cout << "[FATAL_ERROR][LEXER] Impossible to open dot file named : " << name_file << std::endl;
        exit(1);
    }

    // Variables used to lex
    char x;
    bool word_incr = 1;
    bool char_not_lexed = 1;
    bool is_comment = 0;
    std::string word;
    word.clear();

    // Going trough every chars of input file
    while( ! file_lex.eof()){
        char_not_lexed = 1;
        x = file_lex.get();
    // ----- Removing empty spaces & carrier return
        switch(x){
            case ' ':
            case '\r':
            case '\t':
            case '\n':{
                if(word_incr == 0){ my_parsed_file.push_back(word);word_incr = 1;}
                word.clear();
                char_not_lexed = 0;
                break;
            }
        }
    // ----- Case for dot files :
        if(char_not_lexed && type_file == "dot"){
            switch(x){
                case '{':
                case '[':
                case ']':
                case '}':
                case ';':
                case '=':
                case ',':{
                    if(word_incr == 0){ my_parsed_file.push_back(word);word_incr = 1;}
                    word.clear();
                    word = x;
                    my_parsed_file.push_back(word);
                    word.clear();
                    char_not_lexed = 0;
                    break;
                }
            }
        }
    // ----- Case for dot files :
        if(char_not_lexed && type_file == "json"){
            switch(x){
                case '{':
                case '[':
                case ']':
                case '}':
                case ';':
                case '=':
                case ':':
                case ',':{
                    if(word_incr == 0){ my_parsed_file.push_back(word);word_incr = 1;}
                    word.clear();
                    word = x;
                    my_parsed_file.push_back(word);
                    char_not_lexed = 0;
                    word.clear();
                    break;
                }
            }
        }
    // ----- Lexing brackets or words
        if(char_not_lexed){
            switch(x){
                case '/':{
                    if(word_incr == 0){ my_parsed_file.push_back(word);word_incr = 1;}
                    word.clear();
                    if(is_comment){
                        while(!(x == '\r' || x == '\n')){
                            x = file_lex.get();
                            word = word + x;
                        }
                        std::cout << "[LEXER][COMMENT] ignoring "<< word << std::endl;
                        is_comment = 0;
                        word.clear();
                        break;
                    }
                    is_comment = 1;
                    break;
                }
                case '"':{
                    if(word_incr == 0){ my_parsed_file.push_back(word);word_incr = 1;}
                    word.clear();
                    my_parsed_file.push_back("\"");
                    x = file_lex.get();
                    // Waiting until next
                    while(x != '"'){
                        word = word + x;
                        x = file_lex.get();
                        // Condition in case bracket opened but didn't closed before end of file
                        if(x == std::ifstream::traits_type::eof()){
                            std::cout << "[FATAL_ERROR][LEXER]["<< type_file <<"] Brackets opened but did not closed can't continue\n -------> last word lexed : "<< word << std::endl;
                            exit(1);
                        }
                        // Condition in case we encounter \" character
                        if(x == '\\'){
                            x = file_lex.get();
                            if(x == '"'){
                                word = word + '"';
                            } else{
                                word = word + '\\';
                            }
                        }
                    }
                    if(word.empty()){
                        error_report.push_back("[ERROR][LEXER]["+type_file+"] Word inside brackets is empty");
                    }
                    my_parsed_file.push_back(word);
                    my_parsed_file.push_back("\"");
                    word.clear();
                    char_not_lexed = 0;
                    break;
                }
                // ----------------- FEATURE TO REMOVE for ''potential update
                /*case '\'':{
                    if(word_incr == 0){ my_parsed_file.push_back(word);word_incr = 1;}
                    word.clear();
                    my_parsed_file.push_back("\"");
                    x = file_lex.get();
                    // Waiting until next
                    while(x != '\''){
                        word = word + x;
                        x = file_lex.get();
                        // Condition in case bracket opened but didn't closed before end of file
                        if(x == std::ifstream::traits_type::eof()){
                            std::cout << "[FATAL_ERROR][LEXER]["<< type_file <<"] Brackets opened but did not closed can't continue\n -------> last word lexed : "<< word << std::endl;
                            exit(1);
                        }
                        // Condition in case we encounter \" character
                        if(x == '\\'){
                            x = file_lex.get();
                            if(x == '\''){
                                word = word + '\'';
                            } else{
                                word = word + '\\';
                            }
                        }
                    }
                    if(word.empty()){
                        error_report.push_back("[ERROR][LEXER]["+type_file+"] Word inside brackets is empty");
                    }
                    my_parsed_file.push_back(word);
                    my_parsed_file.push_back("\"");
                    word.clear();
                    char_not_lexed = 0;
                    break;
                }*/
                // ----------------- FEATURE TO REMOVE for ''potential update
                // -----    Default getting word which doesn't correspond to any rules
                default:{
                    word = word + x;
                    char_not_lexed = 0;
                    word_incr = 0;
                    break;
                }
            }
        }
    }
    // Check if empty file
    if(my_parsed_file.size()<1){
        std::cout << "[FATAL_ERROR][LEXER] file named : " << name_file << " is empty"<<std::endl;
        exit(1);
    }
    // If debug, we print a file to compare with expected one from test
    if(debug){
        std::string name_out = type_file+"_lexed_debug_file.txt";
        std::ofstream file_to_write(name_out, std::ofstream::out);
        if(! file_to_write.is_open()){
            std::cout << "[FATAL_ERROR][LEXER] Impossible to open output file"<< std::endl;
            exit(1);
        }
        for(int i=0; i<my_parsed_file.size(); i++){
            file_to_write << my_parsed_file[i];
            if(i != (my_parsed_file.size()-1)){
                file_to_write << "\n";
            }
        }
        file_to_write.close();
    }

    // Check if no problem inside lexer
    // -> Json file is tested inside lexer directly
    if(type_file == "dot"){
        check_syntaxe_dot(my_parsed_file, error_report);
    }

    // If errors detected, print them and stop the program
    if(error_report.size() != 0){
        std::cout << "[FATAL_ERROR][LEXER] File isn't correct check report bellow :" << std::endl;
        for(int i=0; i<error_report.size(); i++){
            std::cout << " ----- "<< error_report[i] << std::endl;
        }
        exit(1);
    }
    file_lex.close();
}

/* check_syntaxe_dot()
    -> check if size > 4 : digraph name { }
    -> check if first word is digraph
    -> check if -> are done correctly
    -> check if name doesn't start with numbers
*/
void check_syntaxe_dot(std::vector<std::string> &my_parsed_file, std::vector<std::string> &error_report){
    // 0 -> if size of dot is less than 4 throw error < digraph name { } >
    if(my_parsed_file.size() < 4){
        error_report.push_back("[FATAL][ERROR][LEXER][dot] file is too small to be .DOT \" size < 4 \"");
    }

    // I -> first word is 'Digraph' or 'digraph' and no name after
    for(int i=0; i<my_parsed_file[0].size(); i++){
        my_parsed_file[0][i] = tolower(my_parsed_file[0][i]);
    }
    if(my_parsed_file[0] != "digraph"){
        error_report.push_back("[ERROR][LEXER][dot] file didn't start with digraph\n -------> name wrote : "+my_parsed_file[0]);
    }
    if(my_parsed_file[1] == "{"){
        error_report.push_back("[ERROR][LEXER][dot] empty word after digraph\n");
    }
    if(my_parsed_file[2] != "{"){
        error_report.push_back("[ERROR][LEXER][dot] too many args for name check if \" { \" oppened corretly \n");
    }

    // II -> case with -> dispatched
    for(int i=0; i<my_parsed_file.size(); i++){
        switch(my_parsed_file[i][0]){
            case '-': {
                if(my_parsed_file[i].size() == 2 && my_parsed_file[i][1] == '>'){
                    break;
                } else{
                    error_report.push_back("[ERROR][LEXER][dot] arrow syntax isn't wrote correctly '-'\n");
                }

            }
            case '>': {
                error_report.push_back("[ERROR][LEXER][dot] arrow syntax isn't wrote correctly '>'\n");
            }
            default :{
                break;
            }
        }
    }
    for(int i=0; i<my_parsed_file.size(); i++){
        for(int j=1; j<my_parsed_file[i].size(); i++){
            if(my_parsed_file[i][j] == '-'){
                error_report.push_back("[ERROR][LEXER][dot] arrow syntax isn't wrote correctly '-' is in a random location in word\n");
            }
        }
    }

    // III -> Detect wrong symbol at beginning (number | $ | __2 )
    for(int i=0; i<my_parsed_file.size(); i++){
        switch(my_parsed_file[i][0]){
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':{
                error_report.push_back("[ERROR][LEXER][dot] word can't start with number\n -------> name wrote : "+my_parsed_file[i]);
            }
            default:{
                break;
            }
        }
    }
}