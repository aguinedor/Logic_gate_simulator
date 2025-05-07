#ifndef PARSER_JSON_H
#define PARSER_JSON_H

#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <fstream>

#include <iterator>
#include <string>
#include <vector>

void get_parsed_json(std::vector<std::string> &input_JSON_Parser, std::vector<std::string> &output_JSON_Parser, bool debug_activate);

struct FlagStruct{
    int hook = 0;
    int bracket = 0;
    int d_bracket = 0;
}; // This struct is used to evaluate whether the number of opening brackets/hooks is equal to the closing brackets/hooks.
   // It is eventually used if there is a variation in the input e.g the use of double brackets : "wave" and 'wave'.


#endif