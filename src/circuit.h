#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <vector>
#include <string>
#include <iostream>

#include "parser_dot.h"
#include "element.h"
#include "output_file_generator.h"

class circuit{

    private:
        std::string m_name;
        std::vector<element*> m_all_gates;

        // Default delta cycle is 10
        int delta_cycle = 10;
        // those two vectors have the same values
        std::vector<std::string> name_inputs;
        std::vector<std::string> value_inputs;

        std::vector<std::string> name_outputs;
        std::vector<std::string> value_outputs;

    public:
        circuit(std::string name = "circuit"): m_name(name){element::set_done_first_column(0);};
        ~circuit(){clear_element();};

        void set_name(std::string name){m_name = name;};

        // Return in our vector of element the pointor for specific name
        element* find_by_name(std::string name);
        // Add element in our list depending on name and type declaration
        void add_element(instance_NODE &Nodes);
        // Go for each elements in our vector and grab pointors from our list of input in Nodes
        // Haven't done it for outputs because it is not useful for the moment
        void connect_elements(instance_NODE &Nodes);
        void connect_selectors(instance_NODE &Nodes);

        void clear_element();

        // Clear outputs -> reset the whole circuit
        // -> set outputs of each gates to -1
        void reset_circuit();
        void grab_outputs();

        // Set the number of delta_cylce depending on number of gate
        void set_delta_cycle(){delta_cycle = 10 * m_all_gates.size();};
        // Call add_element() then connect_element()
        void assign_parser_dot(std::vector<instance_NODE> &Nodes);
        // store each values in inputs_values from nodes
        void assign_parser_json(std::vector<std::string> &Nodes);

        // Used to simulate for n delta cycle
        //  -> compare current values and last one simulated -> if no diff you can stop here
        //  -> else stop function when reaching delta_cycle
        void simulate_OUT_one(std::vector<int> &values_out, int position_input);
        // Used to call upper function in order to grab string of outputs
        void simulate_OUT_full(); // NAME VAL[] NAME VAL[] ....
        // Used to print a json output
        void print_to_json(std::string &file_name);
        // Used to print in terminal
        void print_out();
};
#endif