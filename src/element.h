#ifndef ELEMENT_H
#define ELEMENT_H

#include <vector>
#include <string>
#include <iostream>

#include "parser_dot.h"

class element{

    private:
        std::string m_name;
        std::string m_type;

        // Accessible variable in subclass
        protected: bool already_been_here = 0;
        protected: std::vector<element*> m_inputs;
        protected: std::vector<element*> m_outputs; //probably useless
        protected: int m_output; // -1 is x || 0 is 0 || 1 is 1
        protected: int past_value;
        protected: int next_value = 0;


    public:
        static int current_delta_cycle;
        static bool done_first_column;

        element(std::string name, std::string type): m_name(name), m_type(type), m_output(-1){};
        ~element(){std::cout<<"Element destroyed"<<std::endl;};

        void connect_new_element_input(element* node);
        void connect_new_element_output(element* node);
        void remove_element_input(std::string name);
        void remove_element_output(std::string name);

        void clear_output(){m_output = 0;};
        void clear_been_here(){already_been_here = 0;};
        void clear_past_values(){past_value = 0;};
        void clear_next_values(){next_value = 0;};

        static void set_delta_cycle(int i){current_delta_cycle = i;};
        static void set_done_first_column(bool i){done_first_column = i;};

        // Overrident methods for subclass
        virtual void add_selectors(element* Node) = 0;
        virtual int get_output() = 0;
        virtual void calcul_gate() = 0;
        virtual void import_input(int value) = 0;

        virtual void update_next_value() = 0;

        const std::string& get_name() const {return m_name;};
        const std::string& get_type() const {return m_type;};
        const int& get_past_value() const {return past_value;};
};
/* Derivated class
    -> there is one for each gates used
    -> FF still need to be implemented
*/
class input : public element{
    private:
    public:
        input(std::string name): element(name,"INPUT"){};
        ~input();

        int get_output(){this->past_value = this->m_output;std::cout<<"               [INPUT][get_output] name : " << this->get_name()<< " value outputed : "<< this->m_output <<std::endl;return this->m_output;};
        void calcul_gate(){};
        void import_input(int value){this->past_value = value;std::cout<<"               [INPUT][import_input] name : " << this->get_name()<< " value imported : "<< value <<std::endl;this->m_output = value;};
        void add_selectors(element* Node){};
        void update_next_value(){};
};
class output : public element{
    private:
    public:
        output(std::string name): element(name,"OUTPUT"){};
        ~output();

        int get_output();
        void calcul_gate(){};
        void import_input(int value){};
        void add_selectors(element* Node){};
        void update_next_value(){};
};
class and_gate : public element{
    private:
    public:
        and_gate(std::string name): element(name,"AND"){};
        ~and_gate();

        int get_output();
        void calcul_gate();
        void import_input(int value){};
        void add_selectors(element* Node){};
        void update_next_value(){};
};
class not_gate : public element{
    private:
    public:
        not_gate(std::string name): element(name,"NOT"){};
        ~not_gate();

        int get_output();
        void calcul_gate();
        void import_input(int value){};
        void add_selectors(element* Node){};
        void update_next_value(){};
};
class nand_gate : public element{
    private:
    public:
        nand_gate(std::string name): element(name,"NAND"){};
        ~nand_gate();

        int get_output();
        void calcul_gate();
        void import_input(int value){};
        void add_selectors(element* Node){};
        void update_next_value(){};
};
class or_gate : public element{
    private:
    public:
        or_gate(std::string name): element(name,"OR"){};
        ~or_gate();

        int get_output();
        void calcul_gate();
        void import_input(int value){};
        void add_selectors(element* Node){};
        void update_next_value(){};
};
class nor_gate : public element{
    private:
    public:
        nor_gate(std::string name): element(name,"NOR"){};
        ~nor_gate();

        int get_output();
        void calcul_gate();
        void import_input(int value){};
        void add_selectors(element* Node){};
        void update_next_value(){};
};
class xor_gate : public element{
    private:
    public:
        xor_gate(std::string name): element(name,"XOR"){};
        ~xor_gate();

        int get_output();
        void calcul_gate();
        void import_input(int value){};
        void add_selectors(element* Node){};
        void update_next_value(){};
};
class mux_gate : public element{
    private:
        std::vector<element*> m_selectors;
    public:
        mux_gate(std::string name): element(name,"MUX"){};
        ~mux_gate();

        int get_output();
        void calcul_gate();
        void import_input(int value){};
        void add_selectors(element* Node){this->m_selectors.push_back(Node);};
        void update_next_value(){};
};
class ff_gate : public element{
    private:
    public:
        ff_gate(std::string name): element(name,"FF"){};
        ~ff_gate();

        int get_output();
        void calcul_gate(){};
        void import_input(int value){};
        void add_selectors(element* Node){};
        void update_next_value();
};
#endif