#ifndef __VEGA_DATA_COMMON_H__
#define __VEGA_DATA_COMMON_H__

namespace vega
{
    namespace data
    {
        // MVP: Model Interface
        // Model = What is my data ?
        // Selections = How do I specify my data ?
        // Commands = How do I change my data ?
        class i_model
        {
        public:
            i_model() {}
            virtual ~i_model() {}
        };
    }
}

#endif