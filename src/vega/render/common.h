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

        // Model-View-Presenter Pattern: Supervising Controller
        // http://msdn.microsoft.com/en-us/library/ff647543.aspx
        // The presenter updates the view to reflect changes in the model.
        // The interaction with the model is handled through the presenter.
        class i_presenter
        {
        public:
            i_presenter(const std::shared_ptr<i_view>& _view, const std::shared_ptr<vega::data::i_model>& _model) : myView(_view), myModel(_model) { }
            virtual ~i_presenter() {}

            virtual void handle_keyboard(unsigned char key, int x, int y) { }
            virtual void handle_mouse(int button, int state, int x, int y) { }

        protected:
            std::weak_ptr<i_view> myView;
            std::weak_ptr<vega::data::i_model> myModel;
        };

        // MVP: View Interface
        // How do I display my data ?
        class i_view 
        {
        public:
            i_view() { }
            virtual ~i_view() {}

            void set_presenter(const std::shared_ptr<i_presenter>& _presenter) { myPresenter = _presenter; }
            void set_model(const std::shared_ptr<vega::data::i_model>& _model) { myModel = _model; }

            void action_keyboard(unsigned char key, int x, int y) { myPresenter->handle_keyboard(key, x, y); }
            void action_mouse(int button, int state, int x, int y) { myPresenter->handle_mouse(button, state, x, y); }

            template <class Model, class View, class Presenter>
            static std::shared_ptr<View> factory_create()
            {
                std::shared_ptr<Model> myModel = std::make_shared<Model>();
                std::shared_ptr<View> myView = std::make_shared<View>();
                std::shared_ptr<Presenter> myPresenter = std::make_shared<Presenter>(myView, myModel);

                // make connections
                myView->set_presenter(myPresenter);
                myView->set_model(myModel);

                return myView;
            }

            virtual void render() const = 0;

        public:
            std::shared_ptr<i_presenter> myPresenter;
            std::shared_ptr<vega::data::i_model> myModel;
        };

    }
}


#endif // __VEGA_RENDER_COMMON_H__