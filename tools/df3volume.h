#ifndef __VEGA_TOOLS_DF3VOLUME_H__
# define __VEGA_TOOLS_DF3VOLUME_H__

#include <vector>

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;


// todo: template <class T> and determine type from file size
struct df3volume {

    bool load(const char* file);
    bool load_raw(uint16 width, uint16 height, uint16 depth, const char* file);
    bool save(const char* file);
    
    bool equal_size(const df3volume& v) const {
        return width==v.width && height==v.height && depth==v.depth;
    }
    
    uint32 size() const { return width * height * depth; }

    uint16 width;
    uint16 height;
    uint16 depth;
    
    std::vector<uint8> data;
};

#endif // __VEGA_TOOLS_DF3VOLUME_H__