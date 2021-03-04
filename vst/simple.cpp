#include <simple.h>

#include "VST_SDK/VST3_SDK/pluginterfaces/vst/ivsthostapplication.h"
using namespace Steinberg;

Steinberg::tresult Simple::initialize(Steinberg::FUnknown *context)
{
    FUnknownPtr<IHostApplication> hostApp (hostContext);
    if (hostApp)
    {
        String128 name;
        if (hostApp->getName (name) == kResultTrue)
        {
            //...
        }
    }
    //...

}
