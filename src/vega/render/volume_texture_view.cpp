#include <iostream>
#include <windows.h>
#include <GL/glew.h>
#include <algorithm>
#include <xutility>
#include <assert.h>
#include <GL/freeglut_std.h>

#include "volume_texture_view.h"
#include "../math/matrix.h"
#include "../math/transformations.h"
#include "../math/plane.h"


using namespace vega::data;
using namespace vega::math;

extern int g_WindowWidth;
extern int g_WindowHeight;


vega::render::volume_texture_view::volume_texture_view()
    : mySliceCount(0)
	, myRenderFlag(true)
{
     for(int i=0; i<eTex_ENUMNO; ++i)
         myTextureHandle[i] = 0;
}

bool vega::render::volume_texture_view::create( )
{
    auto pVolume = std::dynamic_pointer_cast<i_volume>(myModel);

    if( pVolume == NULL )
        return false;

    // vertex coords array
    //    v2----- v6
    //   /|      /|
    //  v3------v7|
    //  | |     | |
    //  | |v1---|-|v5
    //  |/      |/
    //  v0------v4
    //
    myFrameVertices.resize(8);
    myFrameVertices[0].x = myFrameVertices[1].x = myFrameVertices[2].x = myFrameVertices[3].x = -0.5f;
    myFrameVertices[4].x = myFrameVertices[5].x = myFrameVertices[6].x = myFrameVertices[7].x = 0.5f;
    myFrameVertices[0].y = myFrameVertices[1].y = myFrameVertices[4].y = myFrameVertices[5].y = -0.5f;
    myFrameVertices[2].y = myFrameVertices[3].y = myFrameVertices[6].y = myFrameVertices[7].y = 0.5f;
    myFrameVertices[0].z = myFrameVertices[3].z = myFrameVertices[4].z = myFrameVertices[7].z = 0.5f;
    myFrameVertices[1].z = myFrameVertices[2].z = myFrameVertices[5].z = myFrameVertices[6].z = -0.5f;

    myFrameSegments.resize(12);
    myFrameSegments[0] = std::pair<int,int>(0, 1);
    myFrameSegments[1] = std::pair<int,int>(1, 2);
    myFrameSegments[2] = std::pair<int,int>(2, 3);
    myFrameSegments[3] = std::pair<int,int>(3, 0);
    myFrameSegments[4] = std::pair<int,int>(4, 5);
    myFrameSegments[5] = std::pair<int,int>(5, 6);
    myFrameSegments[6] = std::pair<int,int>(6, 7);
    myFrameSegments[7] = std::pair<int,int>(7, 4);
    myFrameSegments[8] = std::pair<int,int>(0, 4);
    myFrameSegments[9] = std::pair<int,int>(1, 5);
    myFrameSegments[10] = std::pair<int,int>(2, 6);
    myFrameSegments[11] = std::pair<int,int>(3, 7);

    update_proxy_geometry();
    
    glEnable(GL_TEXTURE_3D);

    glGenTextures(eTex_ENUMNO, myTextureHandle);
    glBindTexture(GL_TEXTURE_3D, myTextureHandle[eTex_Volume]);

    // texture parameters
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    // set pixel store with no alignment when reading from main memory
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, pVolume->get_width(), pVolume->get_height(), pVolume->get_depth(), 0, GL_RGBA, GL_UNSIGNED_BYTE, pVolume->get_color_data());

    glBindTexture(GL_TEXTURE_2D, myTextureHandle[eTex_TranferFunction]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, pVolume->get_lut_data());

    return true;
}

void vega::render::volume_texture_view::render() const
{
    if( mySlices.size() == 0 || !myRenderFlag )
        return;

    glEnable(GL_TEXTURE_3D);
    glBindTexture(GL_TEXTURE_3D, myTextureHandle[eTex_Volume]);

    glColor3f(1.f,1.f,1.f);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // render each slice (back-front)
    std::for_each(mySlices.rbegin(), mySlices.rend(), [](const volume_slice& slice)
    {
        glVertexPointer(3, GL_FLOAT, 0, slice.myVertices.data());
        glTexCoordPointer(3, GL_FLOAT, 0, slice.myTexture.data());
        glDrawArrays(GL_TRIANGLE_FAN, 0, slice.myVertices.size());
    });

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisable(GL_TEXTURE_3D);

    // 2D transfer function rendering
    // TODO: needs refactoring
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, g_WindowWidth, 0, g_WindowHeight);
    glScalef(1.f, -1.f, 1.f);
    glTranslatef(0.f, -1.f * g_WindowHeight, 0.f);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, myTextureHandle[eTex_TranferFunction]);
    glBegin(GL_QUADS);
    glVertex2f(10, 40); glTexCoord2f(0.f, 0.f);
    glVertex2f(10, 560); glTexCoord2f(0.f, 1.f);
    glVertex2f(50, 560); glTexCoord2f(1.f, 1.f);
    glVertex2f(50, 40); glTexCoord2f(1.f, 0.f);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void vega::render::volume_texture_view::update_proxy_geometry()
{
    auto pVolume = std::dynamic_pointer_cast<i_volume>(myModel);

    if( pVolume && pVolume->get_size() == 0 )
        return;

    matrix4x4 mView;
    glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*)&(mView.m));

    // transpose the matrix to get a row-major matrix
    make_transpose(mView);

    // transform frame coordinates into view coordinates using the ModelView matrix
    std::vector<vector3d> myTransformedFrameVertices(myFrameVertices);
    transform_coord_array(myTransformedFrameVertices, mView);

    // compute inverse view matrix
    matrix4x4 mViewInv = make_inverse(mView);

    // create affine transform matrix for 3D texture coordinates
    matrix4x4 mTex;
    make_translation(mTex, vector3d(0.5f, 0.5f, 0.5f));

    // Find the minimum and maximum z coordinates of the transformed vertices. 
    // Compute the number of sampling planes used between these two values using equidistant spacing from the view origin. 
    float minz = 1000.f, maxz = -1000.f;
    for(unsigned int i=0;i<myTransformedFrameVertices.size();i++)
    {
        minz = min(minz, myTransformedFrameVertices[i].z);
        maxz = max(maxz, myTransformedFrameVertices[i].z);
    }

    // The sampling distance is computed from the voxel size and current sampling rate. 
    if( mySliceCount == 0 )
        mySliceCount = max( max( pVolume->get_width(), pVolume->get_height()), pVolume->get_depth() );

    assert(mySliceCount > 0);
    if( mySliceCount == 0 )
        return;

    float dDist = (maxz - minz) / mySliceCount;

    // discard previous mesh
    mySlices.clear();

    // we are now working on view coordinates
    //  so the slicing planes should always be perpendicular to the view 
    //  thus the normal should always be the versor of Z axis
    vector3d norm(0.f, 0.f, 1.f);

    // the initial position is at the farthest vertex to the camera
    vector3d pos(0.f, 0.f, maxz);

    // create each slice
    for( int iSlice = 0; iSlice < mySliceCount; ++iSlice )
    {
        // create slice plane
        plane SlicePlane(pos, norm);

        // advance the slice plane point along Z Axis (back to front)
        pos -= vector3d(0.f, 0.f, dDist);
    
        // the current slice
        std::vector<vector3d> vSlice;
    
        std::for_each(myFrameSegments.begin(), myFrameSegments.end(), [&myTransformedFrameVertices, SlicePlane, &vSlice](const std::pair<int, int>& segment)
        {
            vector3d res;
            if( SlicePlane.intersect_segment(myTransformedFrameVertices[segment.first], myTransformedFrameVertices[segment.second], res) )
            {
                // test if the intersection point is unique
                auto it = std::find_if(vSlice.cbegin(), vSlice.cend(), [res](const vector3d& v) -> bool
                {
                    return vector3d(res - v).is_null();
                });
                
                // add unique intersection point
                if( it == vSlice.end() )
                {
                    vSlice.push_back(res);
                    assert(vSlice.size() <= 6); //!< maximum 6 intersection points
                }
            }
        });

        // skip this slice as it has less than two points and it's not a polygon
        if( vSlice.size() < 3 )
            continue;

        // sort vertices to preserve winding order after their angle in [-PI,PI] range
        // and basically we ignore the z coordinate because the points are in the same plane
        std::sort(vSlice.begin(), vSlice.end(), [](const vector3d& v1, const vector3d& v2) -> bool 
        {
            float a = atan2(v1.y, v1.x);
            float b = atan2(v2.y, v2.x);

            if(a<0.f) a += 2*PI;
            if(b<0.f) b += 2*PI;

            return a < b;
        });

        volume_slice slice;

        // transform the slice vertices back to world coordinates
        transform_coord_array(vSlice, mViewInv);

        slice.myVertices = vSlice;

        // compute the texture coordinates
        transform_coord_array(vSlice, mTex);

        slice.myTexture = vSlice;

        // add the slice to the list
        mySlices.push_back(slice);
    }
}

void vega::render::volume_texture_view::update()
{
    glDeleteTextures(eTex_ENUMNO, myTextureHandle);
    myFrameVertices.clear();
    myFrameSegments.clear();
    mySlices.clear();
    mySliceCount = 0;

    create();
}

//////////////////////////////////////////////////////////////////////////
void vega::render::volume_texture_controller::handle_mouse( int button, int state, int x, int y )
{
    std::shared_ptr<volume_texture_view> view = std::dynamic_pointer_cast<volume_texture_view>(myView);
    if( view )
        view->update_proxy_geometry();
}

void vega::render::volume_texture_controller::handle_keyboard( unsigned char key, int x, int y )
{
	switch( key )
	{
	case ' ':
		{
			std::shared_ptr<volume_texture_view> view = std::dynamic_pointer_cast<volume_texture_view>(myView);
			view->toggle_render_flag();
		}
		break;
	}
}