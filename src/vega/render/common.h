#ifndef __VEGA_RENDER_COMMON_H__
#define __VEGA_RENDER_COMMON_H__

#include <memory>
#include "../data/common.h"

namespace vega
{
    namespace render
    {
        class vega::data::i_model;
        class i_view;

        // Model-View-Controller Design Pattern
        class i_controller
        {
        public:
            i_controller() { }
            virtual ~i_controller() {}

            void set_model(const std::shared_ptr<vega::data::i_model>& _model) { myModel = _model; }
            void set_view(const std::shared_ptr<i_view>& _view) { myView = _view; }

            virtual void handle_keyboard(unsigned char key, int x, int y) { }
            virtual void handle_mouse(int button, int state, int x, int y) { }

        protected:
            std::shared_ptr<i_view> myView;
            std::shared_ptr<vega::data::i_model> myModel;
        };

        class i_view : public data::i_observer
        {
        public:
            i_view() { }
            virtual ~i_view() {}

            void set_model(const std::shared_ptr<vega::data::i_model>& _model) { myModel = _model; }

            virtual bool create() = 0;

            virtual void render() const = 0;

        protected:
            std::shared_ptr<vega::data::i_model> myModel;
        };

        static void model_view_controller(const std::shared_ptr<vega::data::i_model>& _model, const std::shared_ptr<i_view>& _view, const std::shared_ptr<i_controller>& _controller)
        {
            _model->subscribe(_view);
            _view->set_model(_model);
            _controller->set_model(_model);
            _controller->set_view(_view);
        }

    }
}


#endif // __VEGA_RENDER_COMMON_H__