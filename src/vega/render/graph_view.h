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
			graph_view() : myRenderFlag(false), myPrimitiveIsLine(true), myVerticesAreIndexed(false) { }

			virtual bool create();

            virtual void update();

			void mst_kruskal();

			virtual void render() const;
			
			void toggle_render_flag() { myRenderFlag = !myRenderFlag; }
            void toggle_render_primitive() { myPrimitiveIsLine = !myPrimitiveIsLine; }

		private:
			bool myRenderFlag;
            bool myPrimitiveIsLine;
            bool myVerticesAreIndexed;
			std::vector<math::vector3d> myVertices;
            std::vector<uint32> myIndices;
			std::vector<math::vector4d> myColors;
        };


        class graph_controller : public i_controller
        {
        public:
			virtual void handle_keyboard( unsigned char key, int x, int y );
		};
	}
}

#endif // __VEGA_RENDER_GRAPH_VIEW__