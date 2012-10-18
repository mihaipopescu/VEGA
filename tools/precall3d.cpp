#include <iostream>
#include <string>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/detail/endian.hpp>
#include <fstream>

using namespace std;
namespace po = boost::program_options;


inline void swap_short(short* data) {
    *data = (*data & 0xf) << 8 | (*data >> 8);
}

// todo: template <class T> and determine type from file size
struct volume {

    bool load(const char * file)
    {
        ifstream ffile;
        
        ffile.open(file, ios_base::binary | ios_base::in);

        if( ffile.fail() ) {
            cout << "ERROR: Could not read input volume file '" << file << "' ! " << endl;
            return false;
        }
        
        ffile.read((char*)&width, sizeof(short)); 
        ffile.read((char*)&height, sizeof(short));
        ffile.read((char*)&depth, sizeof(short));
        
    #ifdef BOOST_LITTLE_ENDIAN
        swap_short(&width);
        swap_short(&height);
        swap_short(&depth);
    #endif
        
        size_t s = width * height * depth;
        data.resize(s);
        
        ffile.read((char*)&data[0], s);
        ffile.close();
        
        cout << "Volume " << file << " size = " << s << " bytes" << endl;
        return true;
    }

    short width;
    short height;
    short depth;
    std::vector<unsigned char> data;
};

void compare(volume v1, volume v2) {
    int TP = 0, TN = 0;
    int FP = 0, FN = 0;
    
    if (v1.data.size() == v2.data.size()) {
    
        for(size_t i=0; i<v1.data.size(); ++i) {
            if (v1.data[i] == 0 && v2.data[i] == 0) {
                TN++;
            } else if (v1.data[i] == v2.data[i]) {
                TP ++;
            }
            else if (v1.data[i] == 0) {
                FN ++;
            } else {
                FP ++;
            }
        }
        
        cout << "TP=" << TP << " TN=" << TN << " FP=" << FP << " FN=" << FN << endl;
     
        float p = -1.f;
        
        if (TP + FP > 0)
            p = TP * 1.f / (TP + FP);
        
        float r = -1.f;
        
        if (TP + FN > 0)
            r = TP * 1.f / (TP + FN);
        
        cout << "Precision = " << p << endl;
        cout << "Recall = " << r << endl;
    }
}

int main(int argc, char ** argv) {

    cout << "Precission-Recall 3D" << endl;
    
    
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce this help message")
        ("file,f", po::value<string>(), "file to compare")
        ("reference,r", po::value<string>(), "reference file")
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
        cout << "File to compare = " << file << endl;
    } else {
        cout << "File to compare was not set !" << endl;
    }
    
    string fileRef;
    
    if (vm.count("reference")) {
        fileRef = vm["reference"].as<string>();
        cout << "Reference file = " << fileRef << endl;
    } else {
        cout << "Reference file not set !" << endl;
    }
    
    volume vol;
    if (!vol.load(file.c_str())) {
        return 1;
    }
    
    volume volRef;
    if (!volRef.load(fileRef.c_str())) {
        return 1;
    }

    compare(vol, volRef);
    
    return 0;
}