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
			graph_view() : myRenderFlag(false) { }

			void create(const std::shared_ptr<data::volume>& v);

			virtual void render() const;
			
			void toggle_render_flag() { myRenderFlag = !myRenderFlag; }

		private:
			bool myRenderFlag;
			std::vector<math::vector3d> myVertices;
		};


		class graph_presenter : public i_presenter
		{
		public:
			graph_presenter(const std::shared_ptr<graph_view> & _view, const std::shared_ptr<data::volume> & _model) : i_presenter(_view, _model) { }
			virtual void handle_keyboard( unsigned char key, int x, int y );
		};
	}
}

#endif // __VEGA_RENDER_GRAPH_VIEW__