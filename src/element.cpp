#include "element.h"

int element::current_delta_cycle = 0;
bool element::done_first_column = 0;
/*
    -> element class function definition
*/
void element::connect_new_element_input(element* node)
{
    this->m_inputs.push_back(node);
}
void element::connect_new_element_output(element* node)
{
    this->m_outputs.push_back(node);
}
void element::remove_element_input(std::string name)
{
    // TO DO but not used
}
void element::remove_element_output(std::string name)
{
    // TO DO but not used
}


/* OUTPUT related methods
    -> always one input
*/
int output::get_output(){
    if(already_been_here == 0){ // First time going in
        this->already_been_here = 1;
        this->past_value = m_inputs[0]->get_output();
        std::cout << "        [OUTPUT][get_output] name : " << this->get_name()<<" Output value : "<< this->past_value << " Already_been_here : 0"<< std::endl;
        return past_value;
    } else {
        if(current_delta_cycle > 0 || done_first_column == 1){
            std::cout << "        [OUTPUT][get_output] Name : " << this->get_name()<<" Output value : "<< this->past_value << " Already_been_here : 1 Done_first_column : "<< done_first_column<< std::endl;
            return past_value; // Return value from last iteration
        } else {
            std::cout << "        [OUTPUT][get_output] Name : " << this->get_name()<<" Output value : "<< this->past_value << " Already_been_here : 1 Done_first_column : "<< done_first_column<< std::endl;
            return 0; // Return smallest value of type
        }
    }
}

/*
    AND GATE related methods
*/
void and_gate::calcul_gate(){
    int temp_calcul = 0;
    for(int i_in=0; i_in<m_inputs.size(); i_in++){
        temp_calcul += m_inputs[i_in]->get_output(); // if return -1 ? would it be possible ?
    }
    if(temp_calcul == m_inputs.size()){ // AND GATE
        this->m_output = 1;
    } else {
        this->m_output = 0;
    }
    std::cout << "        [AND_GATE][calcul_gate] Name : " << this->get_name()<<" Output value : "<< this->m_output << " Inputs size : "<< m_inputs.size() << std::endl;
}

int and_gate::get_output(){
    if(already_been_here == 0){ // First time going in
        this->already_been_here = 1;
        this->calcul_gate();
        this->past_value = this->m_output;
        std::cout << "        [AND_GATE][get_output] Name : " << this->get_name()<<" Output value : "<< this->past_value << " Already_been_here : 0 Done_first_column : "<< done_first_column<< std::endl;
        return past_value;
    } else {
        if(current_delta_cycle > 0 || done_first_column == 1){
            std::cout << "        [AND_GATE][get_output] name : " << this->get_name()<<" Output value : "<< this->past_value << " Already_been_here : 1 Done_first_column : "<< done_first_column<< std::endl;
            return past_value; // Return value from last iteration
        } else {
            std::cout << "        [AND_GATE][get_output] Name : " << this->get_name()<<" Output value : 0 Already_been_here : 1 Done_first_column : "<< done_first_column<< std::endl;
            return 0; // Return smallest value of type
        }
    }
}

/*
    NOT GATE related methods
*/
void not_gate::calcul_gate(){
    int temp_calcul = 0;
    temp_calcul = m_inputs[0]->get_output();
    if(temp_calcul == 0){
        this->m_output = 1;
    } else {
        this->m_output = 0;
    }
    std::cout << "        [NOT_GATE][calcul_gate] Name : " << this->get_name()<<" Output value : "<< this->m_output << std::endl;
}

int not_gate::get_output(){
    if(already_been_here == 0){ // First time going in
        this->already_been_here = 1;
        this->calcul_gate();
        this->past_value = this->m_output;
        std::cout << "        [NOT_GATE][get_output] Name : " << this->get_name()<<" Output value : "<< this->past_value << " Already_been_here : 0 Done_first_column : "<< done_first_column<< std::endl;
        return past_value;
    } else {
        if(current_delta_cycle > 0 || done_first_column == 1){
            std::cout << "        [NOT_GATE][get_output] name : " << this->get_name()<<" Output value : "<< this->past_value << " Already_been_here : 1 Done_first_column : "<< done_first_column<< std::endl;
            return past_value; // Return value from last iteration
        } else{
            std::cout << "        [NOT_GATE][get_output] name : " << this->get_name()<<" Output value : 0 Already_been_here : 1 Done_first_column : "<< done_first_column<< std::endl;
            return 0; // Return smallest value of type
        }
    }
}

/*
    NAND GATE related methods
*/
void nand_gate::calcul_gate(){
    int temp_calcul = 0;
    for(int i_in=0; i_in<m_inputs.size(); i_in++){
        temp_calcul += m_inputs[i_in]->get_output();
    }
    if(temp_calcul == m_inputs.size()){
        this->m_output = 0;
    } else {
        this->m_output = 1;
    }
    std::cout << "        [NAND_GATE][calcul_gate] Name : " << this->get_name()<<" Output value : "<< this->m_output << " Inputs size : "<< m_inputs.size() << std::endl;
}

int nand_gate::get_output(){
    if(already_been_here == 0){ // First time going in
        this->already_been_here = 1;
        this->calcul_gate();
        this->past_value = this->m_output;
        std::cout << "        [NAND_GATE][get_output] Name : " << this->get_name()<<" Output value : "<< this->past_value << " Already_been_here : 0 Done_first_column : "<< done_first_column<< std::endl;
        return past_value;
    } else {
        if(current_delta_cycle > 0 || done_first_column == 1){
            std::cout << "        [NAND_GATE][get_output] name : " << this->get_name()<<" Output value : "<< this->past_value << " Already_been_here : 1 Done_first_column : "<< done_first_column<< std::endl;
            return past_value; // Return value from last iteration
        } else{
            std::cout << "        [NAND_GATE][get_output] name : " << this->get_name()<<" Output value : 0 Already_been_here : 1 Done_first_column : "<< done_first_column<< std::endl;
            return 0; // Return smallest value of type
        }
    }
}

/*
    OR GATE related methods
*/
void or_gate::calcul_gate(){
    int temp_calcul = 0;
    for(int i_in=0; i_in<m_inputs.size(); i_in++){
        temp_calcul += m_inputs[i_in]->get_output();
    }
    if(temp_calcul <= 0){
        this->m_output = 0;
    } else {
        this->m_output = 1;
    }
    std::cout << "        [OR_GATE][calcul_gate] Name : " << this->get_name()<<" Output value : "<< this->m_output << " Inputs size : "<< m_inputs.size() << std::endl;
}

int or_gate::get_output(){
    if(already_been_here == 0){ // First time going in
        this->already_been_here = 1;
        this->calcul_gate();
        this->past_value = this->m_output;
        std::cout << "        [OR_GATE][get_output] Name : " << this->get_name()<<" Output value : "<< this->past_value << " Already_been_here : 0 Done_first_column : "<< done_first_column<< std::endl;
        return past_value;
    } else {
        if(current_delta_cycle > 0 || done_first_column == 1){
            std::cout << "        [OR_GATE][get_output] name : " << this->get_name()<<" Output value : "<< this->past_value << " Already_been_here : 1 Done_first_column : "<< done_first_column<< std::endl;
            return past_value; // Return value from last iteration
        } else{
            std::cout << "        [OR_GATE][get_output] name : " << this->get_name()<<" Output value : 0 Already_been_here : 1 Done_first_column : "<< done_first_column<< std::endl;
            return 0; // Return smallest value of type
        }
    }
}

/*
    NOR GATE related methods
*/
void nor_gate::calcul_gate(){
    int temp_calcul = 0;
    for(int i_in=0; i_in<m_inputs.size(); i_in++){
        temp_calcul += m_inputs[i_in]->get_output();
    }
    if(temp_calcul <= 0){
        this->m_output = 1;
    } else {
        this->m_output = 0;
    }
    std::cout << "        [NOR_GATE][calcul_gate] Name : " << this->get_name()<<" Output value : "<< this->m_output << " Inputs size : "<< m_inputs.size() << std::endl;
}

int nor_gate::get_output(){
    if(already_been_here == 0){ // First time going in
        this->already_been_here = 1;
        this->calcul_gate();
        this->past_value = this->m_output;
        std::cout << "        [NOR_GATE][get_output] Name : " << this->get_name()<<" Output value : "<< this->past_value << " Already_been_here : 0 Done_first_column : "<< done_first_column<< std::endl;
        return past_value;
    } else {
        if(current_delta_cycle > 0 || done_first_column == 1){
            std::cout << "        [NOR_GATE][get_output] name : " << this->get_name()<<" Output value : "<< this->past_value << " Already_been_here : 1 Done_first_column : "<< done_first_column<< std::endl;
            return past_value; // Return value from last iteration
        } else{
            std::cout << "        [NOR_GATE][get_output] name : " << this->get_name()<<" Output value : 0 Already_been_here : 1 Done_first_column : "<< done_first_column<< std::endl;
            return 0; // Return smallest value of type
        }
    }
}

/*
    XOR GATE related methods
*/
void xor_gate::calcul_gate(){
    bool temp_global, temp_next;
    for(int i_in=0; i_in<m_inputs.size(); i_in++){
        if(i_in==0){
            temp_global = m_inputs[i_in]->get_output();
        } else{
            temp_next = m_inputs[i_in]->get_output();
            temp_global = temp_global^temp_next;
        }
    }
    this->m_output = temp_global;
    std::cout << "        [XOR_GATE][calcul_gate] Name : " << this->get_name()<<" Output value : "<< this->m_output << " Inputs size : "<< m_inputs.size() << std::endl;
}

int xor_gate::get_output(){
    if(already_been_here == 0){ // First time going in
        this->already_been_here = 1;
        this->calcul_gate();
        this->past_value = this->m_output;
        std::cout << "        [XOR_GATE][get_output] Name : " << this->get_name()<<" Output value : "<< this->past_value << " Already_been_here : 0 Done_first_column : "<< done_first_column<< std::endl;
        return past_value;
    } else {
        if(current_delta_cycle > 0 || done_first_column == 1){
            std::cout << "        [XOR_GATE][get_output] name : " << this->get_name()<<" Output value : "<< this->past_value << " Already_been_here : 1 Done_first_column : "<< done_first_column<< std::endl;
            return past_value; // Return value from last iteration
        } else{
            std::cout << "        [XOR_GATE][get_output] name : " << this->get_name()<<" Output value : 0 Already_been_here : 1 Done_first_column : "<< done_first_column<< std::endl;
            return 0; // Return smallest value of type
        }
    }
}

/*
    MUX GATE related methods
*/
void mux_gate::calcul_gate(){
    int index_to_target = 0;
    for(int i_sel=0; i_sel<this->m_selectors.size(); i_sel++){
        index_to_target += this->m_selectors[i_sel]->get_output() * pow(2, i_sel);
    }
    if(index_to_target+1 > m_inputs.size()){
        this->m_output = 0;
        std::cout << "        [MUX_GATE][calcul_gate] Name : " << this->get_name()<<" Index to target isn't inside inputs -> Output value : "<< this->m_output << " Inputs size : "<< m_inputs.size() << std::endl;
    } else {
        this->m_output = m_inputs[index_to_target]->get_output();
        std::cout << "        [MUX_GATE][calcul_gate] Name : " << this->get_name()<<" Index to target : "<< index_to_target << " Gate : "<< m_inputs[index_to_target]->get_name() <<" Output value : "<< this->m_output << " Inputs size : "<< m_inputs.size() << std::endl;
    }
}

int mux_gate::get_output(){
    if(already_been_here == 0){ // First time going in
        this->already_been_here = 1;
        this->calcul_gate();
        this->past_value = this->m_output;
        std::cout << "        [MUX_GATE][get_output] Name : " << this->get_name()<<" Output value : "<< this->past_value << " Already_been_here : 0 Done_first_column : "<< done_first_column<< std::endl;
        return past_value;
    } else {
        if(current_delta_cycle > 0 || done_first_column == 1){
            std::cout << "        [MUX_GATE][get_output] name : " << this->get_name()<<" Output value : "<< this->past_value << " Already_been_here : 1 Done_first_column : "<< done_first_column<< std::endl;
            return past_value; // Return value from last iteration
        } else{
            std::cout << "        [MUX_GATE][get_output] name : " << this->get_name()<<" Output value : 0 Already_been_here : 1 Done_first_column : "<< done_first_column<< std::endl;
            return 0; // Return smallest value of type
        }
    }
}

/*
    FF GATE related methods
*/
void ff_gate::update_next_value(){
    this->past_value = this->next_value;
    this->next_value = m_inputs[0]->get_past_value();
    std::cout << "        [FF_GATE][update_next_value] Name : " << this->get_name()<<" Next value : "<< this->next_value << std::endl;
}
int ff_gate::get_output(){
    // First time we go insid
    if(!done_first_column){
        std::cout << "        [FF_GATE][get_output] Name : " << this->get_name()<<" Output value : "<< 0 << std::endl;
        return 0;
    } else {
        std::cout << "        [FF_GATE][get_output] Name : " << this->get_name()<<" Output value : "<< this->next_value << std::endl;
        return this->next_value;
    }
}