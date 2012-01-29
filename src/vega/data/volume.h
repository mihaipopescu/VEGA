#ifndef __VEGA_DATA_VOLUME_H__
#define __VEGA_DATA_VOLUME_H__

#include "common.h"
#include "../common/types.h"
#include "../math/vector3d.h"
#include "../math/transfer_function.h"

#include <vector>
#include <memory>

#define MINDENSITY 0
#define MAXDENSITY 255

#define _I(x, y, z) ((z)*myWidth*myHeight + (y)*myWidth + (x))


namespace vega
{
    namespace data
    {
        class volume : public i_model
        {
        public:
            volume                      ();
            volume                      (uint16 Width, uint16 Height, uint16 Depth);
            virtual ~volume             ();

			virtual bool create			(const volume &v) { copy_from(v); return true; }

            size_t get_size             () const { return myWidth * myHeight * myDepth; }
            uint16 get_width            () const { return myWidth; }
            uint16 get_height           () const { return myHeight; }
            uint16 get_depth            () const { return myDepth; }

            float  get_voxel            (uint16 x, uint16 y, uint16 z) const { return float(myVoxelArray[_I(x,y,z)])/255.f; }
			void   set_voxel			(uint16 x, uint16 y, uint16 z, voxel v) { myVoxelArray[_I(x, y, z)] = v; }
            voxel& operator()  			(uint16 x, uint16 y, uint16 z) { return myVoxelArray[_I(x, y, z)]; }
            voxel  operator()           (uint16 x, uint16 y, uint16 z) const { return myVoxelArray[_I(x, y, z)]; }
            void*  get_raw_data()       { return myVoxelArray.data(); }
            void*  get_lut_data()       { return &myTransferFunction->myTransferArray[0]; }

        public:
            void copy_from              (const volume& v);
            bool load                   (const std::string& _FileName, bool _UseGradients);
            bool save                   ();
            void compute_gradients      (int nSampleRadius = 1);
            void paint_voxels           (std::vector<r8g8b8a8>& vPaintedVoxels) const;

        protected:
            bool load_raw               ();
            bool save_raw               () const;

            bool load_gradients         ();
            bool save_gradients         () const;
            
            math::vector3d filter_gradient  (uint16 x, uint16 y, uint16 z, uint16 nFilterRadius) const;

        public:
            // operators only usable on volumes of the same size
            volume operator+(const volume &v);
            volume operator*(const volume &v);
            volume operator-(const volume &v);
            volume operator~();

        protected:
            std::vector<math::transfer_control_point>   myColorKnots;
            std::vector<math::transfer_control_point>   myAlphaKnots;
            std::string                                 myRawFilename;
            std::string                                 myGradientsFilename;

            std::vector<voxel>                          myVoxelArray;
            std::vector<math::vector3d>                 myGradients;
            std::shared_ptr<math::transfer_function>    myTransferFunction;

            bool                                        myVolumeUsesGradients;
            bool                                        myGradientDataIsDirty;
            bool                                        myRawDataIsDirty;

            uint16                                      myWidth;
            uint16                                      myHeight;
            uint16                                      myDepth;
        };
    }
}


#endif