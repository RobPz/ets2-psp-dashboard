#pragma once

#include <oslib/oslib.h>
#include <png.h>

OSL_IMAGE* loadPNG(const char* filename, bool swizzle = true);