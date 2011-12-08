#ifndef __VEGA_RENDER_IMAGE_VIEW_H__
#define __VEGA_RENDER_IMAGE_VIEW_H__

#include "common.h"
#include "../common/types.h"
#include "../data/common.h"
#include "../data/image.h"

namespace vega
{
    namespace render
    {
        class image_view : public i_view
        {
        public:
            image_view() : myPolygonModeFill(true), myTexture(0) { }

            virtual bool create(const char* szFilename);
            virtual void create(const std::shared_ptr<data::image> & img);

            void toggle_polygon_fill() { myPolygonModeFill = !myPolygonModeFill; }

            std::shared_ptr<data::image> get_model() const { return std::dynamic_pointer_cast<data::image>(myModel); }

            virtual void render() const;
            
        protected:
            void create_texture(const std::shared_ptr<data::image> & img);

        private:
            uint32 myTexture;
            bool myPolygonModeFill;
        };

        class image_presenter : public i_presenter
        {
        public:
            image_presenter(const std::shared_ptr<image_view> & _view, const std::shared_ptr<data::image> & _model) : i_presenter(_view, _model) { }
            void handle_keyboard(unsigned char key, int x, int y);
        };
    }
}


#endif