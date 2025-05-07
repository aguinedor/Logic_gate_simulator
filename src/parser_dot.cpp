#include "parser_dot.h"

/* contains_name
    -> Check if name already exist in vector
    -> if it exist return the index
    -> if it doesn't exist return -1
    -> if there is more than one return -2
*/
int contains_name(const std::vector<instance_NODE> &my_node, const std::string &my_name){
    int cpt = 0, index = 0;
    for (int i=0; i < my_node.size(); i++){
        if (my_node[i].name == my_name){
            cpt++;
            index = i;
        }
    }
    if(cpt == 1){return index;}
    if(cpt == 0){return -1;}
    return -2;
}

/* check_entrance_connection
    -> all inputs should be on left ->
*/
void check_entrance_connection(const parsed_matrice &connection_parsed, std::vector<instance_NODE> &Nodes){
    int temp_valid = -1;
    for(int i_line=0; i_line<connection_parsed.size(); i_line++){
        for(int i_con=0; i_con<connection_parsed[i_line].size(); i_con++){
            temp_valid = contains_name(Nodes, connection_parsed[i_line][i_con]);
            if(temp_valid == -1){std::cout << "[FATAL_ERROR][PARSER][dot][check_entrance_connection] name "<< connection_parsed[i_line][i_con] << " doesn't exist in dot file \n"; exit(1);}
            if(Nodes[temp_valid].type == "INPUT"){
                if(i_con != 0){std::cout << "[FATAL_ERROR][PARSER][dot][check_entrance_connection] input "<< connection_parsed[i_line][i_con] << " can't be connected after \" -> \" \n"; exit(1);}
            }
        }
    }
}

/* assign_connection
    -> Check if connection exist in instance return -1
    -> Assign inputs values
    -> Assign outputs values
*/
void assign_connection(const parsed_matrice &connection_parsed, std::vector<instance_NODE> &Nodes){
    // STORE CONNECTION
    int temp_valid = -1;
    for(int i_line=0; i_line<connection_parsed.size(); i_line++){
        // Check if connection exist in instance
        for(int i_con=0; i_con<connection_parsed[i_line].size(); i_con++){
            temp_valid = contains_name(Nodes, connection_parsed[i_line][i_con]);
            if(temp_valid == -1){std::cout << "[FATAL_ERROR][PARSER][dot][assign_connection] name "<< connection_parsed[i_line][i_con] << " doesn't exist in dot file \n"; exit(1);}
        }
        // Assign inputs values
        for(int i_con=1; i_con<connection_parsed[i_line].size(); i_con++){
            temp_valid = contains_name(Nodes, connection_parsed[i_line][i_con]);
            /*
            for(int i=0; i<Nodes[temp_valid].inputs.size(); i++){
                if(Nodes[temp_valid].inputs[i] == connection_parsed[i_line][i_con]){
                    std::cout << "[FATAL_ERROR][PARSER][dot][assign_connection] name "<<connection_parsed[i_line][i_con-1]<<" is assigned two times as inputs \n"; exit(1);
                }
            }
            */
            Nodes[temp_valid].inputs.push_back(connection_parsed[i_line][i_con-1]);
        }
        // Assign outputs values
        for(int i_con=0; i_con<connection_parsed[i_line].size()-1; i_con++){
            temp_valid = contains_name(Nodes, connection_parsed[i_line][i_con]);
            /*
            for(int i=0; i<Nodes[temp_valid].outputs.size(); i++){
                if(Nodes[temp_valid].outputs[i] == connection_parsed[i_line][i_con]){
                    std::cout << "[FATAL_ERROR][PARSER][dot][assign_connection] name "<<connection_parsed[i_line][i_con-1]<<" is assigned two times as outputs \n"; exit(1);
                }
            }
            */
            Nodes[temp_valid].outputs.push_back(connection_parsed[i_line][i_con+1]);
        }
    }
}

/* check_alone_input_element
    -> Outputs should be 1 input & N output
*/
void check_reconnected_out(const std::vector<instance_NODE> &Nodes){
    for(int i_node=0; i_node<Nodes.size(); i_node++){
        if((Nodes[i_node].type == "OUTPUT" || Nodes[i_node].type == "NOT") && Nodes[i_node].inputs.size()>1){
            std::cout << "[FATAL_ERROR][PARSER][dot][check_reconnected_out] output "<< Nodes[i_node].name << " have to many inputs \n";
            for(int i=0;i<Nodes[i_node].inputs.size(); i++){
                std::cout << " INPUT"<<i<< "  "<<Nodes[i_node].inputs[i]<<std::endl;
            }
            exit(1);
        }
    }
}

/* check_missing_input_GATE
    -> GATE have less than 1 input and one output
*/
void check_missing_input_GATE(const std::vector<instance_NODE> &Nodes){
    for(int i_node=0; i_node<Nodes.size(); i_node++){
        if(!(Nodes[i_node].type == "FF" || Nodes[i_node].type == "OUTPUT" || Nodes[i_node].type == "NOT" || Nodes[i_node].type == "INPUT") && Nodes[i_node].inputs.size()<2){
            std::cout << "[FATAL_ERROR][PARSER][dot][check_missing_input_GATE] less than one input for : " << Nodes[i_node].name << std::endl;
            exit(1);
        }
    }
}

/* check_missing_input_OUTPUT
    -> OUTPUT should have 1 input
*/
void check_missing_input_OUTPUT(const std::vector<instance_NODE> &Nodes){
    for(int i_node=0; i_node<Nodes.size(); i_node++){
        if(Nodes[i_node].type == "OUTPUT" && Nodes[i_node].inputs.size()!=1){
            std::cout << "[FATAL_ERROR][PARSER][dot][check_missing_input_OUTPUT] inputs size for OUTPUT "<< Nodes[i_node].name <<" is : " <<  Nodes[i_node].inputs.size() <<std::endl;
            exit(1);
        }
    }
}

/* check_missing_output_INPUT
    -> INPUT should have at least 1 output & no inputs
*/
void check_missing_output_INPUT(const std::vector<instance_NODE> &Nodes){
    for(int i_node=0; i_node<Nodes.size(); i_node++){
        if(Nodes[i_node].type == "INPUT" && Nodes[i_node].outputs.size()<1){
            // Check if in mux ?
            bool there_is = 0;
            for(int i_node_2=0; i_node_2 < Nodes.size(); i_node_2++){
                if(Nodes[i_node_2].type == "MUX"){
                    for(int j=0; j<Nodes[i_node_2].selectors.size(); j++){
                        if(Nodes[i_node_2].selectors[j] == Nodes[i_node].name){there_is = 1;}
                    }
                }
            }
            if(!there_is){
                std::cout << "[FATAL_ERROR][PARSER][dot][check_missing_output_INPUT] missing output for INPUT "<< Nodes[i_node].name << std::endl;
                exit(1);
            }
        }
        if(Nodes[i_node].type == "INPUT" && Nodes[i_node].inputs.size()!=0){
            std::cout << "[FATAL_ERROR][PARSER][dot][check_missing_output_INPUT] INPUT "<< Nodes[i_node].name << " shouldn't have inputs "<< std::endl;
            exit(1);
        }
    }
}

/* check_mux_input_sel
    -> check if inputs are superiors to selectors
*/
void check_mux_input_sel(std::vector<instance_NODE> &Nodes){
    for(int i_node=0; i_node<Nodes.size(); i_node++){
        if(Nodes[i_node].type == "MUX"){
            Nodes[i_node].calc_size();
            //std::cout << "\nMUX size : " << Nodes[i_node].size_in_max << "\n" <<std::endl;
            if(Nodes[i_node].inputs.size() > Nodes[i_node].size_in_max){
                std::cout << "[FATAL_ERROR][PARSER][dot][check_mux_input_sel] too many inputs for MUX "<< Nodes[i_node].name << std::endl;
                exit(1);
            }
        }
    }
}

/* check_FF_input
    -> check if FF have one input
*/
void check_FF_input(std::vector<instance_NODE> &Nodes){
    for(int i_node=0; i_node<Nodes.size(); i_node++){
        if(Nodes[i_node].type == "FF"){
            if(Nodes[i_node].inputs.size() != 1){
                std::cout << "[FATAL_ERROR][PARSER][dot][check_FF_input] FF should have one input only "<< Nodes[i_node].name << std::endl;
                exit(1);
            }
        }
    }
}

/* check_missing_input_GATE
    -> GATE have less than 1 input and one output
*/
void check_one_input_NOT(const std::vector<instance_NODE> &Nodes){
    for(int i_node=0; i_node<Nodes.size(); i_node++){
        if(Nodes[i_node].type == "NOT" && Nodes[i_node].inputs.size()!=1){
            std::cout << "[FATAL_ERROR][PARSER][dot][check_one_input_NOT] missing or to many inputs for NOT GATE : "<< Nodes[i_node].name << std::endl;
            exit(1);
        }
        if(Nodes[i_node].type == "NOT" && Nodes[i_node].outputs.size()<1){
            std::cout << "[FATAL_ERROR][PARSER][dot][check_one_input_NOT] missing outputs for NOT GATE : "<< Nodes[i_node].name << std::endl;
            exit(1);
        }
    }
}

/* check_multiple_connection
    -> if both connection exist in Nodes
*/
void check_multiple_connection(const std::vector<instance_NODE> &Nodes){
    for(int i_node=0; i_node<Nodes.size(); i_node++){
        if(!(Nodes[i_node].type == "OUTPUT" || Nodes[i_node].type == "NOT" || Nodes[i_node].type == "INPUT")){
            for(int i_vect1=0; i_vect1<Nodes[i_node].inputs.size(); i_vect1++){
            for(int i_vect2=0; i_vect2<Nodes[i_node].inputs.size(); i_vect2++){
                if(i_vect1 != i_vect2 && Nodes[i_node].inputs[i_vect1] == Nodes[i_node].inputs[i_vect2]){
                    std::cout << "[FATAL_ERROR][PARSER][dot][check_multiple_connection] repeating same inputs : "<< Nodes[i_node].inputs[i_vect2] <<"\n";
                    exit(1);
                }
            }}
        }
    }
}

/* check_atleast_one_input
    -> we should have at least one input
*/
void check_atleast_one_input(std::vector<instance_NODE> &Nodes){
    int cpt=0;
    for(int i=0; i<Nodes.size(); i++){
        if(Nodes[i].type == "INPUT"){cpt++;}
    }
    if(cpt==0){
        std::cout << "[FATAL_ERROR][PARSER][dot][check_atleast_one_input] Dot file should have at least one input" <<std::endl;
        exit(1);
    }
}

/* check_atleast_one_output
    -> we should have at least one output
*/
void check_atleast_one_output(std::vector<instance_NODE> &Nodes){
    int cpt=0;
    for(int i=0; i<Nodes.size(); i++){
        if(Nodes[i].type == "OUTPUT"){cpt++;}
    }
    if(cpt==0){
        std::cout << "[FATAL_ERROR][PARSER][dot][check_atleast_one_output] Dot file should have at least one output" <<std::endl;
        exit(1);
    }
}

/* check_outputs_out_gates
    -> outputs should only be connected to gate, not input or
*/
void check_outputs_out_gates(std::vector<instance_NODE> &Nodes){
    for(int i=0; i<Nodes.size(); i++){
        if(Nodes[i].type == "OUTPUT"){
            for(int j=0; j<Nodes[i].outputs.size(); j++){
                for(int k=0; k<Nodes.size(); k++){
                    if(Nodes[i].outputs[j] == Nodes[k].name){
                        if(Nodes[k].type == "OUTPUT" || Nodes[k].type == "INPUT"){
                            std::cout << "[FATAL_ERROR][PARSER][dot][check_outputs_out_gates] Output outs should only be connected to Gates (not input or output)" <<std::endl;
                            exit(1);
                        }
                    }
                }
            }
        }
    }
}

/* check_LABEL_in_list
    -> autorized type of gates
*/
bool check_LABEL_in_list(std::string &string){
    if( string == "INPUT" ||
        string == "OUTPUT" ||
        string == "AND" ||
        string == "OR" ||
        string == "XOR" ||
        string == "NOR" ||
        string == "MUX" ||
        string == "NAND" ||
        string == "FF" ||
        string == "NOT"){
        return 0;
    }
    return 1;
}

/* store_LABEL_in_list
    -> grab the type of instance, remove the number declared
    -> for example AND2 become AND and adapt to number of inputs
*/
void store_LABEL_in_list(std::string &string, instance_NODE &instance_node_parsed){
    std::string string_copy = string;
    std::string my_size_str;
    // C++ standard solution for erasing stuff from a container
    string.erase(std::remove_if(string.begin(), string.end(), ::isdigit), string.end());
    if(check_LABEL_in_list(string)){std::cout << "[FATAL_ERROR][PARSER][dot][store_LABEL_in_list] type : "<<string<<" doesn't exist in list of label \n"; exit(1);}
    instance_node_parsed.type = string;

    for(int i=0; i<string_copy.size(); i++){
        if(isdigit(string_copy[i])){
            my_size_str += string_copy[i];
        }
    }
}

/* fsm_reading_instance
    fsm used for ? [ LABEL = "?"]; for example
*/
void fsm_reading_instance(std::vector<std::string> &my_parsed_file, int &index_parse, instance_NODE &instance_node_parsed){
    enum reading_instance {GET_NAME, GET_WORD, GET_LABEL, GET_SEL, GET_INPUT, CHECK_END};
    reading_instance fsm_state = GET_NAME;
    unsigned int iteration_get = 0;

    while(my_parsed_file[index_parse] != ";"){
        switch(fsm_state){
            // in each case update index_parse to evolve in file
            // if didn't went trough GET_LABEL -> throw error
            // If gate is MUX$ with number=$ check if SEL >= log2(MUX$)
            case GET_NAME:{
                // Grab NAME
                // If next string is [      GO TO GET_WORD
                // If next string is other  THROW ERROR
                instance_node_parsed.name = my_parsed_file[index_parse];
                index_parse++;
                if (my_parsed_file[index_parse] != "["){std::cout << "[FATAL_ERROR][PARSER][dot][Reading_instance] open \"[\" aren't done correctly : "<<my_parsed_file[index_parse]<<"\n"; exit(1);}
                fsm_state = GET_WORD;
                break;
            }
            case GET_WORD: {
                // If next string is LABEL  GO TO GET_LABEL
                // If next string is SEL*   GO TO GET_SEL
                // If next string is INPUT* GO TO GET_INPUT
                // Else                     THROW ERROR
                //std::cout << "[DEBUG][GET_WORD] iteration_get : "<< iteration_get << " instance_node_parsed.type : " << instance_node_parsed.type << " instance_node_parsed.size_selectors : "<< instance_node_parsed.size_selectors << std::endl;
                if(iteration_get==0){
                    // Grab LABEL
                    index_parse++;
                    if(my_parsed_file[index_parse] != "LABEL"){std::cout << "[FATAL_ERROR][PARSER][dot][Reading_instance] no LABEL readed instead : "<<my_parsed_file[index_parse]<<"\n"; exit(1);}
                    fsm_state = GET_LABEL;
                    break;
                } else if(instance_node_parsed.type == "MUX"){
                    index_parse++;
                    if(my_parsed_file[index_parse].substr(0,3) != "SEL"){std::cout << "[FATAL_ERROR][PARSER][dot][Reading_instance] SEL for mux isn't declare correctly : "<<my_parsed_file[index_parse]<<"\n"; exit(1);}
                    fsm_state = GET_SEL;
                    break;
                } else {
                    std::cout << "[ERROR][PARSER][dot][Reading_instance] unexpected behaviour next word : "<<my_parsed_file[index_parse]<<"\n"; exit(1);
                }
            }
            case GET_LABEL:{
                // If next string is not =  THROW ERROR
                // If next string is not "  THROW ERROR
                // GRAB LABEL               CALL store_LABEL_in_list
                // If next string is "      GO TO CHECK_END
                // Else                     THROW ERROR
                index_parse++;
                if(my_parsed_file[index_parse] != "="){std::cout << "[FATAL_ERROR][PARSER][dot][Reading_instance][LABEL] no = readed instead : "<<my_parsed_file[index_parse]<<"\n"; exit(1);}
                index_parse++;
                if(my_parsed_file[index_parse] != "\""){std::cout << "[FATAL_ERROR][PARSER][dot][Reading_instance][LABEL] no \" readed at begin instead : "<<my_parsed_file[index_parse]<<"\n"; exit(1);}
                index_parse++;
                store_LABEL_in_list(my_parsed_file[index_parse], instance_node_parsed);
                index_parse++;
                if(my_parsed_file[index_parse] != "\""){std::cout << "[FATAL_ERROR][PARSER][dot][Reading_instance][LABEL] no \" readed at end instead : "<<my_parsed_file[index_parse]<<"\n"; exit(1);}
                iteration_get++;
                fsm_state = CHECK_END;
                break;
            }
            case GET_SEL:{
                // If next string is not =  THROW ERROR
                // If next string is not "  THROW ERROR
                // GRAB SEL$                call store_SEL_in_list
                // If next string is "      GO TO CHECK_END
                // Else                     THROW ERROR
                index_parse++;
                if(my_parsed_file[index_parse] != "="){std::cout << "[FATAL_ERROR][PARSER][dot][Reading_instance][SEL] no = readed instead : "<<my_parsed_file[index_parse]<<"\n"; exit(1);}
                index_parse++;
                if(my_parsed_file[index_parse] != "\""){std::cout << "[FATAL_ERROR][PARSER][dot][Reading_instance][SEL] no \" readed at begin instead : "<<my_parsed_file[index_parse]<<"\n"; exit(1);}
                index_parse++;
                instance_node_parsed.selectors.push_back(my_parsed_file[index_parse]);
                index_parse++;
                if(my_parsed_file[index_parse] != "\""){std::cout << "[FATAL_ERROR][PARSER][dot][Reading_instance][SEL] no \" readed at end instead : "<<my_parsed_file[index_parse]<<"\n"; exit(1);}
                iteration_get++;
                fsm_state = CHECK_END;
                break;
            }
            /*
            case GET_INPUT:{
                // If next string is not =  THROW ERROR
                // If next string is not "  THROW ERROR
                // GRAB INPUT$              call store_INPUT_in_list
                // If next string is "      GO TO CHECK_END
                // Else                     THROW ERROR
                index_parse++;
                if(my_parsed_file[index_parse] != "="){std::cout << "[FATAL_ERROR][PARSER][dot][Reading_instance][INPUT] no = readed instead : "<<my_parsed_file[index_parse]<<"\n"; exit(1);}
                index_parse++;
                if(my_parsed_file[index_parse] != "\""){std::cout << "[FATAL_ERROR][PARSER][dot][Reading_instance][INPUT] no \" readed at begin instead : "<<my_parsed_file[index_parse]<<"\n"; exit(1);}
                index_parse++;
                instance_node_parsed.inputs.push_back(my_parsed_file[index_parse]);
                index_parse++;
                if(my_parsed_file[index_parse] != "\""){std::cout << "[FATAL_ERROR][PARSER][dot][Reading_instance][INPUT] no \" readed at end instead : "<<my_parsed_file[index_parse]<<"\n"; exit(1);}
                iteration_get++;
                fsm_state = CHECK_END;
                break;
            }
            */
            case CHECK_END:{
                // If next string is ,      GO TO GET_WORD
                // If next string is ]      UPDATE NEXT VALUE AND BREAK WHILE(!";")
                // Else                     THROW ERROR
                index_parse++;
                //std::cout << "[DEBUG][FSM_READING_INSTANCE][CHECK_END] next data is : " << my_parsed_file[index_parse] << std::endl;
                if(my_parsed_file[index_parse] == ","){
                    fsm_state = GET_WORD;
                    break;
                } else if (my_parsed_file[index_parse] == "]"){
                    index_parse++;
                    if(my_parsed_file[index_parse] != ";"){std::cout << "[FATAL_ERROR][PARSER][dot][Reading_instance] missing ; after ] \n"; exit(1);}
                    break;
                } else {
                    std::cout << "[FATAL_ERROR][PARSER][dot][Reading_instance] instance isn't done correctly missing  \" , \" or \" ] \" \n"; exit(1);
                }
            }
        }
    }
}

/* fsm_reading_connection
    fsm used for ? -> ?; ... for example
*/
void fsm_reading_connection(std::vector<std::string> &my_parsed_file, int &index_parse, std::vector<std::string> &connection_parsed){
    enum reading_connection {GET_WIRE, GET_ARROW_WIRE, CHECK_END};
    reading_connection fsm_state = GET_WIRE;

    while(my_parsed_file[index_parse] != ";"){
        switch(fsm_state){
            // in each case update index_parse to evolve in file
            case GET_WIRE:{
                // Grab WIRE
                // If next string is ->     GO TO GET_ARROW_WIRE
                // If next string is other  THROW ERROR
                connection_parsed.push_back(my_parsed_file[index_parse]);
                index_parse++;
                if(my_parsed_file[index_parse] != "->"){std::cout << "[FATAL_ERROR][PARSER][dot][Reading_connection] Connection isn't done correctly, no arrows\n"; exit(1);}
                fsm_state = GET_ARROW_WIRE;
                break;
            }
            case GET_ARROW_WIRE:{
                // Grab ARROW
                // If next string is WIRE   GRAB IT
                //                          GO TO CHECK_END
                index_parse++;
                connection_parsed.push_back(my_parsed_file[index_parse]);
                fsm_state = CHECK_END;
                break;
            }
            case CHECK_END:{
                // If next string is ->     GO TO GET_ARROW_WIRE
                // If next string is ;      BREAK WHILE(!";")
                // If next is other         THROW ERROR
                index_parse++;
                if(my_parsed_file[index_parse] == "->"){
                    fsm_state = GET_ARROW_WIRE;
                    break;
                } else if(my_parsed_file[index_parse] == ";"){
                    break;
                } else{
                    std::cout << "[FATAL_ERROR][PARSER][dot][Reading_connection] Connection isn't done correctly, no -> or ; \n"; exit(1);
                }
            }
        }
    }
}

/* get_parsed_dot
    -> base fsm who call reading instance & reading connection
*/
void get_parsed_dot(std::vector<std::string> &my_parsed_file, std::vector<instance_NODE> &Nodes){
    enum parsed_dot {START, CONDITION_IC, NEXT_LINE, END};
    parsed_dot fsm_state = START;

    std::string name_parsed;                // Defined by circuit name
    std::vector<std::string> temp_line;     // Used to grab instance or connection
    parsed_matrice connection_parsed;       // Defined by <I1 GATE O1>

    instance_NODE temp_node;
    // Start at beginning of lexed dot file
    int index_parse = 0;

    while(index_parse < my_parsed_file.size()){
        switch(fsm_state){
            case START :{
                // if !digraph                          THROW ERROR
                // Grab name_parsed
                // if next string is {                  GO TO CONDITION_IC
                // Else                                 THROW ERROR
                if(my_parsed_file[index_parse] != "digraph"){std::cout << "[FATAL_ERROR][PARSER][dot][parsed_dot] no digraph at beginning\n"; exit(1);}
                index_parse++;
                name_parsed = my_parsed_file[index_parse];
                index_parse++;
                if(my_parsed_file[index_parse] != "{"){std::cout << "[FATAL_ERROR][PARSER][dot][parsed_dot] no '{' in file\n"; exit(1);}
                fsm_state = CONDITION_IC;
                index_parse++;
                break;
            }
            case CONDITION_IC :{
                // if next string is }                  THROW WARNING PROBLEM WITH FILE
                // if two next strings are ? + [        CALL fsm_reading_instance
                // if two next strings are ? + ->       CALL fsm_reading_connection
                // if next string is ;                  GO TO NEXT_LINE
                // Else                                 THROW ERROR
                if(my_parsed_file[index_parse] == "}"){std::cout << "[FATAL_ERROR][PARSER][dot][parsed_dot] file is empty because {} closed immediatly\n"; exit(1);}

                if(index_parse+1 < my_parsed_file.size()){
                    if(my_parsed_file[index_parse+1] == "["){
                        if(contains_name(Nodes, my_parsed_file[index_parse]) != -1){std::cout << "[FATAL_ERROR][PARSER][dot][parsed_dot] name already exist in file : "<< my_parsed_file[index_parse]<<"\n"; exit(1);}
                        fsm_reading_instance(my_parsed_file, index_parse, temp_node);
                        Nodes.push_back(temp_node);
                        temp_node.clear();}
                    if(my_parsed_file[index_parse+1] == "->"){fsm_reading_connection(my_parsed_file, index_parse, temp_line); connection_parsed.push_back(temp_line); temp_line.clear();}
                }
                if(my_parsed_file[index_parse] != ";"){std::cout << "[FATAL_ERROR][PARSER][dot][parsed_dot] problem with skiping_lines in file after ';' check if oppening -> or [ is done correctly (missing spaces)"; exit(1);}
                fsm_state = NEXT_LINE;
                index_parse++;
                break;
            }
            case NEXT_LINE :{
                // if next string is ? + [ OR ? + ->    GO TO CONDITION_IC
                // if next string is }                  GO TO END
                // Else                                 THROW ERROR
                // std::cout << "current : " << my_parsed_file[index_parse] << " next : " << my_parsed_file[index_parse+1] << std::endl;
                if(index_parse+1 < my_parsed_file.size() && (my_parsed_file[index_parse+1] == "[" || my_parsed_file[index_parse+1] == "->")){
                    fsm_state = CONDITION_IC;
                } else if(my_parsed_file[index_parse] == "}"){
                    fsm_state = END;
                } else{std::cout << "[FATAL_ERROR][PARSER][dot][parsed_dot] problem with skiping_lines in file after ';' check if oppening -> or [ is done correctly (missing spaces)"; exit(1);}
                break;
            }
            case END :{
                // if next string is }                  FILE DONE PARSED
                // if index_parse == file.size()        BREAK WHILE()
                // Else                                 THROW ERROR
                index_parse++;
                if(index_parse < my_parsed_file.size()){std::cout << "[FATAL_ERROR][PARSER][dot][parsed_dot] problem with size of file check where {} closed \n"; exit(1);}
                break;
            }
        }
    }
    std::cout << "[INFO][dot][parsed_dot] ending of parsing dot" << std::endl;
    std::cout << "[INFO][dot][parsed_dot] starting of verification dot" << std::endl;
    // empty file
    if(Nodes.size() == 0){
        std::cout << "[FATAL_ERROR][PARSER][dot][parsed_dot] file is empty \n";
        exit(1);
    }

    // can you assign connection to input ? no
    check_entrance_connection(connection_parsed, Nodes);
    // assign connections
    assign_connection(connection_parsed, Nodes);
    // ONLY one output can be reconnected
    check_reconnected_out(Nodes);
    // Gates should have more than one input
    check_missing_input_GATE(Nodes);
    // Outputs should have one input
    check_missing_input_OUTPUT(Nodes);
    // Inputs should have only outputs
    check_missing_output_INPUT(Nodes);
    // Not should have one input
    check_one_input_NOT(Nodes);
    // Check multiple time same connection in GATE
    check_multiple_connection(Nodes);
    // Check at least one input
    check_atleast_one_input(Nodes);
    // Check at least one output
    check_atleast_one_output(Nodes);
    // Check outputs only have gate outputs
    check_outputs_out_gates(Nodes);
    // Check mux selectors are enough for inputs
    check_mux_input_sel(Nodes);
    // Check FF have only one input
    check_FF_input(Nodes);

    std::cout << "[INFO][dot][parsed_dot] ending of verification dot" << std::endl;
}