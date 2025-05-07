#ifndef PARSER_DOT
#define PARSER_DOT

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <tgmath.h>

/* Obligation to consider
    -> All labels and types are wrote in caps LABEL INPUT AND$ ....
    -> you can use name such as INPUT, it is not forbidden

    -> in case you put AND2, only the AND is considered and the size is adapted direclty
    to the number of inputs connected to it

    -> complete with tests
*/
typedef struct instance_NODE_tag{
    std::string name;
    std::string type;
    std::vector<std::string> selectors;
    int size_in_max;
    std::vector<std::string> inputs;
    std::vector<std::string> outputs;

    void clear(){
        name.clear();
        type.clear();
        selectors.clear();
        size_in_max = 0;
        inputs.clear();
        outputs.clear();
    }
    void calc_size(){
        if(selectors.size() == 1){
            size_in_max = 2;
        } else {
            size_in_max = std::pow(selectors.size(),2);
        }
    }
    void display(){
        std::cout << "\nName is : " << name << " - type is : " << type;
        if(type == "MUX"){
            calc_size();
            std::cout << " - sel size : " << selectors.size() << " - max input : " << size_in_max;
            for(int i=0; i<selectors.size(); i++){std::cout << "\n SEL"<<i<<" is : "<< selectors[i];}
        }
        for(int i=0; i<outputs.size(); i++){
            std::cout << "\n OUTPUT"<<i<< " is : " << outputs[i];
        }
        for(int i=0; i<inputs.size(); i++){
            std::cout << "\n INPUT"<<i<< " is : " << inputs[i];
        }
        std::cout << std::endl;
    }
} instance_NODE;

typedef std::vector<std::vector<std::string>> parsed_matrice;

bool check_LABEL_in_list(std::string &string);
bool store_SEL_in_list(std::string &string);

int contains_name(const std::vector<instance_NODE> &my_node, const std::string &my_name);
void assign_connection(const parsed_matrice &connection_parsed, std::vector<instance_NODE> &Nodes);

// All check functions to use
void check_entrance_connection(const parsed_matrice &connection_parsed, std::vector<instance_NODE> &Nodes);
void check_reconnected_out(const std::vector<instance_NODE> &Nodes);
void check_missing_input_GATE(const std::vector<instance_NODE> &Nodes);
void check_missing_input_OUTPUT(const std::vector<instance_NODE> &Nodes);
void check_missing_output_INPUT(const std::vector<instance_NODE> &Nodes);
void check_one_input_NOT(const std::vector<instance_NODE> &Nodes);
void check_multiple_connection(const std::vector<instance_NODE> &Nodes);
void check_mux_input_sel(std::vector<instance_NODE> &Nodes);
void check_FF_input(std::vector<instance_NODE> &Nodes);
void check_atleast_one_input(std::vector<instance_NODE> &Nodes);
void check_atleast_one_output(std::vector<instance_NODE> &Nodes);
void check_outputs_out_gates(std::vector<instance_NODE> &Nodes);

// Classic FSM to parse the lexed dot file
void fsm_reading_instance(std::vector<std::string> &my_parsed_file, int &index_parse);
void fsm_reading_connection(std::vector<std::string> &my_parsed_file, int &index_parse);
void get_parsed_dot(std::vector<std::string> &my_parsed_file, std::vector<instance_NODE> &Nodes);

int contains(const std::vector<instance_NODE> &my_node, const std::string &my_name);
void assign_connection(const parsed_matrice &connection_parsed, std::vector<instance_NODE> &Nodes);

#endif