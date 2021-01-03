#include "language_model.h"

int main (int argc, char** argv) {

    //minimum number of arguments is 3
    if (argc <  3) {
        //error
    }

    return buildModel(argc, argv);
}