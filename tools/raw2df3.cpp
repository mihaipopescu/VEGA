#include <stdlib.h>
#include <iostream>
#include <fstream>


using namespace std;


#define SWAP_2(x) ( (((x) & 0xff) << 8) | ((unsigned short)(x) >> 8) )
#define SWAP_4(x) ( ((x) << 24) | \
         (((x) << 8) & 0x00ff0000) | \
         (((x) >> 8) & 0x0000ff00) | \
         ((x) >> 24) )
#define FIX_SHORT(x) (*(unsigned short *)&(x) = SWAP_2(*(unsigned short *)&(x)))
#define FIX_INT(x)   (*(unsigned int *)&(x)   = SWAP_4(*(unsigned int *)&(x)))


#ifdef _WIN32
# define LITTLE_ENDIAN
#endif

int main(int argc, char **argv) {
    
    if (argc != 6) {
        cerr << "usage: " << argv[0] << " width height depth input(raw) output(df3)" << endl;
        return 1;
    }

    ifstream raw(argv[4], ios_base::in | ios_base::binary);
    if( raw.fail() ) {
        cerr << "ERROR: could not open input raw volume file " << argv[4] << " !" << endl;
        return 1;
    }

    short width = (short)atoi(argv[1]);
    short height = (short)atoi(argv[2]);
    short depth = (short)atoi(argv[3]);

    size_t size = width * height * depth;

    char * data = new char[size];
    raw.read((char*)data, size);

    if( raw.fail() ) {
        cerr << "Failed to read from raw volume file !" << endl;
        raw.close();
        delete [] data;
        return 1;
    }
    
    raw.close();
   
    ofstream df3(argv[5], ios::out | ios::binary);
    if( df3.fail() ) {
        cerr << "ERROR: could not open file " << argv[5] << " for writing !" << endl;
        delete [] data;
        return 1;
    }

#ifdef LITTLE_ENDIAN
    width = FIX_SHORT(width);
    height = FIX_SHORT(height);
    depth = FIX_SHORT(depth);
#endif

    df3.write((const char*)&width, sizeof(width));
    df3.write((const char*)&height, sizeof(width));
    df3.write((const char*)&depth, sizeof(width));

    df3.write((const char*)data, size);
    delete [] data;
    
    if( df3.fail() ) {
        cerr << "Failed to write to output file " << endl;
        df3.close();
        return 1;
    }
    
    df3.close();
    return 0;
}