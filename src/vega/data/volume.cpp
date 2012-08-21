#include <fstream>
#include <iostream>
#include <xutility>
#include <assert.h>
#include <algorithm>
#include <sstream>

#include "volume.h"
#include "../math/transfer_function.h"
#include "../math/transformations.h"
#include "../common/logger.h"


#define clamp(val, vmin, vmax) min((vmax), max((vmin), (val)))


using namespace vega::math;
using namespace std;

vega::data::volume::volume(const std::string& _FileName, bool _UseGradients)
    : myWidth(0)
    , myHeight(0)
    , myDepth(0)
    , myVolumeUsesGradients(_UseGradients)
    , myRawDataIsDirty(false)
    , myGradientDataIsDirty(false)
    , myHeaderFilename(_FileName)
{
}

vega::data::volume::volume( uint16 iWidth, uint16 iHeight, uint16 iDepth )
    : myWidth(iWidth)
    , myHeight(iHeight)
    , myDepth(iDepth)
    , myVolumeUsesGradients(false)
    , myRawDataIsDirty(false)
    , myGradientDataIsDirty(false)
{
	myVoxelArray.resize(get_size());
	myTransferFunction = std::make_shared<transfer_function>();
}

vega::data::volume::~volume(void)
{
}

bool vega::data::volume::create()
{
    ifstream input(myHeaderFilename, ios::in);

    std::stringstream msg;

    if( input.fail() )
    {
        msg.str("");
        msg << "Failed to open volume file " << myHeaderFilename.c_str() << endl;
        VEGA_LOG_ERROR(msg.str().c_str());
        return false;
    }

    msg << "Loading volume file ["<< myHeaderFilename.c_str() << "]...";
    VEGA_LOG_INFO(msg.str().c_str());

    int iWidth = 0, iHeight = 0, iDepth = 0;

    input >> iWidth >> iHeight >> iDepth;
    
    int nColorCount = 0;
    input >> nColorCount;

	myTransferFunction.reset();

    if( nColorCount < 256 )
    {
        myColorKnots.clear();
        myColorKnots.reserve(nColorCount);
        for(int i=0; i<nColorCount; i++)
        {
            float r = 0.f, g = 0.f, b = 0.f;
            int iso = 0;
            input >> r >> g >> b >> iso;
            myColorKnots.push_back(transfer_control_point(r, g, b, iso));
        }

        int nAlphaCount = 0;
        input >> nAlphaCount;

        myAlphaKnots.clear();
        myAlphaKnots.reserve(nAlphaCount);
        for(int i=0; i<nAlphaCount; i++)
        {
            float a = 0.f;
            int iso = 0;
            input >> a >> iso;
            myAlphaKnots.push_back(transfer_control_point(a, iso));
        }

        myTransferFunction = std::make_shared<transfer_function>(myColorKnots, myAlphaKnots);
    }
    else if (nColorCount == 256)
    {
        myColorKnots.clear();
        myColorKnots.reserve(nColorCount);
        for(int i=0; i<nColorCount; i++)
        {
            int r, g, b;
            input >> r >> g >> b;
            myColorKnots.push_back(transfer_control_point((float)r / 255.f, (float)g / 255.f, (float)b / 255.f, (float)i / 255.f, i));
        }

        myTransferFunction = std::make_shared<transfer_function>(myColorKnots);
    }

    assert(myTransferFunction != NULL);

    if( input.fail() )
    {
        msg.str("");
        msg << "Error reading volume file " << myHeaderFilename.c_str() << endl;
        VEGA_LOG_ERROR(msg.str().c_str());
        input.close();
        return false;
    }

    input.close();

    // prepare volume for loading voxel data
    myWidth = iWidth;
    myHeight = iHeight;
    myDepth = iDepth;

    // Create raw and gradient filename from hdr filename by changing extension
    // NOTE: The files must be in the same folder as the header !
    if(myHeaderFilename.find_last_of('.') != string::npos)
    {
        myGradientsFilename = myHeaderFilename.substr(0, myHeaderFilename.find_last_of('.'));
        myGradientsFilename += ".grd";
        myRawFilename = myHeaderFilename.substr(0, myHeaderFilename.find_last_of('.'));
        myRawFilename += ".raw";
    }

    // load raw volume data
    load_raw();

    msg.str("");
    msg << "Width = " << myWidth << ", Height = " << myHeight << ", Depth = " << myDepth << std::endl;
    VEGA_LOG_INFO(msg.str().c_str());

    paint_voxels();

    // load voxel gradients
    if( myVolumeUsesGradients )
    {
        if( !load_gradients() )
        {
            compute_gradients(1);
        }
    }
    
    return true;
}

bool vega::data::volume::save()
{
    // save raw voxel data
    if( myRawDataIsDirty && !save_raw() )
        return false;

    // save gradient data
    if( myVolumeUsesGradients && myGradientDataIsDirty && !save_gradients() )
        return false;

    myRawDataIsDirty = false;
    myGradientDataIsDirty = false;

    return true;
}

bool vega::data::volume::save_as(const string& _FileName)
{
	myRawFilename = _FileName;
	myRawDataIsDirty = true;
	return save();
}

bool vega::data::volume::load_raw()
{
    ifstream input;
    input.open(myRawFilename, ios::in | ios::binary);

    if( input.fail() )
        return false;

    myVoxelArray.resize(get_size());

    input.read((char*)myVoxelArray.data(), get_size() * sizeof(voxel));

    if( input.fail() )
        return false;

    return true;
}

bool vega::data::volume::save_raw() const
{
    ofstream outfile;
    outfile.open(myRawFilename, ofstream::out | ios::binary);

    if( outfile.fail() )
        return false;
    
    outfile.write((const char*)myVoxelArray.data(), get_size() * sizeof(voxel));

    if( outfile.fail() )
    {
        outfile.close();
        return false;
    }
    
    outfile.close();
    return true;
}

void vega::data::volume::paint_voxels()
{
    // Paint voxels
    myPaintedVoxels.clear();
    myPaintedVoxels.reserve(myWidth * myHeight * myDepth);

    assert(myTransferFunction);

    std::for_each(myVoxelArray.cbegin(), myVoxelArray.cend(), [this](voxel v)
    {
        myPaintedVoxels.push_back(myTransferFunction->myTransferArray[v]);
    });
}

// Computes the gradients using a central difference scheme
// and filters using a NxNxN filter
void vega::data::volume::compute_gradients(int nSampleRadius /*= 1*/)
{
    int n = nSampleRadius;
    vector3d v1, v2;

    myGradientDataIsDirty = true;

    myGradients.clear();
    myGradients.reserve(get_size());

    cout << "Computing gradients... ";

    // Generates gradients central differences scheme
    for (uint16 z = 0; z < myDepth; z++)
    {
        for (uint16 y = 0; y < myHeight; y++)
        {
            for (uint16 x = 0; x < myWidth; x++)
            {
                v1.x = x >= n           ? get_voxel(x - n, y, z) : 0.f;
                v2.x = x < myWidth-n    ? get_voxel(x + n, y, z) : 0.f;
                v1.y = y >= n           ? get_voxel(x, y - n, z) : 0.f;
                v2.y = y < myHeight-n   ? get_voxel(x, y + n, z) : 0.f;
                v1.z = z >= n           ? get_voxel(x, y, z - n) : 0.f;
                v2.z = z < myDepth-n    ? get_voxel(x, y, z + n) : 0.f;

                v2 -= v1;
                myGradients.push_back(v2 * 0.5f);
            }
        }
    }

    cout << "Filtering gradients... ";

    int index = 0;
    // Applies an NxNxN filter to the gradients.
    for (uint16 z = 0; z < myDepth; z++)
    {
        for (uint16 y = 0; y < myHeight; y++)
        {
            for (uint16 x = 0; x < myWidth; x++)
            {
                myGradients[index++] = filter_gradient(x, y, z, n);
            }
        }
    }

    cout << "Done !" << endl;
}

vector3d vega::data::volume::filter_gradient(uint16 x, uint16 y, uint16 z, uint16 nFilterRadius) const
{
    vector3d average;
    int num = 0, n = nFilterRadius;

    for (uint16 k = z - n; k <= z + n; k++)
    {
        for (uint16 j = y - n; j <= y + n; j++)
        {
            for (uint16 i = x - n; i <= x + n; i++)
            {
                if ((x >= 0 && x < myWidth) &&
                    (y >= 0 && y < myHeight) &&
                    (z >= 0 && z < myDepth))
                {
                    average += myGradients[_I(x,y,z)];
                    num++;
                }
            }
        }
    }

    average.x /= (float)num;
    average.y /= (float)num;
    average.z /= (float)num;

    if (average.x != 0.0f && average.y != 0.0f && average.z != 0.0f)
        average.normalize();

    return average;
}

bool vega::data::volume::save_gradients() const
{
    ofstream outfile;
    outfile.open(myGradientsFilename, ofstream::out | ios::binary);

    if( outfile.fail() )
        return false;

    outfile.write((const char*)myGradients.data(), get_size() * sizeof(vector3d));

    if( outfile.fail() )
    {
        outfile.close();
        return false;
    }

    outfile.close();
    return true;
}

bool vega::data::volume::load_gradients()
{
    ifstream input;
    input.open(myGradientsFilename, ios::in | ios::binary);

    if( input.fail() )
        return false;

    myGradients.clear();
    myGradients.reserve(get_size());

    input.read((char*)myGradients.data(), get_size() * sizeof(voxel));

    if( input.fail() )
        return false;

    return true;
}

vega::data::volume vega::data::volume::operator+(const volume &v)
{
    volume result = *this;			
    unsigned int size = myWidth * myHeight * myDepth;
    for(register unsigned int i = 0; i < size; ++i)
        result.myVoxelArray[i] = clamp(myVoxelArray[i] + v.myVoxelArray[i], MINDENSITY, MAXDENSITY);

    myRawDataIsDirty = true;

    return result;
}

vega::data::volume vega::data::volume::operator*(const volume &v)
{
    volume result = *this;			
    unsigned int size = myWidth * myHeight * myDepth;
    for(register unsigned int i = 0; i < size; ++i)
        result.myVoxelArray[i] = (voxel)clamp((myVoxelArray[i] * v.myVoxelArray[i])/255.f, 1.f* MINDENSITY, 1.f * MAXDENSITY);

    myRawDataIsDirty = true;

    return result;
}

vega::data::volume vega::data::volume::operator-(const volume &v)
{
    volume result = *this;			
    unsigned int size = myWidth * myHeight * myDepth;
    for(register unsigned int i = 0; i < size; ++i)
        result.myVoxelArray[i] = clamp(myVoxelArray[i] - v.myVoxelArray[i], MINDENSITY, MAXDENSITY);

    myRawDataIsDirty = true;

    return result;
}

vega::data::volume vega::data::volume::operator~()
{
    volume result = *this;	

    unsigned int size = myWidth * myHeight * myDepth;
    for(register unsigned int i = 0; i < size; ++i)
        result.myVoxelArray[i] = clamp(MAXDENSITY - myVoxelArray[i], MINDENSITY, MAXDENSITY);

    myRawDataIsDirty = true;

    return result;
}

float vega::data::volume::get_voxel_color_distance( uint16 x1, uint16 y1, uint16 z1, uint16 x2, uint16 y2, uint16 z2 ) const
{
    r8g8b8a8 v1 = myPaintedVoxels[_I(x1, y1, z1)];
    r8g8b8a8 v2 = myPaintedVoxels[_I(x2, y2, z2)];

    // TODO: What happens with alpha ?
    math::vector3d p1(v1.R, v1.G, v1.B);
    math::vector3d p2(v2.R, v2.G, v2.B);

    return math::HSV_distance_from_RGB(p1 * (1.f/255), p2 * (1.f/255));
}
