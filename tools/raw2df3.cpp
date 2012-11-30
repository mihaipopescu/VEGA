#include <iostream>
#include "df3volume.h"

using namespace std;


int main(int argc, char **argv) {
    
    if (argc != 6) {
        cerr << "usage: " << argv[0] << " width height depth input(raw) output(df3)" << endl;
        return 1;
    }

    short width = (short)atoi(argv[1]);
    short height = (short)atoi(argv[2]);
    short depth = (short)atoi(argv[3]);
    
    df3volume v;
    v.load_raw(width, height, depth, argv[4]);
    v.save(argv[5]);
    
    return 0;
}