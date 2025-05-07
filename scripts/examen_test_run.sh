#!/bin/bash

# ----------------------------------------------------- Global variables 
TEST_FAIL=true
TEST_ELEM=true
TEST_STRUCT=true
TEST_SPECIAL_CIRCUIT=true

output_file=output_simulation.json
src_dir=./src/
examen_test_dir=tests/examen_tests

circuit_check_dir=$examen_test_dir/circuits
grammar_check_dir=$examen_test_dir/grammar_check
structure_check_dir=$examen_test_dir/structure_check

gate_1_dir=$examen_test_dir/portes_elementaires/gate_1
gate_2_dir=$examen_test_dir/portes_elementaires/gate_2
gate_3_dir=$examen_test_dir/portes_elementaires/gate_3

dir_struct_ok1=$examen_test_dir/structure_ok/gate_1
dir_struct_ok2=$examen_test_dir/structure_ok/gate_2
dir_struct_ok3=$examen_test_dir/structure_ok/gate_3
dir_struct_ok4=$examen_test_dir/structure_ok/gate_4

in_json_1=$examen_test_dir/stimuli/one_input.json
in_json_2=$examen_test_dir/stimuli/two_inputs.json
in_json_3=$examen_test_dir/stimuli/three_inputs.json
in_json_4=$examen_test_dir/stimuli/four_inputs.json


# ----------------------------------------------------- useful functions
function clean_output_file() {
   if [ -f "$output_file" ]; then
      rm $output_file
   fi
}

function display_output_file() {
   if [ -f "$output_file" ]; then
      cat $output_file
   fi
}

function running_test_dir() {
   for file in $1/*
   do
      echo -e "\n[test] running " $file ""
      clean_output_file
      ./$src_dir/simulator $file $2 | grep ERROR --color=always -A 5
      display_output_file
   done
}

# ----------------------------------------------------- compiling program
make -C $src_dir
make -C $src_dir clean

clean_output_file

# ----------------------------------------------------- 1 - Failing tests
if $TEST_FAIL; then

   echo -e "\n[TEST] ---------------------- Running grammar check \n"
   running_test_dir "$grammar_check_dir" "scripts/dumb.json"

   echo -e "\n[TEST] ---------------------- Running structure check \n"
   running_test_dir "$structure_check_dir" "scripts/dumb.json"

fi

# ----------------------------------------------------- 2 - elementary gate tests
if $TEST_ELEM; then

   echo -e "\n[TEST] ---------------------- Running gate 1 \n"
   running_test_dir "$gate_1_dir" "$in_json_1"

   echo -e "\n[TEST] ---------------------- Running gate 2 \n"
   running_test_dir "$gate_2_dir" "$in_json_2"

   echo -e "\n[TEST] ---------------------- Running gate 3 \n"
   running_test_dir "$gate_3_dir" "$in_json_3"

fi

# ----------------------------------------------------- 3 - structure gate tests
if $TEST_STRUCT; then

   echo -e "\n[TEST] ---------------------- Running struct gate 1 \n"
   running_test_dir "$dir_struct_ok1" "$in_json_1"

   echo -e "\n[TEST] ---------------------- Running struct gate 2 \n"
   running_test_dir "$dir_struct_ok2" "$in_json_2"

   echo -e "\n[TEST] ---------------------- Running struct gate 3 \n"
   running_test_dir "$dir_struct_ok3" "$in_json_3"

   echo -e "\n[TEST] ---------------------- Running struct gate 4 \n"
   running_test_dir "$dir_struct_ok4" "$in_json_4"

fi

# -----------------------------------------------------4 - complex circuits tests
if $TEST_SPECIAL_CIRCUIT; then

   echo -e "\n[TEST] ---------------------- Running circuit half adder\n"
   clean_output_file
   ./$src_dir/simulator $circuit_check_dir/half_adder.dot $circuit_check_dir/half_adder.json | grep ERROR --color=always -A 5
   display_output_file

   echo -e "\n[TEST] ---------------------- Running circuit full adder\n"
   clean_output_file
   ./$src_dir/simulator $circuit_check_dir/full_adder.dot $circuit_check_dir/full_adder.json | grep ERROR --color=always -A 5
   display_output_file

   echo -e "\n[TEST] ---------------------- Running linear feed back register or LFSR\n"
   clean_output_file
   ./$src_dir/simulator $circuit_check_dir/lfsr.dot $circuit_check_dir/lfsr.json | grep ERROR --color=always -A 5
   display_output_file

fi