#include "parser_json.h"

void get_parsed_json(std::vector<std::string> &input_JSON_Parser, std::vector<std::string> &output_JSON_Parser, bool debug_activate = 0){

    struct FlagStruct f; // Initialize a struct that contains flags to verify the input

    std::vector<std::string> error_list; // Used to store error data and print it at the end of the program

    int cursor = 1; // Used to navigate through the different lines in the file
    int parsingState = 0; // Used to specify the state of the FSM e.g the parsing of an element in a file
    int maxValueSize = 0; // Used to format data after the retrieving

    std::string labelTester; // Used to verify if two signals have the same name



/////////////// BEGIN OF THE FINISHED STATE MACHINE FOR SORTING/PARSING THE DATA WITHOUT FORMATTING ///////////////

    for(const std::string& strParse : input_JSON_Parser){ // Scanning each line of the file, put it in the string "strParse"

        switch(parsingState){
            case 0:                 // First state of FSM : verification of the header : "{ signal: ["
                switch(cursor){
                    case 1:
                        if(strParse == "{"){
                            f.bracket++;
                        }
                        else{
                            std::cout << "[FATAL_ERROR][PARSER][JSON][Syntax] type : First character should be a \"{\" and not : " << strParse << std::endl;
                            exit(1);
                        }
                        break;
                    case 2:
                        if(strParse != "signal"){
                            std::cout << "[FATAL_ERROR][PARSER][JSON][Syntax] type : Header should include a \"signal\" and not : " << strParse << std::endl;
                            exit(1);
                        }
                        break;
                    case 3:
                        if(strParse != ":"){
                            std::cout << "[FATAL_ERROR][PARSER][JSON][Syntax] type : Header should include a \":\" and not : " << strParse << std::endl;
                            exit(1);
                        }
                        break;
                    case 4:
                        if(strParse == "["){
                            f.hook++;
                            parsingState = 1;
                            cursor = 0;
                        }
                        else{
                            std::cout << "[FATAL_ERROR][PARSER][JSON][Syntax] type : Header should include a \"[\" and not : " << strParse << std::endl;
                            exit(1);
                        }
                        break;
                    default:
                        break;
                }
                cursor++;
                break;
            case 1: // Second state of FSM : verification of the element : "{ name: "X" , wave : "1001"}"
                switch(cursor){
                    case 1:
                        if(strParse == "{"){
                            f.bracket++;
                        }
                        else if(strParse == "]"){ // Additionnal warning to tell the user that the last element should not follow a ","
                            f.hook--;
                            parsingState = 2;
                            cursor = 0;
                            std::cout << "[WARNING][PARSER][JSON][Syntax] type : The last element should not include a  \",\"."<< std::endl;
                        }
                        else{
                            std::cout << "[FATAL_ERROR][PARSER][JSON][Syntax] type : Element should start with a \"{\" and not : " << strParse << std::endl;
                            exit(1);
                        }
                        break;
                    case 2:
                        if(strParse == "}"){
                            f.bracket--;
                            cursor = 13;
                        }
                        else if(strParse != "name"){
                            std::cout << "[FATAL_ERROR][PARSER][JSON][Syntax] type : Element should include a \"name\" and not : " << strParse << std::endl;
                            exit(1);
                        }
                        break;
                    case 3:
                        if(strParse != ":"){
                            std::cout << "[FATAL_ERROR][PARSER][JSON][Syntax] type : Element should include a \":\" and not : " << strParse << std::endl;
                            exit(1);
                        }
                        break;
                    case 4:
                        if(strParse == "\""){
                            f.d_bracket = 1;
                        }
                        else if(strParse == "\'"){
                            f.d_bracket = 0;
                        }
                        else{
                            std::cout << "[FATAL_ERROR][PARSER][JSON][Syntax] type : Element should include a \" \" \" or a \" \' \" and not : " << strParse << std::endl;
                            exit(1);
                        }
                        break;
                    case 5:
                        output_JSON_Parser.push_back(strParse);
                        break;
                    case 6:
                        if(strParse == "\""){
                            if(!f.d_bracket){
                                std::cout << "[FATAL_ERROR][PARSER][JSON][Syntax] type : Cannot mix \" \" \" and \" \' \" for data. " << std::endl;
                                exit(1);
                            }
                        }
                        else if(strParse == "\'"){
                            if(f.d_bracket){
                                std::cout << "[FATAL_ERROR][PARSER][JSON][Syntax] type : Cannot mix \" \" \" and \" \' \" for data. " << std::endl;
                                exit(1);
                            }
                        }
                        else{
                            std::cout << "[FATAL_ERROR][PARSER][JSON][Syntax] type : Element should include a \" \" \" or a \" \' \" and not : " << strParse << std::endl;
                            exit(1);
                        }
                        break;
                    case 7:
                        if(strParse != ","){
                            std::cout << "[FATAL_ERROR][PARSER][JSON][Syntax] type : Element should include a \",\" and not : " << strParse << std::endl;
                            exit(1);
                        }
                        break;
                    case 8:
                        if(strParse != "wave"){
                            std::cout << "[FATAL_ERROR][PARSER][JSON][Syntax] type : Element should include a \"wave\" and not : " << strParse << std::endl;
                            exit(1);
                        }
                        break;
                    case 9:
                        if(strParse != ":"){
                            std::cout << "[FATAL_ERROR][PARSER][JSON][Syntax] type : Element should include a \":\" and not : " << strParse << std::endl;
                            exit(1);
                        }
                        break;
                    case 10:
                        if(strParse == "\""){
                            f.d_bracket = 1;
                        }
                        else if(strParse == "\'"){
                            f.d_bracket = 0;
                        }
                        else{
                            std::cout << "[FATAL_ERROR][PARSER][JSON][Syntax] type : Element should include a \" \" \" or a \" \' \" and not : " << strParse << std::endl;
                            exit(1);
                        }
                        break;
                    case 11:
                        if(strParse.size() > maxValueSize){
                            maxValueSize = strParse.size();
                        }
                        output_JSON_Parser.push_back(strParse);
                        break;
                    case 12:
                        if(strParse == "\""){
                            if(!f.d_bracket){
                                std::cout << "[FATAL_ERROR][PARSER][JSON][Syntax] type : Cannot mix \" \" \" and \" \' \" for data. " << std::endl;
                                exit(1);
                            }
                        }
                        else if(strParse == "\'"){
                            if(f.d_bracket){
                                std::cout << "[FATAL_ERROR][PARSER][JSON][Syntax] type : Cannot mix \" \" \" and \" \' \" for data. " << std::endl;
                                exit(1);
                            }
                        }
                        else{
                            std::cout << "[FATAL_ERROR][PARSER][JSON][Syntax] type : Element should include a \" \" \" or a \" \' \" and not : " << strParse << std::endl;
                            exit(1);
                        }
                        break;
                    case 13:
                        if(strParse == "}"){
                            f.bracket--;
                        }
                        else{
                            std::cout << "[FATAL_ERROR][PARSER][JSON][Syntax] type : Element should end with a \"}\" and not : " << strParse << std::endl;
                            exit(1);
                        }
                        break;
                    case 14:
                        if(strParse == ","){
                            cursor = 0;
                        }
                        else if(strParse == "]"){ // Verification of the first part of the foot : "]"
                            f.hook--;
                            parsingState = 2;
                            cursor = 0;
                        }
                        else{
                            std::cout << "[FATAL_ERROR][PARSER][JSON][Syntax] type : Footer should start with a \"]\" and not : " << strParse << std::endl;
                            exit(1);
                        }
                        break;
                    default:
                        break;
                }
                cursor++;
                break;
            case 2: // Last state of FSM : verification of the second part of the foot : "}"
                switch(cursor){
                    case 1:
                        if(strParse == "}"){
                            f.bracket--;
                            parsingState = 3;
                            cursor = 0;
                        }
                        else{
                            std::cout << "[FATAL_ERROR][PARSER][JSON][Syntax] type : Footer should finish with a \"}\" and not : " << strParse << std::endl;
                            exit(1);
                        }
                        break;
                    default:
                        break;
                }
                cursor++;
                break;
            default:
                break;
            }
        }

//////////////////////////////////////// END OF THE FINISHED STATE MACHINE ////////////////////////////////////////


    if(f.bracket!=0 || f.hook!=0){ // Check if the number of opening brackets/hooks is equal to the closing brackets/hooks. Less likely to happen
        error_list.push_back("[ERROR][PARSER][JSON][Syntax] type : Invalid bracket/hook number. Number of brackets : " + std::to_string(f.bracket) + ", number of hooks : " + std::to_string(f.hook) + " \n");
        // std::cout << "brackets & hooks : " << f.bracket << " " << f.hook << std::endl; // Show a special message if there is an error related with brackets/hooks
    }


/////////////////////////////////////// BEGIN OF THE FORMATTING OF THE DATA ///////////////////////////////////////

    for(int j=1 ; j<(output_JSON_Parser.size()/2)+1 ; j++){ // We rescan each value contained in the non-formatted file

        labelTester = output_JSON_Parser[(j*2)-2]; // We initialize the label tester with the current signal label tested
        for(int k=1; k<(output_JSON_Parser.size()/2)+1; k++){ //We check each signal label in the file
            if(((k*2)-2)!=((j*2)-2)){ // (Except the one that we are testing !)
                if(labelTester == output_JSON_Parser[(k*2)-2]){ // If the label tester is identical to another signal label in the file, push an error in the error vector
                    error_list.push_back("[ERROR][PARSER][JSON][Data] type : Invalid data format. Two signals shouldn't have the same label. (label : "+output_JSON_Parser[(k*2)-2]+") \n");
                    break;
                }
            }
        }
            if((output_JSON_Parser[(j*2)-1][0] == '0') || (output_JSON_Parser[(j*2)-1][0] == '1')){} // If the data starts with a "." or something else, it is not OK
			else{
                error_list.push_back("[ERROR][PARSER][JSON][Data] type : Invalid data format number "+std::to_string(j)+" ("+output_JSON_Parser[(j*2)-2]+"). Data should start with a \"1\" or a \"0\" and not : "+ output_JSON_Parser[(j*2)-1][0] +" \n");
			}
        for(int c=1 ; c<maxValueSize ; c++){  // We scan each piece of data
            // std::cout << "index " << c << " output : " << output[(j*2)-1][c] << " size : " << output[(j*2)-1].size() << endl; // " output-1 : " <<output[(j*2)-1][c-1] <<
            if(c > (output_JSON_Parser[(j*2)-1].size()-1)){ // If the data size is actually smaller than the max value size we duplicate the last value to have the same size
                output_JSON_Parser[(j*2)-1] += output_JSON_Parser[(j*2)-1][output_JSON_Parser[(j*2)-1].size()-1];
            }
            else{
                if((output_JSON_Parser[(j*2)-1][c] != '0') && (output_JSON_Parser[(j*2)-1][c] != '1') && (output_JSON_Parser[(j*2)-1][c] != '.')){ // We check if the data is a "1" or a "0" or a "." and we do as follows
                    error_list.push_back("[ERROR][PARSER][JSON][Data] type : Invalid data format number "+std::to_string(j)+" ("+output_JSON_Parser[(j*2)-2]+"). Data should include a \"1\", a \"0\" or a \".\" and not : "+ output_JSON_Parser[(j*2)-1][c] +" \n");
                }
                else if(output_JSON_Parser[(j*2)-1][c] == '.'){
                    output_JSON_Parser[(j*2)-1][c] = output_JSON_Parser[(j*2)-1][c-1];
                }
            }
        }
    }

//////////////////////////////////////// END OF THE FORMATTING OF THE DATA ////////////////////////////////////////

    if(error_list.size() != 0){ // We print the errors registered in the vector and we stop the program
        for(const std::string& strError : error_list){
            std::cout << strError;
        }
        exit(1);
    }
    else{
        std::cout << "[INFO][PARSER][JSON] JSON file parsed successfully. \n";
        if(debug_activate){
            std::ofstream outputFile("json_parsed_debug_file.txt"); // Create an empty file for the output
            std::ostream_iterator<std::string> output_iterator(outputFile, "\n"); // Create an iterator to put "\n" char (in string format) after each data
            copy(begin(output_JSON_Parser), end(output_JSON_Parser), output_iterator); // Copy the content of the output in the file
            outputFile.close();
        }
        return;
    }
}