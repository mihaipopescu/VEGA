#include <iostream>
#include "df3volume.h"

using namespace std;


void threshold_seg(df3volume &v, uint16 th, uint16 eps) {

    for(size_t i=0;i<v.size();i++) {
    
        if( v.data[i] >= th - eps && v.data[i] <= th + eps ) {
            v.data[i] = (uint8)(-1);
        } else {
            v.data[i] = 0;
        }
    }
}


int main(int argc, char **argv) {
    
    if (argc != 5) {
        cerr << "usage: " << argv[0] << " input(df3) output(df3) th(u16) epsilon(u16)" << endl;
        return 1;
    }
    
    uint16 th = atoi(argv[3]);
    uint16 eps = atoi(argv[4]);
    
#ifdef _DEBUG
    std::cout << "Segmentation with hysteresis threshold: " << th << " eps=" << eps << endl;
#endif

    df3volume v;
    v.load(argv[1]);
    
    threshold_seg(v, th, eps);
    v.save(argv[2]);
    
    return 0;
}