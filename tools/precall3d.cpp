#include <iostream>
#include <string>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>

#include <math.h>

#include "df3volume.h"


using namespace std;
namespace po = boost::program_options;


float naive_benchmark(const df3volume& v1, const df3volume& v2) {
    int TP = 0, TN = 0;
    int FP = 0, FN = 0;
    
    if (!v1.equal_size(v2))
        return -1.f;

    for(size_t i=0; i<v1.data.size(); ++i) {
        if (v1.data[i] == 0 && v2.data[i] == 0) {
            TN++;
        } else if (v1.data[i] == v2.data[i]) {
            TP++;
        } else if (v1.data[i] == 0) {
            FN++;
        } else {
            FP++;
        }
    }

    if (TP == 0)
        return -1.f;

    float p = 0;

    if (TP + FP > 0) {
        p = TP * 1.f / (TP + FP);
    }

    float r = -1.f;

    if (TP + FN > 0) {
        r = TP * 1.f / (TP + FN);
    }
    
    cout << p << " " << r << endl;

    return p / r;
}


#define EPSILON 4
#define _I(x,y,z) (x + y*s1.width + z*s1.width*s1.height)
#define _IN(x,y,z) ((x)>=0 && (x)<width && (y)>=0 && (y)<height && (z)>=0 && (z)<depth)

float dist_match(const df3volume &s1, const df3volume &s2) {

    if( !s1.equal_size(s2) )
        return -1.f;

    uint16 width = s1.depth;
    uint16 height = s1.height;
    uint16 depth = s1.depth;
    uint32 matched = 0;
    uint32 count = 0;
    
    bool found = false;

    // for each voxel
    for(int z=0; z<depth; z++) {
        for(int y=0; y<height; y++) {
            for(int x=0; x<width; x++) {
            
                // if there is a boundary voxel in s1 at (x,y,z)
                if(s1.data[_I(x,y,z)] != 0) {
                    count++;
                    
                    found = false;
                    // within a cube neighborhood around (x,y,z)
                    for(int k=z-EPSILON;k<=z+EPSILON;k++) {
                        for(int j=y-EPSILON;j<=y+EPSILON;j++) {
                            for(int i=x-EPSILON;i<=x+EPSILON;i++) {
                            
                                // if the distance from (i,j,k) to (x,y,z) < EPSILON
                                // and (i,j,k) are valid coords
                                // check whether there is a boundary voxel at (i,j,k) in s2
                                float len=sqrtf(float(x-i)*(x-i)+float(y-j)*(y-j)+float(z-k)*(z-k));
                                if (len<=EPSILON && _IN(i,j,k) && s2.data[_I(i,j,k)] != 0) {
                                    
                                    // get a unit vector from (x,y,z)->(i,j,k)
                                    float vx = float(i-x);
                                    float vy = float(j-y);
                                    float vz = float(k-z);
                                    float mag = sqrtf(vx*vx + vy*vy + vz*vz);
                                    
                                    bool noBlock = false;
                                    
                                    // avoid checking the same voxel
                                    if (mag > 0.f) {
                                        vx /= mag;
                                        vy /= mag;
                                        vz /= mag;
                                        
                                        noBlock = true;
                                        
                                        // check there are no other boundary voxels between (x,y,z) and (i,j,k) in s1
                                        // go from (x,y,z) to (i,j,k)
                                        for(int d=1;d<len;d++) {
                                            int xx = (int)floorf(x + d*vx + 0.5f);
                                            int yy = (int)floorf(y + d*vy + 0.5f);
                                            int zz = (int)floorf(z + d*vz + 0.5f);
                                            if (_IN(xx,yy,zz) && xx!=i && yy!=j && zz!=k && s1.data[_I(xx,yy,zz)] != 0) {
                                                // there is another voxel in between
                                                noBlock = false;
                                                break;
                                            }
                                        }
                                    }
                                    
                                    bool sameDir = false;
                                    // if we passed the first test...
                                    if (noBlock) {
                                        // now we check that (x,y,z) is the direction of the closest boundary from (i,j,k) in s2
                                        
                                        sameDir = true;
                                    }
                                    
                                    if (noBlock && sameDir) {
                                    
                                        matched++;
                                        found = true;
                                        break;
                                    }
                                }
                            }
                            if (found) break;
                        }
                        if (found) break;
                    }
                    
                }
            }
        }
    }


    if (count == 0)
        return -1.f;

    return float(matched) / count;
}




int main(int argc, char ** argv) {

#ifdef _DEBUG
    cout << "Precision-Recall 3D" << endl;
#endif
    
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce this help message")
        ("file,f", po::value<string>(), "volume to compare (df3)")
        ("reference,r", po::value<string>(), "reference volume (df3)")
		("mode,m", po::value<string>(), "[naive,distmatch]")
    ;
    
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    
    if (vm.count("help") ) {
        cout << desc << endl;
        return 1;
    }
    
    string file;
    
    if (vm.count("file")) {
        file = vm["file"].as<string>();
#ifdef _DEBUG
        cout << "File to compare = " << file << endl;
#endif
    } else {
        cerr << "File to compare was not set !" << endl;
    }
    
    string fileRef;
    
    if (vm.count("reference")) {
        fileRef = vm["reference"].as<string>();
#ifdef _DEBUG
        cout << "Reference file = " << fileRef << endl;
#endif
    } else {
        cerr << "Reference file not set !" << endl;
    }
    
    string bench;

    if (vm.count("mode")) {
        bench = vm["mode"].as<string>();
#ifdef _DEBUG
        cout << "Benchmark mode =" << bench << endl;
#endif
    } else {
        cerr << "Benchmark mode not set !" << endl;
    }

    df3volume vol;
    if (!vol.load(file.c_str())) {
        return 1;
    }
    
    df3volume volRef;
    if (!volRef.load(fileRef.c_str())) {
        return 1;
    }

    if (bench.compare("naive") == 0) {
        naive_benchmark(vol, volRef);
    }

    if (bench.compare("distmatch") == 0) {
        cout << dist_match(vol, volRef) << " " << dist_match(volRef, vol) << endl;
    }
    
    return 0;
}