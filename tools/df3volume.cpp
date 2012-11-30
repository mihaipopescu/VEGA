#include <fstream>
#include <iostream>

#include "df3volume.h"

#ifdef _WIN32
# define LITTLE_ENDIAN
#endif

#ifdef LITTLE_ENDIAN
    #define SWAP_2(x) ( (((x) & 0xff) << 8) | ((unsigned short)(x) >> 8) )
    #define SWAP_4(x) ( ((x) << 24) | \
             (((x) << 8) & 0x00ff0000) | \
             (((x) >> 8) & 0x0000ff00) | \
             ((x) >> 24) )
    #define FIX_SHORT(x) (*(unsigned short *)&(x) = SWAP_2(*(unsigned short *)&(x)))
    #define FIX_INT(x)   (*(unsigned int *)&(x)   = SWAP_4(*(unsigned int *)&(x)))
#else
    #define FIX_SHORT(x) (x)
    #define FIX_INT(x)   (x)
#endif


using namespace std;


bool df3volume::load(const char * file) {
    ifstream ffile;
    
    ffile.open(file, ios_base::binary | ios_base::in);

    if( ffile.fail() ) {
        cout << "ERROR: Could not read input volume file '" << file << "' ! " << endl;
        return false;
    }
    
    ffile.read((char*)&width, sizeof(short)); 
    ffile.read((char*)&height, sizeof(short));
    ffile.read((char*)&depth, sizeof(short));
    
    width = FIX_SHORT(width);
    height = FIX_SHORT(height);
    depth = FIX_SHORT(depth);
    
    size_t size = width * height * depth;
    data.resize(size);
    
    ffile.read((char*)&data[0], size);
    if( ffile.fail() ) {
        cerr << "Failed to read from volume file !" << endl;
        ffile.close();
        return false;
    }
    
    ffile.close();
    
    cout << "Volume " << file << " size = " << size << " bytes" << endl;
    return true;
}


bool df3volume::load_raw(uint16 _width, uint16 _height, uint16 _depth, const char * file) {
    ifstream ffile;
    
    ffile.open(file, ios_base::binary | ios_base::in);
    if( ffile.fail() ) {
        cout << "Could not read input volume file '" << file << "' ! " << endl;
        return false;
    }
    
    size_t size = _width * _height * _depth;
    data.resize(size);
    
    ffile.read((char*)&data[0], size);
    if( ffile.fail() ) {
        cerr << "Failed to read from volume file " << file << endl;
        ffile.close();
        return false;
    }
    
    width = _width;
    height = _height;
    depth = _depth;
    
    cout << "Volume " << file << " size = " << size << " bytes" << endl;
    return true;
}


bool df3volume::save(const char* file) {

    ofstream df3(file, ios::out | ios::binary);
    if( df3.fail() ) {
        cerr << "Could not open file " << file << " for writing !" << endl;
        return false;
    }
    
    uint16 _width = FIX_SHORT(width);
    uint16 _height = FIX_SHORT(height);
    uint16 _depth = FIX_SHORT(depth);
    
    df3.write((const char*)&_width, sizeof(uint16));
    df3.write((const char*)&_height, sizeof(uint16));
    df3.write((const char*)&_depth, sizeof(uint16));

    df3.write((const char*)&data[0], data.size());

    if( df3.fail() ) {
        cerr << "Failed to save df3 volume to output file " << file << endl;
        df3.close();
        return false;
    }
    
    df3.close();
    return true;
}
