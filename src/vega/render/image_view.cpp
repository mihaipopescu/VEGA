#include <windows.h>
#include <GL/GL.h>
#include "image_view.h"
#include "../data/image.h"
#include "iostream"

using namespace vega::data;


bool vega::render::image_view::create()
{
    auto pImg = std::dynamic_pointer_cast<image>(myModel);
    if( pImg == NULL )
        return false;

    create_texture(pImg);
    return true;
}

void vega::render::image_view::render() const
{
    if( myTexture == 0 )
        return;

    glPolygonMode( GL_FRONT_AND_BACK, myPolygonModeFill ? GL_FILL : GL_LINE );

    glColor4f(1.f, 1.f, 1.f, 1.f);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, myTexture);               // Select Our Texture

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f * myASR, -0.5f, 0.5f);
    glTexCoord2f(1.f, 0.0f); glVertex3f(0.5f * myASR, -0.5f, 0.5f);
    glTexCoord2f(1.f, 1.0f); glVertex3f(0.5f * myASR, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f * myASR, 0.5f, 0.5f);

    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void vega::render::image_view::create_texture(const std::shared_ptr<image> & img)
{
    glGenTextures(1, &myTexture);
    glBindTexture(GL_TEXTURE_2D, myTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Linear Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Linear Filtering

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->get_width(), img->get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, static_cast<const GLvoid*>(img->get_raw_data()));

    myASR = (float)img->get_width() / (float)img->get_height();
}

void vega::render::image_view::update()
{
    glDeleteTextures(1, &myTexture);
    glBindTexture(GL_TEXTURE_2D, 0);

    create();
}

//////////////////////////////////////////////////////////////////////////

void vega::render::image_controller::handle_keyboard( unsigned char key, int x, int y )
{
    switch( key )
    {
    case 'f':
        {
            std::shared_ptr<image_view> view = std::dynamic_pointer_cast<image_view>(myView);
            view->toggle_polygon_fill();
        }
        break;
    }
}
