#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <stdlib.h>
#include <boost/algorithm/string.hpp>

using namespace std;
namespace fs = filesystem;

void printError(string message);
void printUsage();
void compile(string sourceFilePath, string outFilePath);
void dissamble(string compiledFilePath, string disassemblyFilePath);
void extractSection(string sectionName, string disassemblyFilePath, string sectionFilePath);
void extractInstructions(string sectionFilePath, string instructionFilePath);

int main(int argc, char **argv) {
  int required_args = 2;
  if(argc != (required_args + 1)){
    string message = "Incorrect number of Command Line Arguements provided.\n";
    message = message + "\trequired: " + to_string(required_args) + "\n";
    message = message + "\tprovided: " + to_string(argc - 1); 
    printError(message);
    printUsage();
    exit (EXIT_FAILURE);
  }

  fs::path cFilePath = argv[1];
  string section = argv[2];

  boost::trim_left_if(section, boost::is_any_of("<"));
  boost::trim_right_if(section, boost::is_any_of(">"));

  string cFileNamePrefix = boost::trim_right_copy_if(
    cFilePath.filename().string(), 
    boost::is_any_of(cFilePath.extension().string())
  );

  cout << cFilePath << endl;
  cout << cFileNamePrefix << endl;
  cout << section << endl;

  fs::path baseDirPath = cFilePath.parent_path();
  fs::path outFilePath = baseDirPath / (cFileNamePrefix + ".out");
  fs::path disassmeblyFilePath = baseDirPath / (cFileNamePrefix + ".disassembly");
  fs::path sectionFilePath = baseDirPath / ("section-" + section);
  fs::path instructionFilePath = baseDirPath / ("input-" + section);

  compile(cFilePath, outFilePath);
  dissamble(outFilePath, disassmeblyFilePath);
  extractSection(section, disassmeblyFilePath, sectionFilePath);
  extractInstructions(sectionFilePath, instructionFilePath);

  return 0;
}

void printError(string message){
  cerr << "\033[1;31m" << message << "\033[0m\n";
}

void printUsage(){
  cerr
    << "\nUsage: ./generate-test-input.out <c-file-path> <section-name>\n\n"
    << "  c-file-path         absolute or relative path of C program file used to generate the test input.\n"
    << "                      following additional files will be created inside parent directory of the C file.\n"
    << "                          1. compiled output of the C program file\n"
    << "                          2. disassembly of that compiled output\n"
    << "                          3. extracted section from the disassembly\n"
    << "                          4. generated test input file for the section\n"
    << "                      \n"
    << "  section-name        section from the disassmbly to be used for generating test input.\n\n" 
    ;
}

int execSystemCommand(string commandStr){
  if (!system(NULL)) {
    printError ("Command processor is not available");
    exit (EXIT_FAILURE);
  }
  const char* command = commandStr.c_str();
  cout << "Executing " << command << "\n";
  
  return system(command);
}

void compile(string sourceFilePath, string outFilePath){
  cout << "compiling...\n"; 

  string commandStr = "gcc -m32 -O3 -o " + outFilePath + " " + sourceFilePath;
  int status = execSystemCommand(commandStr);
  if (status != 0){
    printError ("Failed to compile");
    exit (EXIT_FAILURE);
  }
}

void dissamble(string compiledFilePath, string disassemblyFilePath){
  cout << "disassembling...\n";

  string commandStr = "objdump -D " + compiledFilePath + " > " + disassemblyFilePath;
  int status = execSystemCommand(commandStr);
  if (status != 0){
    printError ("Failed to create disassembly");
    exit (EXIT_FAILURE);
  }
}

void extractSection(string sectionName, string disassemblyFilePath, string sectionFilePath){
  cout << "extracting section...\n";
  cout << "section name : " << sectionName << "\n";

  string line;
  sectionName = "<" + sectionName + ">";  

  ifstream disassemblyFile (disassemblyFilePath);
  ofstream sectionFile (sectionFilePath);  

  if (disassemblyFile.is_open())
  {
    bool isInSegment = false;  
    while (getline(disassemblyFile, line))
    {
      if (!isInSegment && (line.find(sectionName) != string::npos)){
          isInSegment = true;
      }
      if (isInSegment) {
          if (line == ""){
              break;
          }
          if (sectionFile.is_open()){
               sectionFile << line << "\n";
          } 
          else printError("Unable to open section output file");         
      }
    }

    disassemblyFile.close();
    sectionFile.close();
  }

  else printError("Unable to open disassembly file"); 
}

void extractInstructions(string sectionFilePath, string instructionFilePath){
  cout << "extracting instructions...\n";

  string line;
  
  ifstream sectionFile (sectionFilePath);
  ofstream instructionFile (instructionFilePath);  

  if (sectionFile.is_open())
  {
    getline(sectionFile, line); //skip first line

    vector<string> lineSplit;
    while (getline(sectionFile, line))
    {
      boost::split(lineSplit, line, boost::is_any_of("\t"), boost::token_compress_on);
      boost::trim(lineSplit[1]);

      if (instructionFile.is_open()){
            instructionFile << lineSplit[1] << " ";
      } 
      else printError("Unable to open instruction output file");         
    }

    sectionFile.close();
    instructionFile.close();
  }

  else printError("Unable to open section file");   
}
