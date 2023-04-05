#ifndef CONFIG_H
#define CONFIG_H
#include "misc.h"
#include "shittyintrospection.h"
#include <gui.h>
#include <guiddef.h>
typedef struct Config {
    Rect windowGeometry;
} Config;
extern Config config;
#endif  // CONFIG_H
