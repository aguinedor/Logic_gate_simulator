#include <iostream>
#include <string>
#include <vector>

#include "lexer_json_dot.h"
#include "parser_dot.h"
#include "parser_json.h"
#include "circuit.h"

using namespace std;

int main(int argc, char** argv) {
    bool debug_activate = 0;
    string dot_name_file, json_name_file;
    string dot_type = "dot", json_type = "json";

    vector<string> my_dot_lexed_file;
    vector<string> my_json_lexed_file;
    std::vector<instance_NODE> Nodes;
    vector<string> input_values;

    string name_circ = "output_simulation.json";
    circuit my_circuit;

    if(argc > 2){
        dot_name_file = argv[1];
        json_name_file = argv[2];
    } else{
        std::cout << "[FATAL_ERROR] args not founded" << std::endl;
        exit(EXIT_FAILURE);
    }

    get_lexed(dot_name_file, my_dot_lexed_file, dot_type, debug_activate);
    std::cout << "\n[MAIN] size of lexe dot "<< my_dot_lexed_file.size() << std::endl;
    for(int i=0; i<my_dot_lexed_file.size(); i++){
        std::cout << my_dot_lexed_file[i]<<std::endl;
    }
    get_parsed_dot(my_dot_lexed_file, Nodes); // GET NAME
    std::cout << "\n[MAIN] size of parsed dot "<< Nodes.size() << std::endl;
    for(int i=0; i<Nodes.size(); i++){
        Nodes[i].display();
    }

    get_lexed(json_name_file, my_json_lexed_file, json_type, debug_activate);
    std::cout << "\n[MAIN] size of lexe json "<< my_dot_lexed_file.size() << std::endl;
    for(int i=0; i<my_json_lexed_file.size(); i++){
        std::cout << my_json_lexed_file[i]<<std::endl;
    }
    get_parsed_json(my_json_lexed_file, input_values, debug_activate);
    std::cout << "\n[MAIN] size of parsed json "<< Nodes.size() << std::endl;
    for (int i = 1; i < input_values.size(); i+=2)
    {
        std::cout << input_values[i]<<std::endl;
    }

    std::cout << "\n[MAIN] START SIMULATOR " << std::endl;
    std::cout << "[MAIN] before simulation"<< std::endl;
    my_circuit.set_name(name_circ);
    my_circuit.assign_parser_dot(Nodes);
    my_circuit.assign_parser_json(input_values);

    my_circuit.print_out();
    my_circuit.simulate_OUT_full();
    std::cout << "\n[MAIN] after simulation"<< std::endl;
    my_circuit.print_out();
    my_circuit.print_to_json(name_circ);
    std::cout << "\n[MAIN] END SIMULATOR " << std::endl;
    return 0;
}