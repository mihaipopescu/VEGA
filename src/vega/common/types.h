#ifndef __VEGA_TYPES_H__
#define __VEGA_TYPES_H__

namespace vega
{
    typedef unsigned char uint8;
    typedef unsigned int uint32;
    typedef unsigned short uint16;
    typedef __int64 int64;
    typedef unsigned __int64 uint64;
    typedef const char * lpcstr;
    typedef uint8 voxel;

    union r8g8b8a8
    {
        r8g8b8a8(uint8 r=0, uint8 g=0, uint8 b=0, uint8 a=0) : R(r), G(g), B(b), A(a) {}
        unsigned int RGBA;
        struct
        {
            uint8 R;
            uint8 G;
            uint8 B;
            uint8 A;
        };
    };

    struct box
    {
        uint32 Left;
        uint32 Top;
        uint32 Right;
        uint32 Bottom;
        uint32 Front;
        uint32 Back;
    };
};

#endif