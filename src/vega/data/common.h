#ifndef __VEGA_DATA_COMMON_H__
#define __VEGA_DATA_COMMON_H__

#include <memory>
#include <algorithm>
#include <vector>

namespace vega
{
    namespace data
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

            void subscribe(const std::shared_ptr<i_observer> &o) { myObservers.push_back(o); }
            void notify() const
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

        protected:
            std::vector<std::weak_ptr<i_observer>> myObservers; //!< we need weak pointers to break the cycle View::Observable(Model)::Observer(View)
        };


        // Model Interface
        class i_model : public i_observable
        {
        public:
            virtual ~i_model() {}

            virtual bool create() = 0;
        };
    }
}

#endif