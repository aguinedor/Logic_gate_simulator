#include "output_file_generator.h"

void generateOutputFile(std::vector<std::string> &inputData, std::string name = "OutputSimulation.json"){

    std::ofstream outputSimulation(name);
    std::vector<std::string> outputFile;
    std::string tempString;
    int sizeValue = 0;

    outputFile.push_back("{signal: [");
    for(int i=0 ; i<inputData.size() ; i+=2){
        for(int j=0 ; j<inputData[i+1].size() ; j++){
            if(j!=0){
                if(inputData[i+1][j] == inputData[i+1][j-1]){
                    tempString += ".";
                }
                else{
                    if(inputData[i+1][j] == '-'){
                        if(inputData[i+1][j+1] == '1'){
                            tempString += "x";
                            j++;
                        }
                        else{
                            std::cout << "[FATAL_ERROR][SIM][OUTPUT][Format] type : Invalid data format. \"-\" should only precede a \"1\". (label : "+inputData[i]+")" << std::endl;
                            exit(EXIT_FAILURE);
                        }
                    }
                    else if(inputData[i+1][j] == '0' || inputData[i+1][j] == '1'){
                        tempString += inputData[i+1][j];
                    }
                    else{
                        std::cout << "[FATAL_ERROR][SIM][OUTPUT][Format] type : Invalid data format. Data should be an int between -1 and 1. (label : "+inputData[i]+")" << std::endl;
                        exit(EXIT_FAILURE);
                    }
                }
            }
            else{
                if(inputData[i+1][j] == '-'){
                    if(inputData[i+1][j+1] == '1'){
                        tempString = "x";
                        j++;
                    }
                    else{
                        std::cout << "[FATAL_ERROR][SIM][OUTPUT][Format] type : Invalid data format. \"-\" should only precede a \"1\". (label : "+inputData[i]+")" << std::endl;
                        exit(EXIT_FAILURE);
                    }
                }
                else if(inputData[i+1][j] == '0' || inputData[i+1][j] == '1'){
                    tempString = inputData[i+1][j];
                }
                else{
                    std::cout << "[FATAL_ERROR][SIM][OUTPUT][Format] type : Invalid data format. Data should be an int between -1 and 1. (label : "+inputData[i]+")" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
        }

        if(i==0){
            sizeValue = tempString.size();
        }
        else{
            if(tempString.size() != sizeValue){
                std::cout << "tempString size : " << tempString.size() << "sizeValue : " << sizeValue << std::endl;
                std::cout << "[FATAL_ERROR][SIM][OUTPUT][Format] type : Invalid data format. Each data frame should be the same size." << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        if(i!=inputData.size()-2){
            outputFile.push_back("{name: \'" + inputData[i] + "\', wave: '" + tempString + "\'},");
        }
        else{
            outputFile.push_back("{name: \'" + inputData[i] + "\', wave: '" + tempString + "\'}");
        }

    }
    outputFile.push_back("]}");



    std::ostream_iterator<std::string> output_iterator(outputSimulation, "\n"); // Create an iterator to put "\n" char (in string format) after each data
    copy(begin(outputFile), end(outputFile), output_iterator); // Copy the content of the output in the file
    std::cout << "[INFO][SIM] Output file generated successfully. \n";
    outputSimulation.close();
    return;

}