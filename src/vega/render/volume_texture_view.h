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
        class volume_texture_presenter;
        class volume_texture_view : public i_view
        {
            friend class volume_texture_presenter;
        public:
            volume_texture_view();

            bool create(const std::string& _FileName);
            virtual void render() const;

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
        };

        class volume_texture_presenter : public i_presenter
        {
        public:
            volume_texture_presenter(const std::shared_ptr<volume_texture_view> & _view, const std::shared_ptr<data::volume> & _model) : i_presenter(_view, _model) { }
            virtual void handle_mouse(int button, int state, int x, int y);
        };

    }
}


#endif