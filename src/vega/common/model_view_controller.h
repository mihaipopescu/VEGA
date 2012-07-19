#pragma once

#include <memory>
#include <algorithm>
#include <vector>


namespace vega
{
    // Model-View-Controller Design Pattern
    namespace mvc
    {
        // Observer pattern
        class i_observer
        {
        public:
            virtual ~i_observer() {}
            virtual void update() = 0;
        };

        class i_observable
        {
        public:
            virtual ~i_observable() {}

            void subscribe(const std::shared_ptr<i_observer> &o);
            void notify() const;

        protected:
            std::vector<std::weak_ptr<i_observer>> myObservers; //!< we need weak pointers to break the cycle View::Observable(Model)::Observer(View)
        };


        // Model interface from MVC Pattern
        class i_model : public i_observable
        {
        public:
            virtual ~i_model() {}

            virtual bool create() = 0;
        };

        // View interface from MVC Pattern
        class i_view : public i_observer
        {
        public:
            i_view() { }
            virtual ~i_view() {}

            void set_model(const std::shared_ptr<i_model>& _model) { myModel = _model; }

            virtual bool create() = 0;

            virtual void render() const = 0;

        protected:
            std::shared_ptr<i_model> myModel;
        };

        // Controller from MVC Pattern
        class i_controller
        {
        public:
            i_controller() { }
            virtual ~i_controller() {}

            void set_model(const std::shared_ptr<i_model>& _model) { myModel = _model; }
            void set_view(const std::shared_ptr<i_view>& _view) { myView = _view; }

            virtual void handle_keyboard(unsigned char key, int x, int y) { }
            virtual void handle_mouse(int button, int state, int x, int y) { }

        protected:
            std::shared_ptr<i_view> myView;
            std::shared_ptr<i_model> myModel;
        };

 
        void model_view_controller(const std::shared_ptr<i_model>& _model, const std::shared_ptr<i_view>& _view, const std::shared_ptr<i_controller>& _controller);
        
    }

}
