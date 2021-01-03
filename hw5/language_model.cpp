#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include "language_model.h"
#include <cstring>


using std::ifstream;
using std::string;
using std::stringstream;
using std::vector;
using std::map;
using std::pair;
using std::make_pair;
//using std::err;

int buildModel(int argc, char** argv) {


    //try opening list file
    ifstream listFile( argv[1] );
    if (!listFile.is_open()) {
        std::cerr << "Invalid file list: " << argv[1] << std::endl;
        return 1;
    }

    vector<string> myFiles;
    string nextFile;
    while (listFile >> nextFile) {
        myFiles.push_back(nextFile);
    }

    map<string, int> trigram = makeTrigrams(myFiles);

    if (commandLineErr(argc, argv)) {return 1;}
    //now we have built our model. Let's perform the operation!
    if (strcmp(argv[2], "a") == 0) {
        a_operation(trigram);
    } else if (strcmp(argv[2], "d") == 0) {
        d_operation(trigram);
    } else if (strcmp(argv[2], "c") == 0) {
        c_operation(trigram);
    } else { f_operation(trigram, argv); }

    return 0;
}


map<string, int> makeTrigrams(vector<string> fileVector) {

    map<string, int> trigram;
    string a;
    string b;
    string c;
    string myTrigram;


    for (string currFile : fileVector) {
        // open file and create a vector of maps<string, int> . the string is the trigram and int is count.
        // only make one vector that includes all trigrams from all text files. then you can order them depending on the user op.
        ifstream current( currFile );
        if (!current.is_open()) {
            std::cerr << "Invalid file: " << currFile << std::endl;//print error
            continue;
        }

        stringstream ss;
        string temp;
        b = "<START_1>";
        c = "<START_2>";

        while (current >> temp) {
            a = b;
            b = c;
            c = temp;

            myTrigram = a + " " + b + " " + c;
            std::map<string, int>::iterator it = trigram.find(myTrigram);
            if (it != trigram.end()) {
                //already exists, so just increment count
                it->second += 1;
            } else {
                //first occurence of this trigram, so add it
                trigram[myTrigram] = 1;
            }

        }
        std::map<string, int>::iterator it = trigram.find(b + " " + c + " <END_1>");
        if(it != trigram.end()) {
            it->second += 1;
        } else {
            trigram[b + " " + c + " <END_1>"] = 1;
        }
        it = trigram.find(b + " " + c + " <END_2>");
        if(it != trigram.end()) {
            it->second += 1;
        } else {
            trigram[c + " <END_1> <END_2>"] = 1;
        }
        
    }

    //std::cout << trigram.size() << std::endl;
    return trigram;

}


int commandLineErr(int argc, char** argv) {
    //no operation specified.
    if (argc < 3){
        return 1;
    }
    //operation must be a, d, c or f
    string op = argv[2];
    if ((op != "a") && (op != "d") && (op != "c") && (op != "f")) {
        std::cerr << "Invalid command: valid options are a, d, c, and f" << std::endl;
        return 1;
    }

    //f operation requires two more command args
    if ((op == "f") && (argc < 5)) {
        std::cerr << "Invalid argument list: f requires two additional command-line arguments" << std::endl;
        return 1;
    }

    return 0;
}


void a_operation(map<string, int> myMap) {
    //iterate forwards
    for (map<string, int>::iterator it = myMap.begin(); it != myMap.end(); ++it) {
        std::cout << it->second << " - [" << it->first << "]" << std::endl;
    }

}

void d_operation(map<string, int> myMap) {
    //iterate in reverse
    for (map<string, int>::reverse_iterator it = myMap.rbegin(); it != myMap.rend(); ++it) {
        std::cout << it->second << " - [" << it->first << "]" << std::endl;
    }

}

void c_operation(map<string, int> myMap) {

    //copy map to vector of pairs, then order
    vector<pair<string, int>> vec;
    map<string, int>::iterator it;

    for (it = myMap.begin(); it != myMap.end(); ++it) {
        vec.push_back(make_pair(it->first, it->second));
    }
    //sort, then print
    sort(vec.begin(), vec.end(), wayToSort);
    for (int i = 0; i < (int) vec.size(); i++) {
        std::cout << vec[i].second << " - [" << vec[i].first << "]" << std::endl;
    }

}

bool wayToSort(pair<string, int> pair1, pair<string, int> pair2) {
    //decreasing value, break ties by ascending alphabetical order
    if (pair1.second != pair2.second) {
        return pair1.second > pair2.second;
    } else {return pair1.first < pair2.first;}
}

void f_operation(map<string, int> myMap, char** argv) {

    string str1 = argv[3];
    string str2 = argv[4];

    map<string, int>::iterator it;
    string mySubString( str1 + " " + str2 + " ");
    int maxCount = 0;
    string myTarget;

    for (it = myMap.begin(); it != myMap.end(); ++it) {
        if (it->first.find(mySubString) != std::string::npos) {
            //basically a comparator
            if (it->second > maxCount) {
                maxCount = it->second;
                myTarget = it->first;
            } else if(it->second == maxCount) {
                if (it->first < myTarget) {
                    myTarget = it->first;
                }
            }
        }
    }
    if (maxCount == 0) {
        std::cerr << "No trigrams of the form [" << mySubString << "?]" << std::endl;
    } else { std::cout << maxCount << " - [" << myTarget << "]" << std::endl; }
}