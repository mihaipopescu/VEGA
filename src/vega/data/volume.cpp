#include <fstream>
#include <iostream>
#include <xutility>
#include <assert.h>
#include <algorithm>

#include "volume.h"
#include "../math/transfer_function.h"


#define clamp(val, vmin, vmax) std::min((vmax), std::max((vmin), (val)))

using namespace vega::math;
using namespace std;

vega::data::volume::volume()
    : myWidth(0)
    , myHeight(0)
    , myDepth(0)
    , myVolumeUsesGradients(false)
    , myRawDataIsDirty(false)
    , myGradientDataIsDirty(false)
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
    save();
}

bool vega::data::volume::load(const string& _FileName, bool _UseGradients)
{
    ifstream input(_FileName, ios::in);

    cout << "Loading volume file ["<< _FileName.c_str() << "]...";

    if( input.fail() )
    {
        cerr << "Failed to open volume file " << _FileName.c_str() << endl;
        return false;
    }

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
        cerr << "Error reading volume file " << _FileName.c_str() << endl;
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
    if(_FileName.find_last_of('.') != string::npos)
    {
        myGradientsFilename = _FileName.substr(0, _FileName.find_last_of('.'));
        myGradientsFilename += ".grd";
        myRawFilename = _FileName.substr(0, _FileName.find_last_of('.'));
        myRawFilename += ".raw";
    }

    // load raw volume data
    load_raw();

    std::cout << "Width = " << myWidth << ", Height = " << myHeight << ", Depth = " << myDepth << std::endl;

    // load voxel gradients
    myVolumeUsesGradients = _UseGradients;
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
        result.myVoxelArray[i] = (voxel)clamp((myVoxelArray[i] * v.myVoxelArray[i])*0.00390625f, 1.f* MINDENSITY, 1.f * MAXDENSITY);// 1/256 = 0.00390625

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

void vega::data::volume::paint_voxels( std::vector<r8g8b8a8>& vPaintedVoxels ) const
{
    vPaintedVoxels.clear();
    vPaintedVoxels.reserve(get_size());

    assert(get_size() == myVoxelArray.size());
	assert(myTransferFunction);

	std::for_each(myVoxelArray.cbegin(), myVoxelArray.cend(), [&vPaintedVoxels, this](voxel v)
	{
		vPaintedVoxels.push_back(myTransferFunction->myTransferArray[v]);
	});
}

void vega::data::volume::copy_from( const volume& v )
{
    myWidth = v.myWidth;
    myHeight = v.myHeight;
    myDepth = v.myDepth;

    myRawDataIsDirty = v.myRawDataIsDirty;
    myGradientDataIsDirty = v.myGradientDataIsDirty;
    myVolumeUsesGradients = v.myVolumeUsesGradients;

    myVoxelArray = v.myVoxelArray;
    myGradients = v.myGradients;
    myTransferFunction = v.myTransferFunction;

    myColorKnots = v.myColorKnots;
    myAlphaKnots = v.myAlphaKnots;
    myRawFilename = v.myRawFilename;
    myGradientsFilename = v.myGradientsFilename;
}
