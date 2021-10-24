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
    // std::string input_sequence = "8d 4c 24 04 83 e4 f0 50 c1 f8 02 34 2e 89 e5 45 83 ec 04 80 7c 24 04 81 5d";
    // std::string input_sequence = "83 c0 05 83 c1 10 01 c8 83 c1 20 83 c2 25";
    // std::string input_sequence = "83 80 e8 03 00 00 05 83 80 e8 03 00 00 fe 83 80 e9 03 00 00 ff 01 c8 83 c1 20 83 c2 25";
    // std::string input_sequence = "83 1c 8a 31";
    // std::string input_sequence = "83 b4 ca e8 03 00 00 31";
    // std::string input_sequence = "81 b4 ca e8 03 00 00 d1 2f 01 00";
    // std::string input_sequence = "05 ab cd 8d 84 ca e8 03 00 00";
    // std::string input_sequence = "50 5a";
    // std::string input_sequence = "ff c2";
    // std::string input_sequence = "ff ca";
    // std::string input_sequence = "ff f2";
    std::string input_sequence = "ba af 11 af 11 89 d1";
    int sequence_length;
    uint8_t *sequence;
    decodeSequence(input_sequence, &sequence, &sequence_length);

    printSequence(sequence, sequence_length);
    std::cout << std::endl;

    int current_index = 0;

    AbstractInstruction *ins;
    Storage *storage = new Storage();
    while (current_index < sequence_length)
    {
        ins = InstructionFactory::createInstruction(sequence, &current_index, storage);
        ins->execute();
        storage->printAll();
        delete ins;
    }
    // InstructionFactory::createInstruction(sequence, &current_index);
    // InstructionFactory::createInstruction(sequence, &current_index);

    return 0;
}
