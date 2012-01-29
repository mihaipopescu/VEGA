#include <math.h>
#include "volume_primitive.h"
#include "../math/common.h"


void vega::data::volume_primitive::create_cylinder( float fCx, float fCy, float fCz, float fHeight, float fTopRadius, float fBottomRadius, voxel density /*= MAXDENSITY*/ )
{
	for(float h = 0.0f; h <= fHeight; h += 1.0f)
	{
		float fCurrentCy = fCy - fHeight/2 + h;
		if( fCx < myWidth && fCurrentCy < myHeight && fCz < myDepth && fCx >= 0 && fCurrentCy >= 0 && fCz >= 0)
		{
			int x = (int)fCx;
			int y = (int)fCurrentCy;
			int z = (int)fCz;
			set_voxel(x, y, z, density);	// center density at height h
		}

		float fRadius = fBottomRadius + (fHeight - h) * (fTopRadius - fBottomRadius) / fHeight;

		for(float r = fRadius; r >= 0.1f; r -= 1.0f)
		{
			float fStep = (float)asinf( 1.0f / r) *0.4f;
			for(float phi = 0; phi < math::PI * 0.5f; phi += fStep)	
			{
				for(unsigned char i = 0; i < 4; i++)
				{
					int x, y, z;
					x = (int)( fCx + (i & 1 ? -1: 1) * r * cos(phi) );
					y = (int)( fCurrentCy );
					z = (int)( fCz + (i & 2 ? -1: 1) * r * sin(phi) );
					if( x > (int)myWidth || y > (int)myHeight || z > (int)myDepth || x < 0 || y < 0 || z < 0)
						continue;
					set_voxel(x, y, z, density);
				}
			}
		}
	}
}

void vega::data::volume_primitive::create_cylinder_shell( float fCx, float fCy, float fCz, float fHeight, float fTopSmallRadius, float fTopLargeRadius, float fBottomSmallRadius, float fBottomLargeRadius, voxel density /*= MAXDENSITY */ )
{
	for(float h = 0.0f; h <= fHeight; h += 1.0f)
	{
		float fCurrentCy = fCy - fHeight/2 + h;

		float fSmallRadius = fBottomSmallRadius + (fHeight - h) * (fTopSmallRadius - fBottomSmallRadius) / fHeight;
		float fLargeRadius = fBottomLargeRadius + (fHeight - h) * (fTopLargeRadius - fBottomLargeRadius) / fHeight;

		for(float r = fSmallRadius; r <= fLargeRadius; r += 1.0f)
		{
			float fStep = (float)asin( 1.0f / r) * 0.4f;			
			for(float phi = 0; phi < math::PI * 0.5f; phi += fStep)	
			{
				for(unsigned char i = 0; i < 4; i++)
				{
					int x, y, z;
					x = (int)( fCx + (i & 1 ? -1: 1) * r * cos(phi) );
					y = (int)( fCurrentCy );
					z = (int)( fCz + (i & 2 ? -1: 1) * r * sin(phi) );
					if( x > (int)myWidth || y > (int)myHeight || z > (int)myDepth || x < 0 || y < 0 || z < 0)
						continue;
					set_voxel(x, y, z, density);
				}
			}
		}
	}	
}


void vega::data::volume_primitive::create_honeycomb( uint8 iSpan, uint8 opacity, int xx, int yy, int zz )
{
	#define IN_RANGE_X(x,rx) ( (rx = (x)) >= 0 && (x)<myWidth )
	#define IN_RANGE_Y(y,ry) ( (ry = (y)) >= 0 && (y)<myHeight )

	//The span of the cell must be an even number to produce good results!
	if( iSpan % 2 == 0 )
		return;

	uint16 px, py;

	// for each z slice
	for(uint16 z=0; z<myDepth; z++)
	{
		px = 0;
		py = (iSpan + 1) >> 1;

		for(uint16 x=iSpan, cx=0; x<myWidth; x += (3*iSpan - 1)>>1, px++, cx++)
		{
			for(uint16 y=py, cy=0; y<myHeight; y += iSpan + 1, cy++)
			{
				int a = iSpan;
				bool bWireframe = cx!=xx || cy!=yy || (z/iSpan)!=zz;

				do
				{
					int s = (a-1)>>1;
					for(int ud=-s;ud<=s;ud++)
					{
						uint16 rx, ry;
						if( bWireframe && (z % iSpan) && (abs(ud)!=s) )
							continue;

						unsigned char o = opacity;

						if(IN_RANGE_X(x + ud, rx))
						{
							if(IN_RANGE_Y(y + s + 1, ry))
								set_voxel(rx, ry, z, o);

							if(IN_RANGE_Y(y - s - 1, ry))
								set_voxel(rx, ry, z, o);
						}
						if(IN_RANGE_Y(y + ud, ry))
						{
							if(IN_RANGE_X(x + (abs(ud) - a), rx))
								set_voxel(rx, ry, z, o);

							if(IN_RANGE_X(x - (abs(ud) - a), rx))
								set_voxel(rx, ry, z, o);
						}
					}
					a--;
				} while( !bWireframe && (a>=0)   );
			}

			if(px % 2)
				py += (iSpan + 1)>>1;
			else
				py -= (iSpan + 1)>>1;
		}
	}
}

void vega::data::volume_primitive::create_regular_prism( float fCx, float fCy, float fCz, float fHeight, float fTopRadius, float fBottomRadius, int numEdges, voxel density /*= MAXDENSITY*/ )
{
	struct Vec3{ float x,y,z; } *v = new Vec3[numEdges];
	float alpha = 2 * math::PI / numEdges;
	float *sines = new float[numEdges];
	float *cosines = new float[numEdges];

	for(int i = 0; i < numEdges; ++i)
	{
		sines[i] = sin( i * alpha );
		cosines[i] = cos( i * alpha);
	}

	for(float h = 0.0f; h <= myHeight; h += 1.0f)
	{
		int x = (int)fCx;
		int y = (int)fCy;
		int z = (int)fCz;
		set_voxel(x, y, z, density);	// center density at height h

		float fRadius = fBottomRadius + (myHeight - h) * (fTopRadius - fBottomRadius) / fHeight;
		float fCurrentCy = fCy - fHeight/2 + h;
		for(float r = fRadius; r >= 0.1f; r -= 1.0f)
		{

			for(int i = 0; i < numEdges; ++i)
			{
				v[i].x = fCx + r * sines[i];
				v[i].y = fCurrentCy;
				v[i].z = fCz + r * cosines[i];
			}

			for(int i = 0; i < numEdges; ++i)
			{				
				int iplus1 = (i + 1) % numEdges;
				float fZDist = v[iplus1].z - v[i].z;
				float fXDist = v[iplus1].x - v[i].x;
				float fZStep = fZDist > 0.0f ? 0.1f : -0.1f;
				float fXStep = fXDist > 0.0f ? 0.1f : -0.1f;

				if( fabs(fZDist) < 0.1f)
				{
					if( fabs(fXDist) < 0.1f)
					{
						// the line segment is basically a voxel in this case
						if( fCx > myWidth || fCurrentCy > myHeight || fCz > myDepth || fCx < 0 || fCurrentCy < 0 || fCz < 0)
							continue;

						x = (int)fCx;
						y = (int)fCurrentCy;
						z = (int)fCz;

						set_voxel(x, y, z, density);
					}
					else
					{		
						// interpolate over x
						for(float xt = v[i].x; fabs(xt - v[iplus1].x) >= 0.1f; xt += fXStep)
						{
							x = (int)( xt );						
							y = (int)( v[i].y );
							z = (int)( v[i].z + (xt - v[i].x) * fZDist / fXDist );						
							if( x > myWidth || y > myHeight || z > myDepth || x < 0 || y < 0 || z < 0)
								continue;

							set_voxel(x, y, z, density);
						}					
					}
				}
				else
				{
					// interpolate over z
					for(float zt = v[i].z; fabs(zt - v[iplus1].z) >= 0.1f; zt += fZStep)
					{
						x = (int)( v[i].x + (zt - v[i].z) * fXDist / fZDist );
						y = (int)( v[i].y );
						z = (int)( zt );
						if( x > myWidth || y > myHeight || z > myDepth || x < 0 || y < 0 || z < 0)
							continue;

						set_voxel(x, y, z, density);
					}
				}
			}
		}
	}

	delete[] v;
	delete[] sines;
	delete[] cosines;
}

void vega::data::volume_primitive::create_sphere_shell( float fCx, float fCy, float fCz, float fSmallRadius, float fLargeRadius, voxel density /*= MAXDENSITY*/ )
{
	for(float r = fSmallRadius; r < fLargeRadius; r += 1.0f)
	{
		float fStep = asinf( 1.0f / r) / 2.0f;
		for(float theta = 0.0f; theta < math::PI* 0.5f; theta += fStep)
		{
			float fRSinTheta = r * sin(theta);
			float fRCosTheta = r * cos(theta);
			for(float phi = 0; phi < math::PI * 0.5f; phi += fStep)	
			{
				float fRSinThetaCosPhi = fRSinTheta * cos(phi);
				float fRSinThetaSinPhi = fRSinTheta * sin(phi);
				for(int i = 0; i < 8; ++i)
				{
					int x, y, z;
					x = (int)( fCx + ( i & 1 ? fRSinThetaCosPhi : -fRSinThetaCosPhi ) );
					y = (int)( fCy + ( i & 2 ? fRSinThetaSinPhi : -fRSinThetaSinPhi ) );
					z = (int)( fCz + ( i & 4 ? fRCosTheta : -fRCosTheta ) );
					if( x < myWidth && y < myHeight && z < myDepth && x >= 0 && y >= 0 && z >= 0)
						set_voxel(x, y, z, density);
				}
			}
		}
	}
}

void vega::data::volume_primitive::create_sphere( float fCx, float fCy, float fCz, float fRadius, voxel density /*= MAXDENSITY*/ )
{
	for(float r = 1.0f; r < fRadius; r += 1.0f)
	{
		float fStep = (float)asinf( 1.0f / r) * 0.4f;
		for(float theta = 0.0f; theta < math::PI * 0.5f; theta += fStep)
		{
			float fRSinTheta = r * sin(theta);
			float fRCosTheta = r * cos(theta);
			for(float phi = 0; phi <= math::PI * 0.5f; phi += fStep)
			{
				float fRSinThetaCosPhi = fRSinTheta * cos(phi);
				float fRSinThetaSinPhi = fRSinTheta * sin(phi);
				for(int i = 0; i < 8; ++i)
				{
					int x, y, z;
					x = (int)( fCx + ( i & 1 ? fRSinThetaCosPhi : -fRSinThetaCosPhi ) );
					y = (int)( fCy + ( i & 2 ? fRSinThetaSinPhi : -fRSinThetaSinPhi ) );
					z = (int)( fCz + ( i & 4 ? fRCosTheta : -fRCosTheta ) );
					if( x < myWidth && y < myHeight && z < myDepth && x >= 0 && y >= 0 && z >= 0)
						set_voxel(x, y, z, density);
				}
			}
		}
	}
}

void vega::data::volume_primitive::create_thorus( float fCx, float fCy, float fCz, float fSmallRadius, float fLargeRadius, voxel density /*= MAXDENSITY*/ )
{
	float fThorusRadius = (fLargeRadius - fSmallRadius) * 0.5f;
	float fThorusStep = (float)asin( 1.0 / fThorusRadius ) * 0.5f;
	for(float theta = -math::PI* 0.5f; theta <= 0 ; theta += fThorusStep)
	{
		float fSmallR = fSmallRadius + fThorusRadius * ( 1.0f - cos(theta) );
		float fLargeR = fLargeRadius - fThorusRadius * ( 1.0f - cos(theta) );
		float fRelativeHeight = fThorusRadius * sin(theta);
		for(float r = fSmallR; r <= fLargeR; r += 1.0f)
		{
			float fStep = (float)asin( 1.0 / r) * 0.4f;	
			for(float phi = 0; phi < math::PI * 0.5f; phi += fStep)	
			{
				float fRCosPhi = r * cos( phi );
				float fRSinPhi = r * sin( phi );
				for(int i = 0; i < 8; i++)
				{
					int x, y, z;
					x = (int)( fCx + (i & 1 ? -fRCosPhi : fRCosPhi)  );
					y = (int)( fCy + (i & 2 ? -fRelativeHeight : fRelativeHeight) );
					z = (int)( fCz + (i & 4 ? -fRSinPhi : fRSinPhi) );
					if( x > myWidth || y > myHeight || z > myDepth || x < 0 || y < 0 || z < 0)
						continue;
					set_voxel(x, y, z, density);
				}
			}
		}
	}
}
