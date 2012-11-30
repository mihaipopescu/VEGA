#include <iostream>
#include <strstream>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <GL/wglew.h>
#include <assert.h>

#include "../vega/common/logger.h"
#include "../vega/common/profiler.h"
#include "../vega/data/bitmap_graph.h"
#include "../vega/data/image.h"
#include "../vega/data/resizeable_image.h"
#include "../vega/data/volume.h"
#include "../vega/data/volume_graph.h"
#include "../vega/data/volume_primitive.h"
#include "../vega/data/resizeable_volume.h"
#include "../vega/render/camera.h"
#include "../vega/render/graph_view.h"
#include "../vega/render/image_view.h"
#include "../vega/render/volume_texture_view.h"
#include "../vega/data/compact_hexagonal_lattice.h"


/*! \mainpage VisualVega
 *
 * \author Mihai Popescu
 * \copyright All rights reserved.
 * \date 2011
 * \version 0.1
 * \details VisualVega is the front-end for Visual Volume Segmentation Algorithm.
 * \warning Application is in development. Improper use can crash your system.
 * \note See
 * <a href="http://www.opengl.org/resources/code/samples/advanced/advanced97/notes/node179.html#SECTION000150000000000000000">Volume visualization OpenGL example</a>
 * and <a href="http://www.opengl.org/sdk/docs/man/xhtml/glTexImage3D.xml">glTexImage3D</a> for more information on OpenGL volume rendering.
 * 
 */

using namespace std;
using namespace vega;
using namespace vega::utils;
using namespace vega::render;
using namespace vega::data;


// globals
int g_WindowHandle = 0;
int g_WindowWidth = 800;
int g_WindowHeight = 600;
unsigned g_FrameCount = 0;

camera g_cam;

//#define UNIT_TEST_2D
#define UNIT_TEST_3D
#define SYNTHETIC_SCENE


std::vector<std::shared_ptr<mvc::i_view>> g_Views;
std::vector<std::shared_ptr<mvc::i_controller>> g_Controllers;


void displayCB(void)
{
    ++g_FrameCount;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
    // apply camera transform
    g_cam.apply();
    
    g_cam.render();

    // render objects
    for(auto it = begin(g_Views); it != end(g_Views); it++)
    {
        it->get()->render();
    }

    glutSwapBuffers();
    glutPostRedisplay();

//#ifdef _DEBUG
//    GLenum errCode;
//    if( (errCode = glGetError()) != GL_NO_ERROR )
//    {
//        cout << "OpenGL error:" << gluErrorString(errCode) << endl;
//        assert(false);
//    }
//#endif
}

void reshapeCB(int w, int h) 
{
    // compute aspect ratio
    float aspr = h != 0 ? 1.0f * w / h : 1.f;

    // Use the Projection Matrix
    glMatrixMode(GL_PROJECTION);

    // Reset Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set the correct perspective.
    gluPerspective(45, aspr, 1, 1000);

    // Get Back to the Modelview
    glMatrixMode(GL_MODELVIEW);
}

void cleanupCB()
{
    logger::destroy();
    profiler::destroy();
}

void timerCB(int value)
{
    if( value != 0 )
    {
        strstream str;
        str << g_FrameCount * 4 << " fps @ " << g_WindowWidth << "x" << g_WindowHeight << ends;
        glutSetWindowTitle(str.str());
    }
    
    g_FrameCount = 0;
    glutTimerFunc(250, timerCB, 1);
}

void keyboardCB(unsigned char key, int x, int y)
{
    g_cam.handle_common_keys(key, x, y);

    for(auto it = begin(g_Controllers); it != end(g_Controllers); it++)
    {
        it->get()->handle_keyboard(key, x, y);
    }

    switch(key)
    {
    case VK_ESCAPE:
        glutLeaveMainLoop();
        break;
    }
}

void specialCB(int key, int x, int y)
{
    g_cam.handle_special_keys(key, x, y);
}

void mouseCB(int button, int state, int x, int y)
{
   g_cam.handle_mouse_buttons(button, state, x, y);

   for(auto it = begin(g_Controllers); it != end(g_Controllers); it++)
   {
       it->get()->handle_mouse(button, state, x, y);
   }
}

void mouseMotionCB(int x, int y)
{
    g_cam.handle_mouse_motions(x, y);

    for(auto it = begin(g_Controllers); it != end(g_Controllers); it++)
    {
        it->get()->handle_mouse(0, 0, x, y);
    }

    glutPostRedisplay();
}

bool init_glut(int argc, char **argv)
{
    // init GLUT and create Window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(400, 100);
    glutInitWindowSize(g_WindowWidth, g_WindowHeight);
    g_WindowHandle = glutCreateWindow("Visual VEGA");

    // register callbacks
    glutDisplayFunc(displayCB);
    glutIdleFunc(displayCB);
    glutReshapeFunc(reshapeCB);
    glutKeyboardFunc(keyboardCB);
    glutSpecialFunc(specialCB);
    glutMouseFunc(mouseCB);
    glutMotionFunc(mouseMotionCB);
    glutCloseFunc(cleanupCB);
    glutTimerFunc(0, timerCB, 0);

    return true;
};

bool init_gl_objects()
{
    profiler::create();
    profiler::get()->set_dump_on_end(false);

    logger::create();
    logger::get()->initialize();

#ifdef UNIT_TEST_2D

    model_view_controller(g_resimg, g_imgView, g_imgCtrl);

    if( !g_resimg->create("data/image/lenna.bmp") )
		return false;

    g_resimg->resample<vega::algorithm::resample::bilinear_filter>(128, 256);
    g_imgView->create();
    
    g_resvol->load("data/volume/synthetic.vega", false);
    g_resvol->resample<vega::algorithm::resample::bilinear_filter>(64, 16, 32);    

    //g_graph->create();
#endif

#ifdef UNIT_TEST_3D
#ifdef SYNTHETIC_SCENE
	
#if 0
    std::shared_ptr<volume_primitive> vp = std::make_shared<volume_primitive>(32, 32, 32);
	vp->create_sphere(16.f, 16.f, 16.f, 16.f, MAXDENSITY / 8);
	vp->create_sphere(16.f, 16.f, 16.f, 10.f, MAXDENSITY / 4);
	vp->create_sphere(16.f, 16.f, 16.f, 6.f, MAXDENSITY);
	vp->save_as("data/volume/synthetic.raw");
#else
    
#endif

    std::shared_ptr<resizeable_volume> vol = std::make_shared<resizeable_volume>("data/volume/bonsai.vega");

    if( !vol->create() )
        return false;

    vol->resample<vega::algorithm::resample::bilinear_filter>(64, 64, 64);
    vol->save_as("data/volume/bonsai_lowres.raw");

    std::shared_ptr<volume_graph> vg = std::make_shared<volume_graph>((std::shared_ptr<volume>&)(vol));

    if( !vg->create() )
        return false;

    auto voltex = std::make_shared<volume_texture_view>();
    auto volctrl = std::make_shared<volume_texture_controller>();

    auto graph3d = std::make_shared<graph_view>();
    auto graphctrl = std::make_shared<graph_controller>();

    model_view_controller(vg, voltex, volctrl);
    model_view_controller(vg, graph3d, graphctrl);

    g_Views.push_back(voltex);
    g_Views.push_back(graph3d);

    g_Controllers.push_back(volctrl);
    g_Controllers.push_back(graphctrl);

    graph3d->create();
    voltex->create();

#else
	std::shared_ptr<resizeable_volume> v = std::make_shared<resizeable_volume>("data/volume/bonsai.vega");
    
    if( !v->create() )
        return false;

    v->resample<vega::algorithm::resample::bilinear_filter>(64, 64, 64);
    v->save_as("resized_bonsai.raw");

    auto voltex = std::make_shared<volume_texture_view>();
    auto volctrl = std::make_shared<volume_texture_controller>();

    model_view_controller(v, voltex, volctrl);

    g_Views.push_back(voltex);
    g_Controllers.push_back(volctrl);

    voltex->create();
	
#endif
#endif

    return true;
}

bool init_gl()
{
    glShadeModel(GL_SMOOTH);                            // Enable Smooth Shading
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);               // Black Background
    glClearDepth(1.0f);                                 // Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);                            // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                             // The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Really Nice Perspective Calculations
    
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );        // or GL_LINE
    
    glDisable(GL_CULL_FACE);
    glEnable(GL_ALPHA_TEST);

    return true;                                        // Initialization Went OK
}

bool init(int argc, char **argv)
{
    if( !init_glut(argc, argv) )
    {
        cout << "GLUT init failed !" << endl;
        return false;
    }

    if( !init_gl() )
    {
        cout << "GL init failed !" << endl;
        return false;
    }

    GLenum GlewInitResult = glewInit();
    if (GLEW_OK != GlewInitResult)
    {
        cout << "GLEW init failed with error: " << glewGetErrorString(GlewInitResult) << endl;
        return false;
    }

    GLenum errCode;
    if( (errCode = glGetError()) != GL_NO_ERROR )
    {
        cout << "Encountered OpenGL error: " << gluErrorString(errCode) << endl;
        return false;
    }

    if( !GLEW_EXT_texture3D )
    {
        cout << "OpenGL doesn't support 3d textures !" << endl;
        return false;
    }

	if( !wglSwapIntervalEXT(1) )
	{
		cout << "Error enabling VSync !" << endl;
		return false;
	}

    if( !init_gl_objects() )
    {
        cout << "Failed initializing application objects !" << endl;
        return false;
    }

    cout << "Init Ok. OpenGL Version: " << glGetString(GL_VERSION) << endl;
    return true;
}

int main(int argc, char **argv) 
{
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    //_CrtSetBreakAlloc(159);

    if( !init(argc, argv) )
    {
        cleanupCB();
        return 1;
    }

    std::cout << "VisualVega initialized. Entering GUI mode..." << std::endl;

    // enter GLUT event processing cycle
    glutMainLoop();

    return 0;
}
