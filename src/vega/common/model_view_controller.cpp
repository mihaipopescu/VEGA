#include "model_view_controller.h"


void vega::mvc::i_observable::subscribe( const std::shared_ptr<i_observer> &o )
{
    myObservers.push_back(o); 
}

void vega::mvc::i_observable::notify() const
{
    std::for_each(myObservers.cbegin(), myObservers.cend(),
        [](const std::weak_ptr<i_observer>& o)
    {
        const std::shared_ptr<i_observer> so = o.lock();
        if( so )
            so->update();
    }
    );
}

void vega::mvc::model_view_controller( const std::shared_ptr<i_model>& _model, const std::shared_ptr<i_view>& _view, const std::shared_ptr<i_controller>& _controller )
{
    _model->subscribe(_view);
    _view->set_model(_model);
    _controller->set_model(_model);
    _controller->set_view(_view);
}
