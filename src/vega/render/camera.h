#ifndef __VEGA_RENDER_CAMERA_H__
#define __VEGA_RENDER_CAMERA_H__


namespace vega
{
    namespace render
    {
        class camera
        {
        public:
            camera();

            void reset();
            void apply();
            void render() const;

            void handle_mouse_buttons(int button, int state, int x, int y);
            void handle_mouse_motions(int x, int y);
            void handle_common_keys(unsigned char key, int x, int y);
            void handle_special_keys(int key, int x, int y);

        protected:
            float myPitch;
            float myYaw;
            float myRoll;
            float myOffsetZ;
            float myOffsetX;
            float myOffsetY;
            float myOffsetDelta;
            float myScale;

            bool myMouseLeftDown;
            bool myMouseRightDown;
            bool mouseMiddleDown;
            int myMouseX, myMouseY;
        };
    }
}



#endif // __VEGA_RENDER_CAMERA_H__