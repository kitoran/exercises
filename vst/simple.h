#ifndef SIMPLE_H
#define SIMPLE_H

#include <ivstaudioprocessor.h>


class Simple : public Steinberg::Vst::IAudioProcessor {
    Steinberg::tresult initialize (FUnknown* context) override;


};



#endif // SIMPLE_H
