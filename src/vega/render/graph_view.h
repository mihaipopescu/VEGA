#ifndef __VEGA_RENDER_GRAPH_VIEW__
#define __VEGA_RENDER_GRAPH_VIEW__

#include "common.h"
#include "../data/volume.h"


namespace vega
{
	namespace render
	{

		class graph_view : public i_view
		{
		public:
			graph_view() : myRenderFlag(false), myPrimitiveIsLine(true) { }

			void create(const std::shared_ptr<data::volume>& v);
            void create( vega::uint8 width, vega::uint8 height );

			void mst_kruskal();

			virtual void render() const;
			
			void toggle_render_flag() { myRenderFlag = !myRenderFlag; }
            void toggle_render_primitive() { myPrimitiveIsLine = !myPrimitiveIsLine; }

		private:
			bool myRenderFlag;
            bool myPrimitiveIsLine;
			std::vector<math::vector3d> myVertices;
            std::vector<uint16> myIndices;
			std::vector<math::vector4d> myColors;
        };


        class graph_presenter : public i_presenter
        {
		public:
			graph_presenter(const std::shared_ptr<graph_view> & _view, const std::shared_ptr<data::i_model> & _model) : i_presenter(_view, _model) { }
			virtual void handle_keyboard( unsigned char key, int x, int y );
		};
	}
}

#endif // __VEGA_RENDER_GRAPH_VIEW__