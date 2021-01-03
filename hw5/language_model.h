#ifndef language_model
#define language_model

#include <vector>
#include <string>
#include <map>

int buildModel(int argc, char** argv);

std::map<std::string, int> makeTrigrams(std::vector<std::string> fileVector);

int commandLineErr(int argc, char** argv);

void a_operation(std::map<std::string, int> myMap);

void d_operation(std::map<std::string, int> myMap);

void c_operation(std::map<std::string, int> myMap);

bool wayToSort(std::pair<std::string, int> pair1, std::pair<std::string, int> pair2);

void f_operation(std::map<std::string, int> myMap, char** argv);




#endif