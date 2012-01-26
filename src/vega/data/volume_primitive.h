#ifndef __VEGA_DATA_VOLUME_PRIMITIVE_H__
#define __VEGA_DATA_VOLUME_PRIMITIVE_H__

#include "../common/types.h"
#include "volume.h"


namespace vega
{
	namespace data
	{
		class volume_primitive : public volume
		{
		public:
			volume_primitive(uint16 Width, uint16 Height, uint16 Depth) : volume(Width, Height, Depth) { }

			void create_cylinder(float fCx, float fCy, float fCz, float fHeight, float fTopRadius, float fBottomRadius, voxel density = MAXDENSITY);
			void create_cylinder_shell(float fCx, float fCy, float fCz, float fHeight, float fTopSmallRadius, float fTopLargeRadius, float fBottomSmallRadius, float fBottomLargeRadius, voxel density = MAXDENSITY );
			void create_honeycomb(uint8 iSpan, uint8 opacity, int xx, int yy, int zz);
			
			void create_regular_prism(float fCx, float fCy, float fCz, float fHeight, float fTopRadius, float fBottomRadius, int numEdges, voxel density = MAXDENSITY);
			void create_sphere_shell(float fCx, float fCy, float fCz, float fSmallRadius, float fLargeRadius, voxel density = MAXDENSITY);
			void create_sphere(float fCx, float fCy, float fCz, float fRadius, voxel density = MAXDENSITY);
			void create_thorus(float fCx, float fCy, float fCz, float fSmallRadius, float fLargeRadius, voxel density = MAXDENSITY);
		};
	}
}

#endif // __VEGA_DATA_VOLUME_PRIMITIVE_H__
