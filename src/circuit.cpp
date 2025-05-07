#include "circuit.h"

/* find_by_name()
    -> return pointor corresponding to the targeted name
    -> if not found return null_ptr
*/
element* circuit::find_by_name(std::string name){
    for(int i=0; i<this->m_all_gates.size(); i++){
        if(m_all_gates[i]->get_name() == name){
            return m_all_gates[i];
        }
    }
    return nullptr;
}

/* add_element()
    -> create the wanted element depending on type
    -> TODO : Add flip flop
*/
void circuit::add_element(instance_NODE &Nodes){
    // if else with objects ?
    if(Nodes.type == "INPUT"){
        input* elem = new input(Nodes.name);
        this->m_all_gates.push_back(elem);

    } else if(Nodes.type == "OUTPUT"){
        output* elem = new output(Nodes.name);
        this->m_all_gates.push_back(elem);

    } else if(Nodes.type == "AND"){
        and_gate* elem = new and_gate(Nodes.name);
        this->m_all_gates.push_back(elem);

    } else if(Nodes.type == "NOT"){
        not_gate* elem = new not_gate(Nodes.name);
        this->m_all_gates.push_back(elem);

    } else if(Nodes.type == "NAND"){
        nand_gate* elem = new nand_gate(Nodes.name);
        this->m_all_gates.push_back(elem);

    } else if(Nodes.type == "OR"){
        or_gate* elem = new or_gate(Nodes.name);
        this->m_all_gates.push_back(elem);

    } else if(Nodes.type == "NOR"){
        nor_gate* elem = new nor_gate(Nodes.name);
        this->m_all_gates.push_back(elem);

    } else if(Nodes.type == "XOR"){
        xor_gate* elem = new xor_gate(Nodes.name);
        this->m_all_gates.push_back(elem);

    } else if(Nodes.type == "MUX"){
        mux_gate* elem = new mux_gate(Nodes.name);
        this->m_all_gates.push_back(elem);

    } else if(Nodes.type == "FF"){
        ff_gate* elem = new ff_gate(Nodes.name);
        this->m_all_gates.push_back(elem);

    } else {
        std::cout << "[FATAL_ERROR][CIRCUIT_SIMULATION] type : "<<Nodes.type<< " doesn't exist ";
        exit(1);
    }
}

/* connect_elements()
    -> for each inputs in our Nodes, connect the correct pointor
    -> TODO: for each outputs but not used
*/
void circuit::connect_elements(instance_NODE &Nodes){
    // Inputs connection
    for(int n_in=0; n_in<Nodes.outputs.size(); n_in++){
        for(int n_elem=0; n_elem<this->m_all_gates.size(); n_elem++){
            if(Nodes.outputs[n_in] == m_all_gates[n_elem]->get_name()){
                m_all_gates[n_elem]->connect_new_element_input(find_by_name(Nodes.name));
            }
        }
    }
}

/* connect_selectors()
    -> this function is used for linking selector pointors to other elements
*/
void circuit::connect_selectors(instance_NODE &Nodes){
    // selectors connection
    //std::cout << "SELECTOR CONNECT size : "<<Nodes.selectors.size()<< std::endl;
    for(int n_in=0; n_in<Nodes.selectors.size(); n_in++){
        //std::cout << "MUX adding selector "<< Nodes.name << " input name "<< Nodes.selectors[n_in] <<std::endl;
        for(int n_elem=0; n_elem<this->m_all_gates.size(); n_elem++){
            if(Nodes.name == m_all_gates[n_elem]->get_name()){
                m_all_gates[n_elem]->add_selectors(find_by_name(Nodes.selectors[n_in]));
            }
        }
    }
}

/* clear_element()
    -> clean each elements with delete[]
*/
void circuit::clear_element(){
    for(int i=0; i<m_all_gates.size(); i++){
        delete m_all_gates[i];
    }
}

/* reset_circuit()
    -> call clear_output() for each gates
*/
void circuit::reset_circuit(){
    for(int i=0; i<this->m_all_gates.size(); i++){
        m_all_gates[i]->clear_output();
    }
}

/* grab_outputs()
    -> store values in a vector of string corresponding to name of OUTPUT
*/
void circuit::grab_outputs(){
    name_outputs.clear();
    for(int i=0; i<this->m_all_gates.size(); i++){
        if(m_all_gates[i]->get_type() == "OUTPUT"){
            name_outputs.push_back(m_all_gates[i]->get_name());
        }
    }
}

/* assign_parser_dot()
    -> calling add_element() to instantiate gates
    -> calling connect_elements() to make link with pointors
    -> calling connect_selectors() if the type is mux
*/
void circuit::assign_parser_dot(std::vector<instance_NODE> &Nodes){
    // Creation of all gates
    for(int n_elem=0; n_elem<Nodes.size(); n_elem++){
        add_element(Nodes[n_elem]);
    }
    // Connecting pointors together
    for(int n_elem=0; n_elem<Nodes.size(); n_elem++){
        connect_elements(Nodes[n_elem]);
    }
    // Connecting MUX selectors together
    for(int n_elem=0; n_elem<Nodes.size(); n_elem++){
        if(Nodes[n_elem].type == "MUX"){
            connect_selectors(Nodes[n_elem]);
        }
    }
}

/* assign_parser_json()
    -> Store in both vectors
    -> Two verification are done
        -> check if inputs name exist
        -> check if there is same number of inputs from dot & json file
*/
void circuit::assign_parser_json(std::vector<std::string> &Nodes){
    int cpt_in=0;
    name_inputs.clear();
    value_inputs.clear();
    for(int i=0; i<Nodes.size(); i+=2){
        name_inputs.push_back(Nodes[i]);
        value_inputs.push_back(Nodes[i+1]);
    }
    // Check if inputs exist in our gates
    for(int i_num=0; i_num<name_inputs.size(); i_num++){
        if(find_by_name(name_inputs[i_num]) == nullptr){
            std::cout << "[FATAL_ERROR][CIRCUIT][assign_parser_json] inputs from json : " << name_inputs[i_num] <<" doesn't exist in dot file : "<< std::endl;
            exit(1);
        }
    }
    // check if all inputs are declared
    for(int i_num=0; i_num<this->m_all_gates.size();i_num++){
        //std::cout << " TYPE : " << this->m_all_gates[i_num]->get_type()<< std::endl;
        if(this->m_all_gates[i_num]->get_type() == "INPUT"){
            cpt_in+=1;
        }
    }
    if(cpt_in != Nodes.size()/2){
        std::cout << "[FATAL_ERROR][CIRCUIT][assign_parser_json] inputs from dot : " << cpt_in <<" inputs from json : " << Nodes.size()/2 <<" INCOHERENT" << std::endl;
        exit(1);
    }
}

/* simulate_OUT_one()
    -> clear the circuit
    -> store all inputs in required ones for our gates
    -> compare current values and last one simulated -> if no diff you can stop here
    -> else stop function when reaching delta_cycle
*/
void circuit::simulate_OUT_one(std::vector<int> &value_out, int position_input){
    std::vector<int> previous_out, current_out;

    // Clear all gates
    value_out.clear();
    reset_circuit();

    // Store inputs in INPUT GATE
    for(int i_in=0; i_in<name_inputs.size(); i_in++){
        if(value_inputs[i_in][position_input] == '1'){
            find_by_name(name_inputs[i_in])->import_input(1);
        } else{
            find_by_name(name_inputs[i_in])->import_input(0);
        }
    }

    // Simulate until en of delta cycle
    std::cout << "[CIRCUIT][simulate_alone] delta_cycle size : "<<delta_cycle<<std::endl;
    for(int i_dc=0; i_dc<delta_cycle; i_dc++){
        element::set_delta_cycle(i_dc);
        std::cout << "    [CIRCUIT][simulate_alone] delta_cylce : "<<i_dc<<std::endl;

        if(i_dc == 0){
            // First case we instantiate
            for(int i_out=0; i_out<name_outputs.size(); i_out++){
                previous_out.push_back(find_by_name(name_outputs[i_out])->get_output());
            }
        } else {
            // Second case we replace
            current_out.clear();
            for(int i_out=0; i_out<name_outputs.size(); i_out++){
                current_out.push_back(find_by_name(name_outputs[i_out])->get_output());
            }
            // We can stop here, stabilization done
            if(current_out == previous_out){
                std::cout << "    [CIRCUIT][simulate_alone] done with simulation current = previous number of delta cycle : "<< i_dc <<std::endl;
                break;
            } else {
                // Continue until the reach of delta cycle
                std::cout << "    [CIRCUIT][simulate_alone] continue with next delta cycle current != previous"<<std::endl;
                previous_out.clear();
                previous_out = current_out;
            }
        }
        // used to increase flip flop values
        for(int i_num=0; i_num<m_all_gates.size(); i_num++){
            m_all_gates[i_num]->update_next_value();
        }
        // Printing debugging function
        std::cout<< "\n"<< std::endl;
        for(int i_out=0; i_out<name_outputs.size(); i_out++){
            std::cout << "    [CIRCUIT][simulate_alone] Delta cycle : "<< i_dc << " Output : "<<name_outputs[i_out]<<" Value : "<<previous_out[i_out]<<std::endl;
        }
    }
    value_out = current_out;
}

/* simulate_OUT_full()
    -> reset all and calcul every values for each inputs columns
*/
void circuit::simulate_OUT_full(){

    // Assign correct outputs
    std::vector<int> val_out;
    value_outputs.clear();
    grab_outputs();

    std::cout << "\n------[CIRCUIT][simulate_full][START]------\n"<<std::endl;
    std::cout << "[CIRCUIT][INFO][simulate_full] Number of Output to calcul : "<< name_outputs.size()<<std::endl;
    std::cout << "[CIRCUIT][INFO][simulate_full] Inputs signal iteration : "<< value_inputs[0].size()<<"\n"<<std::endl;

    // GO for size of inputs and store each columns
    for(int i_column=0; i_column<value_inputs[0].size(); i_column++){
        std::cout << "\n\n-------------- [CIRCUIT][simulate_full] Column number : "<< i_column <<" ----------------"<<std::endl;

        // Reseting each gates after delta cycle
        for(int i=0; i<m_all_gates.size(); i++){
            m_all_gates[i]->clear_been_here();
        }
        this->simulate_OUT_one(val_out, i_column);

        // store each lines
        for(int i_line=0; i_line<name_outputs.size();i_line++){
            if(i_column==0){
                value_outputs.push_back(std::to_string(val_out[i_line]));
                std::cout << "\n[CIRCUIT][simulate_full] Output : "<< name_outputs[i_line]<< " Value "<< value_outputs[i_line][i_column]<<std::endl;
            } else {
                value_outputs[i_line] += std::to_string(val_out[i_line]);
                std::cout << "\n[CIRCUIT][simulate_full] Output : "<< name_outputs[i_line]<< " Value "<< value_outputs[i_line][i_column]<<std::endl;
            }
        }
        // Indicate elements that they can search for values N-1
        element::set_done_first_column(1);
    }
    std::cout << "\n------[CIRCUIT][simulate_full][END]------"<<std::endl;
}

/* print_out()
    -> used for debugging, display every connection
    -> inputs and outputs
*/
void circuit::print_out(){
    std::cout << "\n------[CIRCUIT][PRINT]------" << std::endl;
    std::cout << "[INPUTS] :" << std::endl;
    for(int i=0; i<name_inputs.size(); i++){
        std::cout << name_inputs[i] << "\n" << value_inputs[i] << std::endl;
    }
    std::cout << "[OUTPUTS] :" << std::endl;
    for(int i=0; i<name_outputs.size(); i++){
        std::cout << name_outputs[i] << "\n" << value_outputs[i] << std::endl;
    }
    std::cout << "------[CIRCUIT][PRINT]------" << std::endl;
}

/* print_to_json()
    -> used for creating json file with inputs and outputs
*/
void circuit::print_to_json(std::string &file_name){
    std::vector<std::string> data_to_send;
    for(int i=0; i<name_inputs.size(); i++){
        data_to_send.push_back(name_inputs[i]);
        data_to_send.push_back(value_inputs[i]);
    }
    for(int i=0; i<name_outputs.size(); i++){
        data_to_send.push_back(name_outputs[i]);
        data_to_send.push_back(value_outputs[i]);
    }
    generateOutputFile(data_to_send, file_name);
}