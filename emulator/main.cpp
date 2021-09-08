#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>



#include "InstructionFactory.hpp"
#include "Storage.hpp"


int decodeSequence(std::string input_sequence_in, uint8_t **sequence_out, int *sequence_length_out)
{
    int count = 1;
    for (size_t i = 0; i < input_sequence_in.size(); i++)
    {
        if (input_sequence_in[i] == ' ')
        {
            count++;
        }
    }
    (*sequence_length_out) = count;

    *sequence_out = (uint8_t *)malloc(sizeof(uint8_t) * count);

    std::stringstream ss(input_sequence_in);
    std::string word;
    {
        int index = 0;
        while (ss >> word)
        {
            // cout << word << endl;
            (*sequence_out)[index] = (uint8_t)(std::stoi(word, 0, 16));
            index++;
        }
    }
    return 0;
}

void printSequence(uint8_t *sequence, int length)
{
    for (int i = 0; i < length; i++)
    {
        printf("%x ", sequence[i]);
    }
}

int main(int argc, char const *argv[])
{
    std::string input_sequence = "8d 4c 24 04 83 e4 f0 50 c1 f8 02 34 2e 89 e5 45 83 ec 04 80 7c 24 04 81 5d";
    int sequence_length;
    uint8_t *sequence;
    decodeSequence(input_sequence, &sequence, &sequence_length);

    printSequence(sequence, sequence_length);
    std::cout << std::endl;

    int current_index = 0;

    AbstractInstruction *ins;
    Storage* storage =  new Storage();
    for (size_t i = 0; i < 25; i++)
    {
        ins = InstructionFactory::createInstruction(sequence, &current_index, storage);
        ins->execute();
        delete ins;
    }
    // InstructionFactory::createInstruction(sequence, &current_index);
    // InstructionFactory::createInstruction(sequence, &current_index);

    return 0;
}
