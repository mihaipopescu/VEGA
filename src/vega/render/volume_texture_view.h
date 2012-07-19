#ifndef __VEGA_RENDER_VOLUME_TEXTURE_VIEW_H__
#define __VEGA_RENDER_VOLUME_TEXTURE_VIEW_H__

#include "common.h"
#include "../data/volume.h"
#include "../math/vector3d.h"
#include <string>

namespace vega
{
    namespace render
    {
        class volume_texture_controller;
        class volume_texture_view : public mvc::i_view
        {
            friend class volume_texture_controller;
        public:
            volume_texture_view();

            virtual bool create();

            virtual void update();

            virtual void render() const;

			void toggle_render_flag() { myRenderFlag = !myRenderFlag; }

        protected:
            void update_proxy_geometry();

            enum eTextures
            {
                eTex_Volume,
                eTex_TranferFunction,
                eTex_ENUMNO
            };

            uint32 myTextureHandle[2];
            
            std::vector<vega::math::vector3d> myFrameVertices;
            std::vector<std::pair<int, int>> myFrameSegments;

            struct volume_slice
            {
                std::vector<vega::math::vector3d> myVertices;
                std::vector<vega::math::vector3d> myTexture;
            };

            std::vector<volume_slice> mySlices;
            int mySliceCount;
			bool myRenderFlag;
        };

        class volume_texture_controller : public mvc::i_controller
        {
        public:
            virtual void handle_mouse(int button, int state, int x, int y);
			virtual void handle_keyboard( unsigned char key, int x, int y);
        };

    }
}


#endif